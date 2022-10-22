#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {
  int maxpos, size, position;
  bool flag;
  char x;
  string Key;
  string order, key, value, word = "";
  vector<string> lines;
  string line, oneline;
  cin >> Key;
  ifstream f("test.txt", ios::ate);
  if (!f) {
    cout << "打开文件失败" << endl;
  }
  flag = false;
  maxpos = f.tellg();
  f.seekg(ios::end);
  cout << "maxpos: " << maxpos << endl;
  for (int index = 2; index <= maxpos; index++) {
    f.seekg(-index, ios::end);
    f.get(x);
    if (x == 0) {
      continue;
    }
    if (x == '\n') {
      string templine = line;
      if (templine.size() > 0) {
        lines.push_back(templine);
        word = templine[0];
        position = 1;
        size = atoi(word.c_str());
        order.assign(templine, position + 1, size) ; // 从索引开始最多n个字符 
        cout << "order: " << order << endl;
        position = position + size + 1;
        word.clear();
        for (int i = position + 1; ; i++) {
          if (templine[i] != ' ') {
            word.push_back(templine[i]); 
          } else {
            position = i;
            size = atoi(word.c_str());
            key.assign(templine, position + 1, size);
            cout << "key: " << key << endl;
            cout << "key.size(): " << key.size() << endl;
            position = position + size + 1;
            word.clear();
            break;
          }
        }
        for (int i = position + 1; i < templine.size(); i++) {
          if (templine[i] != ' ') {
            word.push_back(templine[i]);
          } else {
            position = i;
            size = atoi(word.c_str());
            value.assign(templine, position + 1, size);
            cout << "value: " << value << endl;
            cout << "value.size(): " << value.size() << endl;
          }
        }
        if (key.compare(Key) == 0 && !value.empty()) {
          flag = true;
          break;
        }
      } 
      line = "";
      word = "";
      key = "";
      value = "";
      order = "";
    } else {
      line = x + line;
    }
    x = 0;
  }
  
  if (flag) {
    cout << "value: " << value << endl;
  } else {
    cout << "not found" << endl;
  }
  return 0;
}
