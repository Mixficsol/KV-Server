#include "../include/kv_encode.h"
#include <string>

int EncodeFix::getCharLength(char *p) { // 获取客户端传来的字符串长度
  int cnt;
  while (*p++ != '\0') {
    cnt++;
  }
  return cnt;
}

int EncodeFix::KOOK(char *p) {  // 判断关机指令
  char *q = (char*)"shutdown";
  int flag = 1;
  for (int i = 0; i < 8; i++) {
    if (p[i] != q[i]) {
      flag = 0;
      break;
    }
  }
  return flag;
}

std::string EncodeFix::getOrder(char *buf, int index, int maxsize) { // 获取字符串长度指令, key, value
  std::string order;
  for (int i = index; i < maxsize; i++) {
    if (buf[i] != '\n' && buf[i] != ' ') {
      order.push_back(buf[i]);
    } else {
      break;
    }
  }
  return order;
}

void EncodeFix::orderTolower(std::string order) { // 指令小写化
  std::string::iterator it = order.begin();
  while (it != order.end()) {
    *it = tolower(*it);
    ++it;
  }
}

std::string EncodeFix::getWord(std::string word) { // 将字符串末尾加上换行
  word.append("\n");
  return word;
}

std::string EncodeFix::sqlSplice(std::string sql, std::string word, int flag) { // // 拼接字符串
  std::string ch = " ";
  sql.append(std::to_string(word.size()));
  sql.append(ch);  //  拼接语句类似于3 set 3 123 3 456这种结构
  sql.append(word);
  if (flag) {
    sql.append(ch);
  }
  return sql;
}

































