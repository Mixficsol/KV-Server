#include "csapp.h"
#include "IO-server.h"
#include <string>
#include <fstream>
#include <iostream>
#include <mutex>

using namespace std;

mutex my_mutex;

void echo(int connfd) {
  char buf[MAXLINE];
  size_t n;
  string order, key, value, word, sql; // 创建指令, key, value, 返回语句, 存入文件语句
  rio_t rio;
  Rio_readinitb(&rio, connfd); // 初始化描述符
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    char *p = buf;
    int i, j, cnt = 0;
    order.clear();
    key.clear();
    value.clear(); //初始化字符串
    word.clear();
    while (*p++ != '\0') {
      cnt++;  // 判断字符串长度
    }
    for (i = 0; i < 3; i++) {
      order.push_back(buf[i]); // 指令赋值
    }
    for (i = 4; i < cnt; i++) {
      if (buf[i] != ' ' && buf[i] != '\n') {
        key.push_back(buf[i]);    // key赋值
      } else {
        break;
      }
    }
    for (j = i + 1; j < cnt; j++) {
      if (buf[j] != ' ' && buf[j] != '\n') {
        value.push_back(buf[j]); // value赋值
      } else {
        break;
      }
    }
    string::iterator it = order.begin();
    while (it != order.end()) {
      *it = tolower(*it);   // 将指令全部小写化
      ++it;
    }
    if (order.compare("set") == 0) {   // 判断指令
      word = "OK\n";
      IO_server a;   // 调用set接口
      a.set(order, key, value);
    }
    else if (order.compare("get") == 0) {
      IO_server a;
      word = a.get(order, key);  // 调用get接口
      word.append("\n");
    } else {
      word = "Error\n";
    }
    n = word.size();
    strcpy(buf, word.c_str());
    Rio_writen(connfd, buf, word.size());
  }
}



















