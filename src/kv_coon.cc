#include "csapp.h"
#include "kv_coon.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "storage_engine.h"

#include <string>
#include <iostream>
#include <vector>

using namespace leveldb;

std::vector<std::string> Coon::NormalFinterpreter(char *buf) {  // 解析正常数据
  std::vector<std::string> v;
  int index, cnt;
  std::string order, key, value;
  index = 0;
  cnt = EncodeFix::getCharLength(buf);
  order = EncodeFix::getOrder(buf, index, cnt);
  EncodeFix::GetCurrent()->orderTolower(order);
  v.push_back(order);
  index = index + order.size() + 1;
  key = EncodeFix::GetCurrent()->getOrder(buf, index, cnt);
  v.push_back(key);
  index = index + key.size() + 1;
  value = EncodeFix::GetCurrent()->getOrder(buf, index, cnt);
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
  assert(EncodeFix::GetCurrent()->Judgestring(in, cur_pos));
  assert(EncodeFix::GetCurrent()->paramtertotal(in , cur_pos, size));
  cnt = size;
  while (cnt--) {
    assert(EncodeFix::GetCurrent()->FindNextSeparators(in, length, cur_pos));
    assert(EncodeFix::GetCurrent()->JudgeOrder(in, cur_pos));
    assert(EncodeFix::GetCurrent()->paramtertotal(in, cur_pos, size));
    assert(EncodeFix::GetCurrent()->FindNextSeparators(in, length, cur_pos));
    EncodeFix::GetCurrent()->Split(in, &v, cur_pos, size);
  }
  return v;
}

int Coon::GetRequest(std::vector<std::string> data, char* buf) {
  Status s;
  std::string word, result;
  std::string order = data[0];
  std::string key = data[1];
  if (order.compare("set") == 0) {
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
  }
  strcpy(buf, word.c_str());
  return word.size();
}

void Coon::SendReply() {

}

