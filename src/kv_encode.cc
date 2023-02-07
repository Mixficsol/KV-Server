#include "kv_encode.h"

#include <string>
#include <assert.h>
#include <iostream>

#include <glog/logging.h>

int Encode::getCharLength(const char *p) { // 获取客户端传来的字符串长度
  int cnt;
  while (*p++ != '\0') {
    cnt++;
  }
  return cnt;
}

bool Encode::Judgestring(const char in[]) {
  if (in[0] == '*') {
    return true;
  } else {
    return false;
  }
}

void Encode::Find$ (const std::string& in, int& cur_pos) {
  while (in[cur_pos] != '$') {
    cur_pos++;
  }
}

void Encode::Back$ (const std::string& in, int& cur_pos) {
  while (in[cur_pos] != '$') {
    cur_pos--;
  }
}

bool Encode::JudgeOrder(const std::string& in, int& cur_pos) { // 判断是不是$开头
  if (cur_pos < in.size() && in[cur_pos] == '$') {
    cur_pos++;
    return true;
  } else {
    return false;
  }
}

bool Encode::paramtertotal(const std::string& in, int& cur_pos, int& size) { // 使字符串指令变成int类型的size 
  std::string word = "";
  if (cur_pos >= in.size()) { // 表示读到$这里刚好读满
    Back$(in, cur_pos);
    return false;
  }

  while (in[cur_pos] != '\r' && cur_pos < in.size()) {
    if (in[cur_pos] >= '0' && in[cur_pos] <= '9') {
      word.push_back(in[cur_pos]);
      cur_pos++;
    } else {
      return false;
    }
  }
  if (cur_pos >= in.size()) { // 表示读数字部分时中间段读满了,ex: xxxx$123|456在3这里读满了
    Back$(in, cur_pos);
  }
  if (cur_pos < in.size() && in[cur_pos] == '\r' && cur_pos + 1 < in.size() && in[cur_pos + 1] == '\n') {
    size = atoi(word.c_str());
    cur_pos += 2;
    return true;
  } else {
    Back$(in, cur_pos); // 在数字部分刚好读满以及'\r'或'\n'这里中断读满的情况
    return false;
  } 
}
 
bool Encode::Split(const std::string& in, std::vector<std::string>* out, int &cur_pos, int& size) { // 使key, value, 指令装到vector里面
  std::string str = "";
  if (cur_pos >= in.size()) { // 在刚开始读数据部分刚好读满
    Back$(in, cur_pos);
  }
  str.assign(in, cur_pos, size);
  if (cur_pos + size - 1 < in.size() && cur_pos + size < in.size() && in[cur_pos + size] == '\r' && cur_pos + size + 1 < in.size() && in[cur_pos + size + 1] == '\n') {
    out->push_back(str);
    cur_pos = cur_pos + size + 2;
    return true;
  } else {
    Back$(in, cur_pos); // 在读数据中途读满，ex:$12\r\nqwer|asddxx在r这里读满
    return false;
  }
}

void Encode::orderTolower(std::string& order) { // 指令小写化
  std::string::iterator it = order.begin();
  while (it != order.end()) {
    *it = tolower(*it);
    ++it;
  }
}

std::string Encode::getOrder(char *buf, int& index, int maxsize) { // 获取字符串长度指令, key, value
  std::string order;
  for (int i = index; i < maxsize; i++) {
    if (buf[i] != '\r' && buf[i] != ' ' && buf[i] != '\n') {
      order.push_back(buf[i]);
    } else {
      break;
    }
  }
  index = index + order.size() + 1;
  return order;
}

std::string Encode::getWord(std::string word) { // 将字符串末尾加上换行
  word.append("\n");
  return word;
}

std::string Encode::sqlSplice(std::string sql, std::string word, int flag) { // // 拼接字符串
  std::string ch = " ";
  sql.append(std::to_string(word.size()));
  sql.append(ch);  //  拼接语句类似于3 set 3 123 3 456这种结构
  sql.append(word);
  if (flag) {
    sql.append(ch);
  }
  return sql;
}

































