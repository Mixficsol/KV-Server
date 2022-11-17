#include <csapp.h>
#include <IO-server.h>
#include <string>
#include <iostream>
#include <leveldb/db.h>
using namespace std;

leveldb::DB *db;
leveldb::Options options;
string dbpath = "leveldb.db";

bool echo(char* buf) {
  size_t n;
  bool flag;
  int index, cnt;
  string order, key, value, word, result; // 创建指令, key, value, 返回语句
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, dbpath, &db); // 打开leveldb数据库
  index = 0;
  cnt = 0;
  flag = false;
  IO_server a; 
  EncodeFix b;
  cnt = b.getCharLength(buf); // 获取从客户端传来的字符串长度
  order = b.getOrder(buf, index, cnt); // 获取order
  index = index + order.size() + 1;    // 更新索引
  key = b.getOrder(buf, index, cnt);  // 获取key
  index = index + key.size() + 1;  // 更新索引
  value = b.getOrder(buf, index, cnt); // 获取value
  b.orderTolower(order); // 将指令小写化
  cout << "order: " << order << endl;
  if (status.ok() == false) {  // 判断数据库是否打开成功
    word = b.getWord("leveldb open failed");
  }
  if (order.compare("set") == 0) {   // 判断指令，执行set
    status = db->Put(leveldb::WriteOptions(), key, value); // 使用leveldb数据库的set方法
    if (status.ok()) {
      word = b.getWord("insert successful");
    } else {
      word = b.getWord("leveldb write faile");
    }
    // a.set(order, key, value); //文件实现set指令
  }
  else if (order.compare("get") == 0) { // 判断指令，执行get
    status = db->Get(leveldb::ReadOptions(), key, &result); // 使用leveldb数据库的get方法
    if (status.ok()) {
      word = b.getWord(result);
    } else {
      word = b.getWord("not found");
    }
    // word = a.get(key); // 调用get接口(文件实现)
  }
  else if (order.compare("flushall") == 0) { // 删除数据库
    a.Flushall();
    word = b.getWord("clear");
    // word = a.flushall(); // 文件清空数据库
  }
  else if (order.compare("exit") == 0) { // 退出服务器
    flag = true;
  }
  else {  // 其他指令识别成Error
    word = b.getWord("Error");
  }
  delete db; // 关闭数据库
  strcpy(buf, word.c_str()); // 将string类型转为char*类型
  buf[word.size()] = '\0';
  return flag;
}



















