#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <stdio.h>

using namespace std;

int cur_pos;
int size, length;

bool FindNextSeparators(const string& in) {  // 找到下一个'\n'的位置
  if (cur_pos > length - 1) {
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
  return false;
} 


void Split(const string& in, std::vector<std::string>* out) { // 使key, value, 指令装到vector里面
  string str = "";
  str.assign(in, cur_pos, size);
  out->push_back(str);
  cur_pos = cur_pos + size;
}

bool Judgestring(const string& in) {  // 判断是不是以*开头的序列化语句
  if (in[cur_pos] == '*') {
    cur_pos++;
    return true;
  } else {
    return false;
  }
}

bool JudgeOrder(const string& in) { // 判断是不是以$开头的参数语句
  if (in[cur_pos] == '$') {
    cur_pos++;
    return true;
  } else {
    return false;
  }
}

bool paramtertotal(const string& in) { // 使字符串指令变成int类型的size
  string word = "";
  string str = "";
  int pos = cur_pos;
  while (in[pos] != '\\') {
    if (in[pos] >= 48 && in[pos] <= 57) { // 判断参数里面有没有掺杂字母比如3f4这种就不合理
      word.push_back(in[pos]);
      pos++;
    } else {
      cout << "false" << endl;
      return false;  
    }
  }
  size = atoi(word.c_str()); // 字符串转为int类型
  cur_pos = pos; // 更新位置
  return true;
}

int main() {
  vector<string> v;
  char str[1010] = {};
  string in;
  int cnt;
  cur_pos = 0;
  scanf("%s", &str);
  cout << str << endl;
  in = (string)str;
  length = in.size();
  assert(Judgestring(in));
  assert(paramtertotal(in));
  cnt = size;
  while (cnt--) {
    assert(FindNextSeparators(in));
    assert(JudgeOrder(in));
    assert(paramtertotal(in));
    assert(FindNextSeparators(in));
    Split(in, &v);
  }
  cout << v[0] << ' ' << v[1] << ' ' << v[2] << endl;
  return 0;
}



