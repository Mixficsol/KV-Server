#include "csapp.h"
#include "kv_conn.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "storage_engine.h"
#include "kv_command.h"
#include "csapp.h"
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
  : auth_(false) {
}

Conn::~Conn() {

}

typedef struct redisCommand {
  char* name;
  int parameter_num;
  void (*pf)(const std::vector<std::string>&, std::string* const);
} redisCommand;

void Conn::ProcessNewConn(const int& listenfd) {  // 建立服务器客户端连接
  struct sockaddr_in clientaddr;
  socklen_t clientlen;
  offset_ = 0;
  read_ = false;
  instruction_len_ = 798;
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

std::vector<std::string> Conn::NormalFinterpreter() {  // 解析正常数据
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
}

void Conn::Splice() {   // 将数字字符串转为整形数字
  if (order == '*') {
    num.append(buffer, analysis_pos, read_pos - analysis_pos);
    instruction_len_ = atoi(num.c_str());  // *后的数字字符串转换为整形数字
    num.clear();
    analysis_pos = read_pos;
    LOG(INFO) << "instruction_len_:  " << instruction_len_;
    Assert();
  } else if (order == '$') {
    number.append(buffer, analysis_pos, read_pos - analysis_pos); 
    left = statement_len_ = atoi(number.c_str());  // $后的数字字符串转换为整形数字
    number.clear();
    analysis_pos = read_pos;
    std::cout << "statement_len_: " << statement_len_ << "  left: " << left << std::endl;
    Assert();
    if (flag) {
      Add();
    }
  }
}

void Conn::Add() {
  std::cout << "left: " << left << std::endl;
  std::cout << "read_pos: " << read_pos << std::endl;
  std::cout << "len: " << read_pos + left << std::endl;
  if (read_pos + left < buffer.size() && left) {
    LOG(INFO) << "pre_data: " << data;
    data.append(buffer, read_pos, left);
    LOG(INFO) << "data: " << data;
    serialize_data.push_back(data);
    data.clear();
    read_pos += left;
    analysis_pos = read_pos;
    left = 0;
  } else if (read_pos + left == buffer.size() && left) {
    data.append(buffer, read_pos, left);
    LOG(INFO) << "Pre_data: " << data;
    serialize_data.push_back(data);
    LOG(INFO) << "Data: " << data;
    left = 0;
    data.clear();
    read_pos = buffer.size();
    analysis_pos = read_pos;
    left = 0;
    flag = false;
  } else {
    data.append(buffer, read_pos, buffer.size() - read_pos);
    left = left + read_pos - buffer.size();
    read_pos = buffer.size();
    analysis_pos = read_pos;
    flag = false;
  }
  Assert();
  return;
}

void Conn::Join() {
  if (order == '*') {
    num.append(buffer, analysis_pos, buffer.size() - analysis_pos);
  } else if (order == '$') {
    number.append(buffer, analysis_pos, buffer.size() - analysis_pos);
  }
  flag = false;
}

void Conn::Assert() {
  LOG(INFO) << "read_pos: " << read_pos;
  LOG(INFO) << "OrDer: " << order;
  if (read_pos + 2 < buffer.size()) {
    LOG(INFO) << "AAAAAAAAAA";
    assert(buffer[read_pos] == '\r');
    assert(buffer[read_pos + 1] == '\n');
    if (order == '&') {
      instruction_len_--;
    }
    if (buffer[read_pos + 2] == '$') {
      order = '$';
      if (read_pos + 3 >= buffer.size()) {
        read_pos = buffer.size();
        flag = false;
        LOG(INFO) << "after order: " << order;
        return;
      }
      read_pos += 3;
      analysis_pos = read_pos;
    } else {
      order = '&';
      read_pos += 2;
      analysis_pos = read_pos;
    }
  } else if (read_pos + 1 == buffer.size() - 1) {
      LOG(INFO) << "HHHHHH";
      assert(buffer[read_pos] == '\r');
      assert(buffer[read_pos + 1] == '\n');
      if (order == '&') {
        instruction_len_--;
        order = '$';
      } else if (order == '$') {
        order = '&';
      }
      read_pos = buffer.size();
      flag = false;
  } else {
     LOG(INFO) << "NNNNNNNN";
    flag = false;
  }
  LOG(INFO) << "After order: " << order;
}

void Conn::Parsing() {  // 指令解析
  for (read_pos = analysis_pos; read_pos < buffer.size(); read_pos++) {
    LOG(INFO) << "index: " << read_pos;
    if (buffer[read_pos] != '\r') { 
      assert(buffer[read_pos] >= '0' && buffer[read_pos] <= '9');
    }
    if (buffer[read_pos] == '\r') {
      Splice();
      return;
    }
    if (read_pos == buffer.size() - 1) {
      Join();
      return;
    }
  }
}

void Conn::Finterpreter() { // 解析序列化后的数据
  buffer.clear();
  flag = true;
  read_pos = analysis_pos = 0;
  for (int i = 0; i < 20; i++) {
    if (read_buffer_[i] == '\0') {
      break;
    }
    buffer.push_back(read_buffer_[i]);
  }
  LOG(INFO) << "buffer: " << buffer;
  std::cout << "buffer.size: " << buffer.size() << std::endl;
  LOG(INFO) << "INST: " << instruction_len_;
  if (buffer[read_pos] == '*' && instruction_len_ == 798) {  // 对序列化句子的第一次判断
    order = '*';
    analysis_pos = read_pos = 1;
    is_standard_ = true;
  }
  if (buffer[read_pos] == '$' && order == '$') {
    read_pos += 1;
    analysis_pos = read_pos;
  }
  LOG(INFO) << "order: " << order;
  if (buffer[read_pos] == '\r') {  // buffer开头是'\r'则先判断
    Assert();  // 若buffer开头是'\r'则先进行判断
  }
  if (order == '&') { // 如果上次字符串未读完则继续读
    Add();
  }
  int cnt = 10;
  while (is_standard_ && flag && cnt--) {
    Parsing();
  }
}

void Conn::AnalyticData() {
  read_pos = analysis_pos = 0;
  memset(read_buffer_ + offset_, '\0', MAXLINE - offset_);
  int n = read(fd_, read_buffer_ + offset_, MAXLINE - offset_);
  LOG(INFO) << "read_buffer_: " << read_buffer_;
  std::cout << "n: " << n << std::endl;
  Finterpreter(); // 序列化解析函数
  std::cout << "instruction_len_: " << instruction_len_ << "       left: " << left << std::endl;
  if (instruction_len_ == 0 && left == 0) {
    LOG(INFO) << "offset_: " << offset_;
    GetRequest();
    return;
  } else {
    offset_ = buffer.size() - read_pos; // 计算menmove的值
    LOG(INFO) << "offset_: " << offset_;
    memmove(read_buffer_, read_buffer_ + read_pos, sizeof(char) * offset_);  // [0][1][2][3][4] -> [2][3][4][x][x]  
    analysis_pos = read_pos = 0;
    ClusterEpoll::SetRead(fd_);
  }
}

void Conn::Init() {
  statement_len_ = 0;
  instruction_len_ = 798;
  left = 0;
  offset_ = 0;
  is_standard_ = false;
  analysis_pos = 0;
  read_pos = 0;
  order = '#';
  serialize_data.clear();
  num.clear();
  number.clear();
  data.clear();
  buffer.clear();
  memset(read_buffer_, '\0', MAXLINE);
}

void Conn::GetRequest() {
  if (!FDException(fd_)) {
    std::cout << "serialize_data.size: " << serialize_data.size() << std::endl;
    std::vector<std::string>::iterator iter = serialize_data.begin();
  //  LOG(INFO) << "-------------------------------";
    LOG(INFO) << "serialize_data: " << serialize_data[0];
    std::string Order = serialize_data[0]; 
    Encode::orderTolower(serialize_data[0]);
  /*  if (!Order.compare("auth")) {
      auth_ = Command::AuthCommandImpl(serialize_data, &reply);
    }*/ 

  //  if (!Order.compare("command")) {
      struct redisCommand rediscommand = Command::lookupCommand(serialize_data[0], serialize_data);
      void (*pd)(const std::vector<std::string>&, std::string* const) = rediscommand.pf;
      pd(serialize_data, &reply);
  //  } 
 /*   else if (!auth_) {
      Command::AutherrorCommandImpl(serialize_data, &reply);
    }*/// else {
    //  Command::ErrorCommandImpl(serialize_data, &reply);
   // }
  } else {
    
  }
  Init();
  ClusterEpoll::SetWrite(fd_);
}

void Conn::SendReply() {
  int32_t nwrite = 0;
  int32_t total_len = reply.size();
  while (true) {
    int n = write(fd_, reply.data() + nwrite, total_len - nwrite);
    nwrite += n;
    if (nwrite == reply.size()) {
      break;
    }
  }
  ClusterEpoll::SetRead(fd_);
}

