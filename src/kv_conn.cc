#include "csapp.h"
#include "kv_conn.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "storage_engine.h"
#include "kv_command.h"
#include "csapp.h"
#include "serverstats.h"
#include "kv_exception.h"
#include "kv_cluster_epoll.h"
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include <glog/logging.h>

using namespace leveldb;

Conn::Conn()
  : is_connect_(true),
    auth_(false),
    is_end_parsing_(false), 
    is_end_read_(false),
    offset_(0),
    read_pos_(0),
    analysis_pos_(0),
    bluk_length_(-1),
    state_(kBluk) {
}

Conn::~Conn() {

}

extern std::map<int, Conn*> conn_map;
extern int keepRunning;
typedef struct redisCommand {
  char* name;
  int parameter_num;
  void (*pf)(const std::vector<std::string>&, std::string* const);
} redisCommand;

void Conn::ProcessNewConn(const int& listenfd) {  // 建立服务器客户端连接
  struct sockaddr_in clientaddr;
  socklen_t clientlen;
  data_.clear();
  Reply_.clear();
  clientlen = sizeof(struct sockaddr_storage);
  int connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
  if (!FDException(connfd)) {
    fd_ = connfd;
  } else {  
    exit(1);
  }
}

int Conn::GetFD() {
  return fd_;
}

bool Conn::IsConnect() {
  return is_connect_;
}

/*std::vector<std::string> Conn::NormalFinterpreter() {  // 解析正常数据
  std::vector<std::string> normal_data;
  int cnt, index = 0;
  std::string order, key, value;
  char* buf = read_buffer_;
  cnt = Encode::getCharLength(buf);
  order = Encode::getOrder(buf, index, cnt);
  Encode::orderTolower(order); // 命令小写化
  normal_data.push_back(order);
  key = Encode::getOrder(buf, index, cnt);
  normal_data.push_back(key);
  value = Encode::getOrder(buf, index, cnt);
  normal_data.push_back(value);
  return normal_data;
}*/

void Conn::Splice() {   // 将数字字符串转为整形数字
  buffer_ = read_buffer_; 
  if (state_ == kBluk) {
    bluk_length_ = atoi(buffer_.substr(analysis_pos_, read_pos_ - analysis_pos_).c_str());  // *后的数字字符串转换为整形数字
   // LOG(INFO) << "bluk_length_: " << bluk_length_;
    Assert(); // 处理'\r','\n'的情况
  } else if (state_ == kMultiBluk) {
    left_ = multibluk_length_ = atoi(buffer_.substr(analysis_pos_, read_pos_ - analysis_pos_).c_str());  // $后的数字字符串转换为整形数字
   // LOG(INFO) << "multibluk_length_: " << multibluk_length_;
    Assert();
    if (!is_end_parsing_) {
      Add();
    }
  }
}

void Conn::Add() {
  buffer_ = read_buffer_;
//  LOG(INFO) << "left: " << left_ << " read_pos_: " << read_pos_;
//  LOG(INFO) << "pre_data: " << data_;
  int num = std::min(left_, read_buffer_size_ - analysis_pos_);
  data_.append(buffer_, analysis_pos_, num);
  if (num == 0) {

  } else if (num == left_ || left_ == read_buffer_size_ - analysis_pos_) {
    left_ = 0;
    argv_.push_back(data_);
  //  LOG(INFO) << "data_: " << data_;
    data_.clear();
    read_pos_ = read_pos_ + num;
  } else if (num == read_buffer_size_ - analysis_pos_) {
    left_ -= num;
    read_pos_ = read_buffer_size_;
  }
  Assert();
}

void Conn::Assert() {
  if (read_pos_ == read_buffer_size_) {
    is_end_parsing_ = true;
   // LOG(INFO) << "exit: 1";
    return;
  }
  assert(read_buffer_[read_pos_] == '\r');
  if (read_pos_ == read_buffer_size_ - 1) {
    is_end_parsing_ = true;
 //   LOG(INFO) << "exit: 2";
    return;
  } 
  assert(read_buffer_[read_pos_ + 1] == '\n');  
  if (state_ == kMultiBluk) { // $后面一定是拼接字符串
    state_ = kParseString;
  } else {  // *和拼接字符串后面一定是$
    if (state_ == kParseString) { 
      bluk_length_--;
    }
    state_ = kMultiBluk;
  }  
  if (bluk_length_ == 0 && left_ == 0) {
    if (read_pos_ + 1 == read_buffer_size_ - 1 || read_buffer_[read_pos_ + 2] != '*') {
      is_end_read_ = true;
      is_end_parsing_ = true;
      read_pos_ = read_buffer_size_;
   //   LOG(INFO) << "enter 1";
    } else if (read_pos_ + 2 == read_buffer_size_ - 1) {
      is_end_parsing_ = true;
      read_pos_ = read_buffer_size_;
     // LOG(INFO) << "enter 2";
    } else {
      read_pos_ += 3;
    }
    GetRequest();
    Reset();
 //   LOG(INFO) << "exit 3";
  //  LOG(INFO) << "is_end_parsing_ " << is_end_parsing_;
    return;
  }
  if (read_pos_ + 1 == read_buffer_size_ - 1) {
    is_end_parsing_ = true;
    read_pos_ = read_buffer_size_;
   // LOG(INFO) << "exit 4";
    return;
  }
  if (read_buffer_[read_pos_ + 2] == '$' && state_ == kMultiBluk) {
    if (read_buffer_[read_pos_ + 2] == read_buffer_size_ - 1) {
      is_end_parsing_ = true;
      read_pos_ = read_buffer_size_;
    } else {
      read_pos_ += 3;
    }
//    LOG(INFO) << "exit 5";
  } else {
    read_pos_ += 2;
    analysis_pos_ = read_pos_;
  //  LOG(INFO) << "exit 6";
  }
}

void Conn::Parsing() {  // 指令解析
  char* pos = strchr(read_buffer_, '\r');
  read_pos_ = pos - read_buffer_;
 // LOG(INFO) << "NUM: " << read_buffer_[0];
  if (pos == NULL || read_pos_ >= read_buffer_size_) {
    is_end_parsing_ = true;
    read_pos_ = analysis_pos_;
  } else {
   // LOG(INFO) << "Read_pos_num: " << read_buffer_[read_pos_];
   // LOG(INFO) << "read_pos: " << read_pos_;
    Splice();
  }
}

void Conn::Memmove() {
//  LOG(INFO) << "read_pos_num: " << read_buffer_[read_pos_] << "  read_pos_num - 1: "  << read_buffer_[read_pos_ - 1];
 // LOG(INFO) << "read_pos: " << read_pos_ << "  read_buffer_size_: " << read_buffer_size_;
  offset_ = read_buffer_size_ - read_pos_; // 计算menmove的值
  memmove(read_buffer_, read_buffer_ + read_pos_, sizeof(char) * offset_);  // [0][1][2][3][4] -> [2][3][4][x][x]
  read_buffer_size_ = offset_;
  analysis_pos_ = read_pos_ = 0;
//  LOG(INFO) << "offset_: " << offset_ << "  read_buffer_size_: " << read_buffer_size_ << "strlen(read): " << strlen(read_buffer_);
}

void Conn::Finterpreter() { // 解析序列化后的数据
//  LOG(INFO) << "read_buffer_: " << read_buffer_;
 // LOG(INFO) << "read_buffer_size_: " << strlen(read_buffer_);
  is_end_parsing_ = false; 
  read_buffer_size_ = actual_read_ + offset_;  // read_buffer的实际大小
 // LOG(INFO) << "Order: " << state_;
  if (read_buffer_[read_pos_] == '*' && bluk_length_ == -1) {  // 对序列化句子的第一次判断
    state_ = kBluk;
    analysis_pos_ = read_pos_ = 1;
  } else if (state_ == kParseString) {
    Add();
    Memmove();
  } else if (state_ == kMultiBluk && read_buffer_[read_pos_] == '$') {
    read_pos_ += 1;
    Memmove();
  } else if (read_buffer_[read_pos_] == '\r' || read_buffer_[read_pos_] == '\n')  {
    Assert();
   // LOG(INFO) << "order: " << state_;
   // LOG(INFO) << "State_ : " << state_;
    if (state_ != kBluk) {
      Add();
    }
    Memmove();
  } 
//  LOG(INFO) << "State_ : " << state_;
  while (!is_end_parsing_) {
    Parsing();
    Memmove();
  //  LOG(INFO) << "is_end_parsing_: " << is_end_parsing_;
  }
  if (!is_end_read_) {
  //  LOG(INFO) << "this";
    ClusterEpoll::SetRead(fd_); // 设置可读事件
  }

}

void Conn::AnalyticData() {
  memset(read_buffer_ + offset_, '\0', MAXLINE - offset_);
  actual_read_ = read(fd_, read_buffer_ + offset_, MAXLINE - offset_);
  ServerStats::GetCurrent()->AddInputBytes(actual_read_);
  if (actual_read_ == 0) { // 客户端断连
    is_connect_ = false;
    return;
  }
 // LOG(INFO) << "actual_read: " << actual_read_;
  Finterpreter(); // 序列化解析函数,每次有新的东西读进来一定要执行解析函数
}

void Conn::Reset() {
  bluk_length_ = -1;
  multibluk_length_ = -1;
  left_ = -1;
  state_ = kBluk;
  argv_.clear();
  data_.clear();
}

void Conn::GetRequest() {
  if (!FDException(fd_)) {
    std::vector<std::string>::iterator iter = argv_.begin();
    std::string Order = argv_[0]; 
    Encode::orderTolower(argv_[0]);
   // LOG(INFO) << "-----------------------------------";
   // LOG(INFO) << "Order: " << argv_[0];
   // LOG(INFO) << "is_end_read_: " << is_end_read_;
    struct redisCommand rediscommand = Command::lookupCommand(argv_[0], argv_);
    void (*pd)(const std::vector<std::string>&, std::string* const) = rediscommand.pf;
    pd(argv_, &reply_);
    if (argv_[0] == "exit") {
      is_connect_ = false;
    } else if (argv_[0] == "shutdown") {
      keepRunning = 0;
    }
  } else {
    is_connect_ = false;
  }
  ServerStats::GetCurrent()->AddCommandProcessed();
  Reply_.append(reply_);
  reply_.clear();
//  LOG(INFO) << "Reply_size: " << Reply_.size();
  if (is_end_read_) {
    ClusterEpoll::SetWrite(fd_);
  }
}

void Conn::SendReply() {
  int32_t nwrite = 0;
  int32_t total_len = Reply_.size();
  while (true) {
    int n = write(fd_, Reply_.data() + nwrite, total_len - nwrite);
    ServerStats::GetCurrent()->AddOutputBytes(n);
    if (n == 0) { /* 暂时写不进去 */
      return;
    } else if (n == -1) {
     /*error*/
    }
    nwrite += n;
    if (nwrite == total_len) { // 已经写入的和实际reply的长度一样表示写完了
      break;
    }
  }
  ClusterEpoll::SetRead(fd_); /* 只注册可读事件(已经移除可写事件了) */
  is_end_read_ = false;
  Reply_.clear();
}

