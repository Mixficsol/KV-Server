#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class IO_server {
  public:  // 公共部分函数   
  void set(string order, string key, string value) {  // set函数参数指令,key,value
    ofs.open("test.txt", ios::app); // 供写，文件不存在则创建，若文件已存在则在原文件内容后写入新的内容，指针位置总在最后
    string ch;
    ch = " ";
    sql.append(to_string(order.size()));  // 拼接语句类似于3 set 3 123 3 456这种结构
    sql.append(ch);
    sql.append(order);
   sql.append(ch);
    sql.append(to_string(key.size()));
    sql.append(ch);
    sql.append(key);
    sql.append(ch);
    sql.append(to_string(value.size()));
    sql.append(ch);
    sql.append(value);
    ofs << sql << endl;  // 写入文件
    sql.clear(); // 语句初始化便于下次拼接
    ofs.close(); // 关闭文件
  } 

  string get(string order, string Key) { // get函数参数指令，key
    ifstream f("test.txt", ios::ate); // 文件打开时，指针在文件最后
    if (!f) {
       cout << "打开文件失败" << endl;
    }
    flag = false;  // 用来判断是否找到对应的key，如果找到则为true,初始值是false
    maxpos = f.tellg(); // 计算整个文本的字符数量
    f.seekg(ios::end); // 将指针放置于文件末尾
    for (int index = 2; index <= maxpos; index++) { 
      f.seekg(-index, ios::end); // 从文件末尾最后一个字符开始读
      f.get(x); // 每次获取一个字符
      if (x == 0) { // 如果当前没有字符则跳过
         continue;
      }
      if (x == '\n') { // 如果遇到换行说明此时一行的语句已经读完了
        string templine = line; // 将读到的一行存入templine
        if (templine.size() > 0) { // 判断这行语句是空语句还是非空
          word = templine[0];
          position = 1; // 初始化位置
          size = atoi(word.c_str()); // 读入第一个指令占几个字符
          order.assign(templine, position + 1, size) ; //在读到的一整行语句中从索引position开始最多n个字符往后读
          position = position + size + 1; // 变更position位置使其跳转到第二个参数key对应的大小开始
          word.clear(); // word清空
          for (int i = position + 1; ; i++) {
            if (templine[i] != ' ') {
              word.push_back(templine[i]);
            } else {
              position = i;
              size = atoi(word.c_str());
              key.assign(templine, position + 1, size); // 将key存入 
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
              size = atoi(word.c_str());          // 将value存入
              value.assign(templine, position + 1, size);
            }
          }
          if (key.compare(Key) == 0 && !value.empty()) { // 判断文件是否有对应的key以及此时的key有没有value值
            flag = true;
            break;
          }
        }
        line = "";
        word = "";
        key = ""; // 初始化
        value = "";
        order = "";
      } else {
        line = x + line; // 添加每个字符到line
      }
      x = 0; // 初始化
    }
    if (flag) {
      return value;  // 找到返回value
    } else {
      return "not found"; // 未找到返回not found
    }
  }
  private:
    int maxpos, size, position; // 文本最大字符数量， 每个指令的数值大小， 索引位置
    bool flag; // 判断是否找到key的变量
    char x; // 文件每次读入的字符
    string order, key, value, word = "";  // 定义变量
    string line, sql, Key; //每次一行的语句，用来拼接的语句，用户给的Key
    ofstream ofs;
};
