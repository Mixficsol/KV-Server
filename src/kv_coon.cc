#include "csapp.h"
#include "kv_coon.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "kv_leveldb.h"
#include "storage_engine.h"

#include <string>
#include <iostream>
#include <vector>

using namespace leveldb;

std::vector<std::string>Coon::NormalFinterpreter(char *buf) {  // 解析正常数据
  IO_server a;
  EncodeFix b;
  std::vector<std::string> v;
  size_t n;
  bool flag;
  int index, cnt;
  std::string order, key, value, word, result;
  index = 0;
  cnt = 0;
  flag = false;
  cnt = b.getCharLength(buf);
  order = b.getOrder(buf, index, cnt);
  b.orderTolower(order);
  v.push_back(order);
  index = index + order.size() + 1;
  key = b.getOrder(buf, index, cnt);
  v.push_back(key);
  index = index + key.size() + 1;
  value = b.getOrder(buf, index, cnt);
  v.push_back(value);
  std::cout << "order: " << v[0] << std::endl;
  return v;
}

std::vector<std::string>Coon::Finterpreter(char *buf) { // 解析序列化后的数据
  int cnt, cur_pos, size, length;
  cur_pos = 0;
  std::vector<std::string> v;
  EncodeFix a;
  std::string in;
  in = (std::string)buf;
  length = in.size();
  assert(a.Judgestring(in, cur_pos));
  assert(a.paramtertotal(in , cur_pos, size));
  cnt = size;
  while (cnt--) {
    assert(a.FindNextSeparators(in, length, cur_pos));
    assert(a.JudgeOrder(in, cur_pos));
    assert(a.paramtertotal(in, cur_pos, size));
    assert(a.FindNextSeparators(in, length, cur_pos));
    a.Split(in, &v, cur_pos, size);
  }
  std::cout << v[0] << ' ' << v[1] << ' ' << v[2] << std::endl;
  return v;
}

int Coon::GetRequest(std::vector<std::string> data, char* buf) {
  EncodeFix b;
  LevelDB c;
  Status s;
  std::string word, result;
  std::string order = data[0];
  std::string key = data[1];
  std::string value = data[2];

  if (order.compare("set") == 0) {
    s = StorageEngine::GetCurrent()->Set(key, value);
    if (s.ok()) {
      word = b.getWord("insert successful!");
    } else {
      word = b.getWord("insert failed");
    }
  } else if (order.compare("get") == 0) {
    s = c.LevelDB_get(key, &result);
    if (s.ok()) {
      word = b.getWord(result);
    } else {
      word = b.getWord("not found");
    }
  } else if (order.compare("flushall") == 0) {
    result = c.Flushall();
  } else if (order.compare("exit") == 0) {
    word = b.getWord("exit");
  } else if (order.compare("shutdown") == 0) {
    word = b.getWord("shutdown");
  } else {
    word = b.getWord("Error");
  }
  strcpy(buf, word.c_str());
  return word.size();
}

void Coon::SendReply() {

}

