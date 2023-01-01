#include "csapp.h"
#include "kv_coon.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "storage_engine.h"
#include "kv_command.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace leveldb;

Coon* Coon::coon_ = nullptr;

Coon::Coon()
  : auth(false) {
}

Coon::~Coon() {

}

Coon* Coon::Init(int fd) {
  if (!coon_) {
   coon_ = new Coon();
   coon_->coon_fd = fd;
  }
  return coon_;
}

int Coon::Get_fd() {
  return coon_->coon_fd;
}

bool Coon::Get_auth() {
  return coon_->auth;
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

std::map<std::string, struct redisCommand> mp; /*= {
  {"set", redisCommandTable[0]},
  {"get", redisCommandTable[1]},
  {"delete", redisCommandTable[2]},
  {"flushall", redisCommandTable[3]},
  {"exit", redisCommandTable[4]},
  {"shutdown", redisCommandTable[5]},
  {"error", redisCommandTable[6]}
};*/

static void MapInit() {
  for (int i = 0; i < 7; i++) {
    char* name = redisCommandTable[i].name;
    mp[name] = redisCommandTable[i];
  }
}

std::vector<std::string> Coon::NormalFinterpreter(char *buf) {  // 解析正常数据
  std::vector<std::string> v;
  int index, cnt;
  std::string order, key, value;
  index = 0;
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

std::vector<std::string> Coon::Finterpreter(char *buf) { // 解析序列化后的数据
  int cnt, cur_pos, size, length;
  cur_pos = 0;
  std::vector<std::string> v;
  std::string in;
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

int Coon::GetRequest(char* buf) {
  MapInit();
  std::cout << "HE" << std::endl;
  std::vector<std::string> data;
  data = Coon::NormalFinterpreter(buf);
  Status s;
  std::string reply, order = data[0];
  struct redisCommand rediscommand;
  std::map<std::string, struct redisCommand>::iterator iter;
  iter = mp.find(order);
  std::cout << "order: " << order << std::endl;
  if (iter != mp.end()) {
    rediscommand = mp[order];
  } else {
    rediscommand = mp["error"];
  }
  void (*pd)(const std::vector<std::string>&, std::string* const) = rediscommand.pf;
  pd(data, &reply);
  strcpy(buf, reply.c_str());
  return reply.size();
}

void Coon::SendReply(const int& fd, const char line[], const int& size) {
  write(fd, line, size);
}

