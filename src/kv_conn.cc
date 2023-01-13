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

std::vector<std::string> Conn::Finterpreter() { // 解析序列化后的数据
  int cnt, cur_pos, size, length;
  cur_pos = 1;
  std::vector<std::string> serialize_data;
  std::string in;
  char* buf = read_buffer_;
  in = (std::string)buf;
  length = in.size();
  assert(Encode::paramtertotal(in, cur_pos, size));
  cnt = size;
  while (cnt--) {
    assert(Encode::JudgeOrder(in, cur_pos));
    assert(Encode::paramtertotal(in, cur_pos, size));
    Encode::Split(in, &serialize_data, cur_pos, size);
  }
  return serialize_data;
}

void Conn::GetRequest() {
  if (!FDException(fd_)) {
    int n = read(fd_, read_buffer_, MAXLINE);
    if (!IOException(n)) {
      std::vector<std::string> data;
      if (Encode::Judgestring(read_buffer_)) {  // 判断是序列化语句还是普通语句
        data = Finterpreter();
      } else {
        data = NormalFinterpreter();
      }
      struct redisCommand rediscommand = Command::lookupCommand(data[0]);
      void (*pd)(const std::vector<std::string>&, std::string* const) = rediscommand.pf;
      pd(data, &reply);
      strcpy(write_buffer_, reply.c_str());
      write_buffer_size = reply.size();
    } else {
    
    }
  }
  memset(read_buffer_, '\0', sizeof(read_buffer_));
}

void Conn::SendReply() {
  int32_t n = write(fd_, write_buffer_, write_buffer_size);
  memset(write_buffer_, '\0', sizeof(write_buffer_));
}

