#ifndef __KV_ENCODE_H_
#define __KV_ENCODE_H_

#include <string>
#include <vector>

class EncodeFix {
 public:
  static int getCharLength(const char *p); // 获取从客户端发来的字符串长度  
  static int JudgeShutdown(const char *p); // 判断指令

  static bool Judgestring(const std::string& in, int& cur_pos); // 判断是不是以*开头的序列化语句
  static bool JudgeOrder(const std::string& in, int& cur_pos); // 判断是不是以$开头的参数语句
  static bool paramtertotal(const std::string& in, int& cur_pos, int& size); // 使字符串指令变成int类型的size 
  static bool FindNextSeparators(const std::string& in, int& length, int& cur_pos); // 找到客户端字符串中下一个'\n'的位置

  static void Split(const std::string& in, std::vector<std::string>* out, int &cur_pos, int& size); // 使key, value, 指令装到vector里面
  static void orderTolower(std::string order); // 指令小写化

  static std::string getOrder(char *buf, int index, int maxsize); // 获取字符串长度指令, key, value
  static std::string getWord(std::string word); // 将字符串末尾加上换行
  static std::string sqlSplice(std::string sql, std::string word, int flag); // 拼接字符串
  
 protected:

 private:
  
};

#endif // __KV_ENCODE_H_













  
