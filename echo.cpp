#include "csapp.h"
#include "IO-server.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

void echo(int connfd) {
  char buf[MAXLINE];
  size_t n;
  int index, cnt;
  string order, key, value, word; // 创建指令, key, value, 返回语句
  rio_t rio;
  Rio_readinitb(&rio, connfd); // 初始化描述符
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    index = 0;
    cnt = 0;
    IO_server a; 
    cnt = a.getCharLength(buf); // 获取从客户端传来的字符串长度
    order = a.getOrder(buf, index, cnt); // 获取order
    index = index + order.size() + 1;    // 更新索引
    key = a.getOrder(buf, index, cnt);  // 获取key
    index = index + key.size() + 1;
    value = a.getOrder(buf, index, cnt); // 获取value
    a.orderTolower(order); // 将指令小写化
    if (order.compare("set") == 0) {   // 判断指令
      word = a.getWord("OK");
      a.set(order, key, value);
    }
    else if (order.compare("get") == 0) {
      word = a.get(key);  // 调用get接口
    } else {
      word = a.getWord("Error");
    }
    n = word.size();
    strcpy(buf, word.c_str());               // 将string类型转为char*类型
    Rio_writen(connfd, buf, word.size());    // 返回数据给服务器
  }
}



















