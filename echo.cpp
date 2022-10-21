#include "csapp.h"
#include "hiredis/hiredis.h"
#include <map>
#include <string>
#include <iostream>
#include <mutex>

using namespace std;

mutex my_mutex;
map<string, string> mp;

void echo(int connfd) {
  char buf[MAXLINE];
  size_t n;
  string order, key, value, word; // 创建指令, key, value,返回字符
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
      cnt++;
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
      mp[key] = value;
    }
    else if (order.compare("get") == 0) {
      map<string, string>::iterator iter = mp.find(key);
      if (iter != mp.end()) {
        word = iter->second;
        word.push_back('\n');
      } else {
        word = "Not find\n";
      }
    } else {
      word = "Error\n";
    }
    n = word.size();
    strcpy(buf, word.c_str());
    Rio_writen(connfd, buf, word.size());
  }
}



















