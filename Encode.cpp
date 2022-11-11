#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <stdio.h>
#include "glog/logging.h"

using namespace std;

bool FindNextSeparators(const string& in, int& length, int& cur_pos) {  // 找到下一个'\n'的位置
  if (cur_pos > length - 1) {
    LOG(ERROR) << "字符串格式错误";
    return false;
  }
  int pos = cur_pos;
  while (pos <= length - 1) {
    if (in[pos] == 'n') {
      cur_pos = pos + 1;  // 找到'\n'的索引并且返回, 使位置往后移一位
      return true; 
    }
    pos++;
  }
  LOG(ERROR) << "字符串格式错误";
  return false;
} 


void Split(const string& in, std::vector<std::string>* out, int& cur_pos, int& size) { // 使key, value, 指令装到vector里面
  string str = "";
  str.assign(in, cur_pos, size);
  out->push_back(str);
  cur_pos = cur_pos + size;
}

bool Judgestring(const string& in, int& cur_pos) {  // 判断是不是以*开头的序列化语句
  if (in[cur_pos] == '*') {
    cur_pos++;
    return true;
  } else {
    LOG(ERROR) << "字符串格式错误";
    return false;
  }
}

bool JudgeOrder(const string& in, int& cur_pos) { // 判断是不是以$开头的参数语句
  if (in[cur_pos] == '$') {
    cur_pos++;
    return true;
  } else {
    LOG(ERROR) << "字符串格式错误";
    return false;
  }
}

bool paramtertotal(const string& in, int& cur_pos, int& size) { // 使字符串指令变成int类型的size
  string word = "";
  string str = "";
  int pos = cur_pos;
  while (in[pos] != '\\') {
    if (in[pos] >= '0' && in[pos] <= '9') { // 判断参数里面有没有掺杂字母比如3f4这种就不合理
      word.push_back(in[pos]);
      pos++;
    } else {
      LOG(ERROR) << "字符串格式错误";
      return false;  
    }
  }
  size = atoi(word.c_str()); // 字符串转为int类型
  cur_pos = pos; // 更新位置
  return true;
}

int main(int argc, char*argv[]) {
  vector<string> v;
  char str[1010] = {};
  string in;
  google::InitGoogleLogging(argv[0]);
  FLAGS_log_dir = "./log";
  int cnt, cur_pos, size, length;
  cur_pos = 0;
  scanf("%s", &str);
  cout << str << endl;
  in = (string)str;
  length = in.size();
  assert(Judgestring(in, cur_pos));
  assert(paramtertotal(in, cur_pos, size));
  cnt = size;
  while (cnt--) {
    assert(FindNextSeparators(in, length, cur_pos));
    assert(JudgeOrder(in, cur_pos));
    assert(paramtertotal(in, cur_pos, size));
    assert(FindNextSeparators(in, length, cur_pos));
    Split(in, &v, cur_pos, size);
  }
  cout << v[0] << ' ' << v[1] << ' ' << v[2] << endl;
  google::ShutdownGoogleLogging();
  return 0;
}



