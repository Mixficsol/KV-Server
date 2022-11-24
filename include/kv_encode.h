#ifndef __KV_ENCODE_H_
#define __KV_ENCODE_H_

#include <string>

class EncodeFix {
 public:
  int getCharLength(char *p); // 获取从客户端发来的字符串长度  
  int KOOK(char *p); // 判断指令

  void orderTolower(std::string order); // 指令小写化

  std::string getOrder(char *buf, int index, int maxsize); // 获取字符串长度指令, key, value
  std::string getWord(std::string word); // 将字符串末尾加上换行
  std::string sqlSplice(std::string sql, std::string word, int flag); // 拼接字符串
  
 protected:

 private:
};
#endif













  
