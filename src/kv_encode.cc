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

bool Encode::Judgestring(const std::string& in, int& cur_pos) {
  if (in[cur_pos] == '*') {
    cur_pos++;
    return true;
  } else {
    return false;
  }
}

bool Encode::JudgeOrder(const std::string& in, int& cur_pos) { // 判断是不是$开头
  if (in[cur_pos] == '$') {
    cur_pos++;
    return true;
  } else {
    return false;
  }
}

bool Encode::paramtertotal(const std::string& in, int& cur_pos, int& size) { // 使字符串指令变成int类型的size 
  std::string word = "";
  while (in[cur_pos] != '\r') {
    if (in[cur_pos] >= '0' && in[cur_pos] <= '9') {
      word.push_back(in[cur_pos]);
      cur_pos++;
    } else {
      return false;
    }
  }
  cur_pos += 2;  // 跳过/r/n;
  size = atoi(word.c_str());
  return true;
}
 
void Encode::Split(const std::string& in, std::vector<std::string>* out, int &cur_pos, int& size) { // 使key, value, 指令装到vector里面
  std::string str = "";
  str.assign(in, cur_pos, size);
  std::cout << "str: " << str << std::endl;
  out->push_back(str);
  cur_pos = cur_pos + size + 2;
}

void Encode::orderTolower(std::string order) { // 指令小写化
  std::string::iterator it = order.begin();
  while (it != order.end()) {
    *it = tolower(*it);
    ++it;
  }
}

std::string Encode::getOrder(char *buf, int index, int maxsize) { // 获取字符串长度指令, key, value
  std::string order;
  for (int i = index; i < maxsize; i++) {
    if (buf[i] != '\r' && buf[i] != ' ' && buf[i] != '\n') {
      order.push_back(buf[i]);
    } else {
      break;
    }
  }
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

































