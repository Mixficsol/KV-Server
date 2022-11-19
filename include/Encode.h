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
  
  int KOOK(char *p) { // 判断指令
    char *q = "shutdown";
    int flag = 1;
    for (int i = 0; i < 8; i++) {
      if (p[i] != q[i]) {
        flag = 0;
        break;
      }
    }
    return flag;
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
  
  string getWord(string word) { // 将字符串末尾加上换行
    word.append("\n");
    return word;
  }

  string sqlSplice(string sql, string word, int flag) { // 拼接字符串
    string ch = " ";
    sql.append(to_string(word.size())); // 拼接语句类似于3 set 3 123 3 456这种结构
    sql.append(ch);
    sql.append(word);
    if (flag) {
      sql.append(ch);
    }
    return sql;
  }

  private:
    
};














  
