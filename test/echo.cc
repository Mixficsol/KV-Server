#include "../include/kv_io.h"
#include "../include/kv_encode.h"
#include "../include/kv_leveldb.h"
#include "../include/csapp.h"
#include <string>
#include <iostream>

int echo(char* buf) {
  IO_server a;
  EncodeFix b;
  LevelDB c;
  size_t n;
  bool flag;
  int index, cnt;
  std::string order, key, value, word, result; // 创建指令, key, value, 返回语句
  leveldb::Status status;
  index = 0;
  cnt = 0;
  flag = false;
  cnt = b.getCharLength(buf); // 获取从客户端传来的字符串长度
  order = b.getOrder(buf, index, cnt); // 获取order
  index = index + order.size() + 1;    // 更新索引
  key = b.getOrder(buf, index, cnt);  // 获取key
  index = index + key.size() + 1;  // 更新索引
  value = b.getOrder(buf, index, cnt); // 获取value
  b.orderTolower(order); // 将指令小写化
  std::cout << "order: " << order << std::endl;
  if (order.compare("set") == 0) {   // 判断指令，执行set
    if (c.LevelDB_set(key, value)) {
      word = b.getWord("insert successful");
    } else {
      word = b.getWord("insert failed");
    }
  } else if (order.compare("get") == 0) { // 判断指令，执行get
    status = c.LevelDB_get(key, &result);
    if (status.ok()) {
      word = b.getWord(result);
    } else {
      word = b.getWord("not found");
    }
  } else if (order.compare("flushall") == 0) { // 删除数据库
    result = c.Flushall();
  } else if (order.compare("exit") == 0) { // 退出服务器
    word = b.getWord("exit");
  } else if (order.compare("shutdown") == 0) {
    word = b.getWord("shutdown");
  } else {  // 其他指令识别成Error
    word = b.getWord("Error");
  }
  strcpy(buf, word.c_str()); // 将string类型转为char*类型
  return word.size();
}



















