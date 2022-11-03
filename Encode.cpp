#include <iostream>
#include <vector>
#include <string>
#include <assert.h>

using namespace std;

string a = "*3\r\n$3\r\nset\r\n$3\r\nkey\r\n$5\r\nvalue\r\n";
int length = a.size();
int cur_pos;

int FindNextSeparators() {
  if (cur_pos >= length - 1) {
    return -1;
  }
  int pos = cur_pos;
  while (pos < length - 1) {
    if (a[pos] == '\n') {
      return pos;
    }
    pos++;
  }
  return -1;
} 

int main() {
  vector<string> v;
  int pos = cur_pos = 0;
  int cnt = 3;
  cout << "length: " << length << endl;
  while (cnt--) {
    pos = FindNextSeparators();
    cout << "pos: " << pos << endl;
    string word = "";
    string str = "";
    pos = pos + 1; // 使位置移到$处
    if (a[pos] == '$') {
      pos++;
      while (a[pos] != '\r') {
        word.push_back(a[pos]);
        pos++;
      }
      cout << "word: " << word << endl;
      int size = atoi(word.c_str());
      cout << "size: " << size << endl;
      str.assign(a, pos + 2, size);
      cur_pos = pos + 2;
      cout << "str: " << str << endl;
      v.push_back(str);
    }
  }
  cout << v[0] << ' ' << v[1] << ' ' << v[2]  << endl;
  
  return 0;
}



