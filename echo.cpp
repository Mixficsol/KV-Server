#include "csapp.h"
#include "IO-server.h"
#include <string>
#include <iostream>
#include "leveldb/db.h"

using namespace std;

void echo(int connfd) {
  char buf[MAXLINE];
  size_t n;
  int index, cnt;
  string order, key, value, word, result; // 创建指令, key, value, 返回语句
  rio_t rio;
  leveldb::DB *db;
  leveldb::Options options;
  string dbpath = "./leveldb.db";
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);
  Rio_readinitb(&rio, connfd); // 初始化描述符
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    index = 0;
    cnt = 0;
    IO_server a; 
    EncodeFix b;
    cnt = b.getCharLength(buf); // 获取从客户端传来的字符串长度
    order = b.getOrder(buf, index, cnt); // 获取order
    index = index + order.size() + 1;    // 更新索引
    key = b.getOrder(buf, index, cnt);  // 获取key
    index = index + key.size() + 1;
    value = b.getOrder(buf, index, cnt); // 获取value
    b.orderTolower(order); // 将指令小写化
    cout << "order: " << order << endl;
   /* if (status.ok() == false) {
      word = b.getWord("leveldb open failed");
    }*/
    if (order.compare("set") == 0) {   // 判断指令
      status = db->Put(leveldb::WriteOptions(), key, value); // 使用lecvel数据库
      if (status.ok()) {
        word = b.getWord("insert successful");
      } else {
        word = b.getWord("leveldb write faile");
      }
   // a.set(order, key, value); 文件实现set指令
   // word = b.getWord("OK");
    }
    else if (order.compare("get") == 0) {
      status = db->Get(leveldb::ReadOptions(), key, &result);
      if (status.ok()) {
        word = b.getWord(result);
      } else {
        word = b.getWord("not found");
      }
      // word = a.get(key); // 调用get接口(文件实现)
    }
    else if (order.compare("flushall") == 0) {
      leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
      for (it->SeekToFirst(); it->Valid(); it->Next()) {
        key = it->key().Tostring();
        status = db->Delete(leveldb::WriteOptions(), key);
        cout << key << ": " << it->value().ToString() << endl;
      }
     // word = a.flushall(); // 文件清空数据库
    }
    else if (order.compare("exit") == 0) {
      delete db;
      return 0;
    }
    else {
      word = b.getWord("Error");
    }
    delete db;
    strcpy(buf, word.c_str());               // 将string类型转为char*类型
    Rio_writen(connfd, buf, word.size());    // 返回数据给服务器
  }
}



















