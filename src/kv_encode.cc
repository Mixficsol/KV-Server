#include "kv_encode.h"

#include <string>
#include <assert.h>

#include <glog/logging.h>

EncodeFix::EncodeFix() {

}

EncodeFix::~EncodeFix() {

}

int EncodeFix::getCharLength(char *p) { // 获取客户端传来的字符串长度
  int cnt;
  while (*p++ != '\0') {
    cnt++;
  }
  return cnt;
}

int EncodeFix::JudgeShutdown(char *p) {  // 判断关机指令
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

bool EncodeFix::Judgestring(const std::string& in, int& cur_pos) {
  if (in[cur_pos] == '*') {
    cur_pos++;
    return true;
  } else {
    return false;
  }
}

bool EncodeFix::JudgeOrder(const std::string& in, int& cur_pos) { // 判断是不是$开头
  if (in[cur_pos] == '$') {
    cur_pos++;
    return true;
  } else {
    return false;
  }
}

bool EncodeFix::paramtertotal(const std::string& in, int& cur_pos, int& size) { // 使字符串指令变成int类型的size 
  std::string word = "";
  std::string str = "";
  int pos = cur_pos;
  while (in[pos] != '\\') {
    if (in[pos] >= '0' && in[pos] <= '9') {
      word.push_back(in[pos]);
      pos++;
    } else {
      return false;
    }
  }
  size = atoi(word.c_str());
  cur_pos = pos;
  return true;
}
 
bool EncodeFix::FindNextSeparators(const std::string& in, int& length, int& cur_pos) { // 找到下一个'\n'的位置
  if (cur_pos > length - 1) {
    return false;
  }
  int pos = cur_pos;
  while (pos <= length - 1) {
    if (in[pos] == 'n') {
      cur_pos = pos + 1; // 找到'\n'的索引并且返回, 使位置往后移一位
      return true;
    }
    pos++;
  }
  return false;
}

void EncodeFix::Split(const std::string& in, std::vector<std::string>* out, int &cur_pos, int& size) { // 使key, value, 指令装到vector里面
  std::string str = "";
  str.assign(in, cur_pos, size);
  out->push_back(str);
  cur_pos = cur_pos + size;
}

void EncodeFix::orderTolower(std::string order) { // 指令小写化
  std::string::iterator it = order.begin();
  while (it != order.end()) {
    *it = tolower(*it);
    ++it;
  }
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

































