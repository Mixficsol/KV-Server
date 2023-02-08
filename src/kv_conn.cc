#include "csapp.h"
#include "kv_conn.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "storage_engine.h"
#include "kv_command.h"
#include "csapp.h"
#include "kv_exception.h"

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

std::vector<std::string> Conn::Finterpreter(int* const cnt, int* const cur_pos) { // 解析序列化后的数据
  int size;
  *cur_pos = 0;
  std::vector<std::string> serialize_data;
  char* buf = read_buffer_;
  std::string in = (std::string)buf;
  Encode::Forwardsearch(in, cur_pos);
  while (*cnt) {
    if (!Encode::JudgeOrder(in, cur_pos)) {
      break;
    }
    if (!Encode::paramtertotal(in, cur_pos, &size)) {
      break;
    }
    if (!Encode::Split(in, &serialize_data, cur_pos, &size)) {
      break;
    } 
    (*cnt)--;
  }
  return serialize_data;
}

int Conn::Finterpretersize() { // 解析序列化后的数据
  int cur_pos, size;
  cur_pos = 1;
  std::string in;
  char* buf = read_buffer_;
  in = (std::string)buf;
  assert(Encode::paramtertotal(in, &cur_pos, &size));
  return size;
}

void Conn::GetRequest() {
  if (!FDException(fd_)) {
    int size = MAXLINE, cur_pos;
    std::vector<std::string> serialize_data;
    memset(read_buffer_, '\0', sizeof(read_buffer_));
    int len = 0;
  //  LOG(INFO) << "-------------------------------begin-------------------------------";
    while (size) {
    //  LOG(INFO) << "-------cnt--------";
      int n = read(fd_, read_buffer_ + len, MAXLINE - len);
     // LOG(INFO) << "n: " << n;
    //  LOG(INFO) << "read_buffer_: " << read_buffer_;
      if (read_buffer_[0] == '*') {
        size = Finterpretersize();
      }
      std::vector<std::string> data = Finterpreter(&size, &cur_pos);
     // LOG(INFO) << "data.size(): " << data.size();
      for (int i = 0; i < data.size(); i++) {
        serialize_data.push_back(data[i]);
      }
      len = MAXLINE - cur_pos;
      char *new_arr = &read_buffer_[0];
    //  LOG(INFO) << "cur_pos: " << cur_pos;
    //  LOG(INFO) << "yuansu: " << read_buffer_[cur_pos];
    //  LOG(INFO) << "len: " << len;
      memmove(read_buffer_, read_buffer_ + cur_pos, sizeof(char) * len);
      memset(read_buffer_ + len, '\0', sizeof(read_buffer_) - len); // [0][1][2][3][4] -> [2][3][4][x][x]
     // LOG(INFO) << "read_buffer_ after memmove: " << read_buffer_;
    //  LOG(INFO) << "----------cnt_end-------------";
    }
  //  LOG(INFO) << "------------------------------------------------end-----------------------------------------";
    LOG(INFO) << "serialize_data: " << serialize_data[0];
    Encode::orderTolower(serialize_data[0]);
    std::string order = serialize_data[0];
    if (!order.compare("auth")) {
      auth_ = Command::AuthCommandImpl(serialize_data, &reply);
    } 
    else if (!order.compare("command") || auth_) {
      struct redisCommand rediscommand = Command::lookupCommand(serialize_data[0], serialize_data);
      void (*pd)(const std::vector<std::string>&, std::string* const) = rediscommand.pf;
      pd(serialize_data, &reply);
    } 
    else if (!auth_) {
      Command::AutherrorCommandImpl(serialize_data, &reply);
    } else {
      Command::ErrorCommandImpl(serialize_data, &reply);
    }
  } else {
    
  }
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
}

