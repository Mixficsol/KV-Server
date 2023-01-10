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

void Conn::ProcessNewConn(const int& listenfd) {
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

typedef struct redisCommand {
  char* name;
  int parameter_num;
  void (*pf)(const std::vector<std::string>&, std::string* const); 
} redisCommand;

struct redisCommand redisCommandTable [8] = {
  {(char*)"set", 3, Command::SetCommandImpl},
  {(char*)"get", 2, Command::GetCommandImpl},
  {(char*)"delete", 1, Command::DeleteCommandImpl},
  {(char*)"flushall", 1, Command::FlushAllCommandImpl},
  {(char*)"exit", 1, Command::ExitCommandImpl},
  {(char*)"shutdown", 1, Command::ShutDownCommandImpl},
  {(char*)"error", 1, Command::ErrorCommandImpl},
  {(char*)"COMMAND", 1, Command::FirstCommandImpl}
};

std::map<std::string, struct redisCommand> command_map;

int Conn::GetFD() {
  return fd_;
}

std::vector<std::string> Conn::NormalFinterpreter() {  // 解析正常数据
  std::vector<std::string> v;
  int index, cnt;
  std::string order, key, value;
  index = 0;
  char* buf = read_buffer_;
  cnt = Encode::getCharLength(buf);
  order = Encode::getOrder(buf, index, cnt);
  Encode::orderTolower(order);
  v.push_back(order);
  index = index + order.size() + 1;
  key = Encode::getOrder(buf, index, cnt);
  v.push_back(key);
  index = index + key.size() + 1;
  value = Encode::getOrder(buf, index, cnt);
  v.push_back(value);
  return v;
}

std::vector<std::string> Conn::Finterpreter() { // 解析序列化后的数据
  int cnt, cur_pos, size, length;
  cur_pos = 0;
  std::vector<std::string> v;
  std::string in;
  char* buf = read_buffer_;
  in = (std::string)buf;
  length = in.size();
  assert(Encode::Judgestring(in, cur_pos));
  assert(Encode::paramtertotal(in, cur_pos, size));
  cnt = size;
  while (cnt--) {
    assert(Encode::JudgeOrder(in, cur_pos));
    assert(Encode::paramtertotal(in, cur_pos, size));
    Encode::Split(in, &v, cur_pos, size);
  }
  return v;
}

bool Conn::JudgeReadBufferCommand() {
  if (read_buffer_[0] == '*') {
    return true;
  } else {
    return false;
  }
}

void Conn::GetRequest() {
  if (!FDException(fd_)) {
    int n = read(fd_, read_buffer_, MAXLINE);
    if (!IOException(n)) {
      std::vector<std::string> data;
      if (this->JudgeReadBufferCommand()) {  // 判断是序列化语句还是普通语句
        data = this->Finterpreter();
      } else {
        data = this->NormalFinterpreter();
      }
      std::string reply, order = data[0];
      struct redisCommand rediscommand = Command::lookupCommand(order);
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
