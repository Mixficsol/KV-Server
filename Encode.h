#include <iostream>
#include <vector>

using namespace std;

class EncodeFix {
  public:

  int getCharLength(char *p) { // 获取从客户端发来的字符串长度
    int cnt;
    while (*p++ != '\0') {
      cnt++;
    }
    return cnt;
  }

  string getOrder(char *buf, int index, int maxsize) { // 获取字符串长度指令, key, value
    string order;
    for (int i = index; i < maxsize; i++) {
      if (buf[i] != '\n' && buf[i] != ' ') {
        order.push_back(buf[i]);
      } else {
        break;
      }
    }
    return order;
  }

  void orderTolower(string order) { // 指令小写化
    string::iterator it = order.begin();
    while (it != order.end()) {
      *it = tolower(*it);
      ++it;
    }
  }
  
  string getWord(string word) {
    word.append("\n");
    return word;
  }

  string sqlSplice(string sql, string word, int flag) {
    string ch = " ";
    sql.append(to_string(word.size())); // 拼接语句类似于3 set 3 123 3 456这种结构
    sql.append(ch);
    sql.append(word);
    if (flag) {
      sql.append(ch);
    }
    return sql;
  }

 /* string Extract(string templine, int position) {
    string word = ""; // 用来存字符串大小
    string str = ""; // 返回值
    int size;
    for (int i = position; ; i++) {
      if (templine[i] != ' ') {
        word.push_back(templine[i]);  // 存入数值大小的字符串
      } else {
        position = i + 1; // 标记当前位置
        size = atoi(word.c_str()); // 将字符串转为int表示大小
        str.assign(templine, position, size); // 在templine中从position开始最多size个字符往后读
        position = position + size + 1; // 变更position位置使其跳转到接下来一个参数的对应数值位的字符串开始
        break;
      }
    }
    return str;
    }*/

  private:
    
};














  
