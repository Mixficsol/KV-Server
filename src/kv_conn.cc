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
  if (!FD_Exception(connfd)) {
    fd_ = connfd;
  } else {  
    exit(1);
  }
}

struct redisCommand{
  char* name;
  int parameter_num;
  void (*pf)(const std::vector<std::string>&, std::string* const); 
};

struct redisCommand redisCommandTable [7] = {
  {(char*)"set", 3, Command::SetCommandImpl},
  {(char*)"get", 2, Command::GetCommandImpl},
  {(char*)"delete", 1, Command::DeleteCommandImpl},
  {(char*)"flushall", 1, Command::FlushAllCommandImpl},
  {(char*)"exit", 1, Command::ExitCommandImpl},
  {(char*)"shutdown", 1, Command::ShutDownCommandImpl},
  {(char*)"error", 1, Command::ErrorCommandImpl}
};

std::map<std::string, struct redisCommand> mp;

int Conn::GetFD() {
  return fd_;
}

static void MapInit() {
  for (int i = 0; i < 7; i++) {
    char* name = redisCommandTable[i].name;
    mp[name] = redisCommandTable[i];
  }
}

std::vector<std::string> Conn::NormalFinterpreter() {  // 解析正常数据
  std::vector<std::string> v;
  int index, cnt;
  std::string order, key, value;
  index = 0;
  char* buf = read_buffer_;
  cnt = EncodeFix::getCharLength(buf);
  order = EncodeFix::getOrder(buf, index, cnt);
  EncodeFix::orderTolower(order);
  v.push_back(order);
  index = index + order.size() + 1;
  key = EncodeFix::getOrder(buf, index, cnt);
  v.push_back(key);
  index = index + key.size() + 1;
  value = EncodeFix::getOrder(buf, index, cnt);
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
  assert(EncodeFix::Judgestring(in, cur_pos));
  assert(EncodeFix::paramtertotal(in , cur_pos, size));
  cnt = size;
  while (cnt--) {
    assert(EncodeFix::FindNextSeparators(in, length, cur_pos));
    assert(EncodeFix::JudgeOrder(in, cur_pos));
    assert(EncodeFix::paramtertotal(in, cur_pos, size));
    assert(EncodeFix::FindNextSeparators(in, length, cur_pos));
    EncodeFix::Split(in, &v, cur_pos, size);
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
  if (!FD_Exception(fd_)) {
    int n = read(fd_, read_buffer_, MAXLINE);
    if (!IO_Exception(n)) {
      MapInit();
      std::cout << "buf: " << read_buffer_ << std::endl;
      std::cout << "buf.size(): " << sizeof(read_buffer_) << std::endl;
      std::vector<std::string> data;
      if (this->JudgeReadBufferCommand()) {  // 判断是序列化语句还是普通语句
        data = this->Finterpreter();
      } else {
        data = this->NormalFinterpreter();
      }
      std::cout << "data.size(): " << data.size() << std::endl;
      std::cout << "order: " << data[0] << std::endl;
      std::cout << "key: " << data[1] << std::endl;
      std::cout << "value: " << data[2] << std::endl;
      Status s;
      std::string reply, order = data[0];
      struct redisCommand rediscommand;
      std::map<std::string, struct redisCommand>::iterator iter;
      iter = mp.find(order);
      if (iter != mp.end()) {
        rediscommand = mp[order];
      } else {
        rediscommand = mp["error"];
      }
      void (*pd)(const std::vector<std::string>&, std::string* const) = rediscommand.pf;
      pd(data, &reply);
      std::cout << "reply: " << reply << std::endl;
      std::cout << "reply.size(): " << reply.size() << std::endl;
      strcpy(write_buffer_, reply.c_str());
      write_buffer_size = reply.size();
    } else {
      /*if (errno == ECONNRESET) {
        close(fd);
        Cluster_Epoll::Set_Miss(fd);
      } else {
        std::cout << "redaline error" << std::endl;
        // LOG(INFO) << "readline error";
      }*/
    }
  }
}

void Conn::SendReply() {
  write(fd_, write_buffer_, write_buffer_size);
}

