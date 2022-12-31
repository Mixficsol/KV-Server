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

struct redisCommand{
  char* name;
  int parameter_num;
  void (*pf)(const std::vector<std::string>&, std::string* const); 
};

struct redisCommand redisCommandTable[] = {
  { (char*)"set", 3, Command::SetCommandImpl}
 /* {"get", 2, Command::GetCommandImpl},
  {"delete", 1, Command::DeleteCommandImpl},
  {"flushall", 1, Command::FlushAllCommandImpl},*/
};

std::map<std::string, struct redisCommand> mp = {
  {"set", redisCommandTable[0]}
 /* {"get", redisCommandTable[1]},
  {"delete", redisCommandTable[2]},
  {"flushall", redisCommandTalbe[3]},*/
};

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

int Coon::GetRequest(const std::vector<std::string>& data, char* buf) {
  Status s;
  std::string word, result, reply;
  std::string order = data[0];
 // mp["set"] = redisCommandTable[0];
  struct redisCommand rediscommand = mp[order];
  void (*pd)(const std::vector<std::string>&, std::string* const) = rediscommand.pf;
  pd(data, &reply);
  //Coommand::SetCommandImpl(data, &reply);
 // pd(data, &reply);
 /* if (order.compare("set") == 0) {
    SetCommandImpl();
    std::string value = data[2];
    s = StorageEngine::GetCurrent()->Set(key, value);
    if (s.ok()) {
      word = EncodeFix::getWord("insert successful!");
    } else {
      word = EncodeFix::getWord("insert failed");
    }
  } else if (order.compare("get") == 0) {
    s = StorageEngine::GetCurrent()->Get(key, &result);
    if (s.ok()) {
      word = EncodeFix::getWord(result);
    } else {
      word = EncodeFix::getWord("not found");
    }
  } else if (order.compare("flushall") == 0) {
    result = StorageEngine::GetCurrent()->FlushAll();
  } else if (order.compare("exit") == 0) {
    word = EncodeFix::getWord("exit");
  } else if (order.compare("shutdown") == 0) {
    word = EncodeFix::getWord("shutdown");
  } else {
    word = EncodeFix::getWord("Error");
  }*/
  strcpy(buf, reply.c_str());
  return reply.size();
}

void Coon::SendReply() {

}

