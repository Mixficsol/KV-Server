#ifndef __KV_ENCODE_H_
#define __KV_ENCODE_H_

#include <string>
#include <vector>

class Encode {
 public:
  static int getCharLength(const char *p); // 获取从客户端发来的字符串长度  
  static bool Judgestring(const char in[]); // 判断是不是以*开头的序列化语句
  static bool JudgeOrder(const std::string& in, int* const cur_pos); // 判断是不是以$开头的参数语句
  static bool paramtertotal(const std::string& in, int* const cur_pos, int* const size); // 使字符串指令变成int类型的size 
  static bool Split(const std::string& in, std::vector<std::string>* const out, int* const cur_pos, int* const size); // 使key, value, 指令装到vector里面
static void orderTolower(std::string& order); // 指令小写化
  static void Forwardsearch(const std::string& in, int* const cur_pos);
  static void Backsearch(const std::string& in, int* const cur_pos);
  static std::string getOrder(char *buf, int& index, int maxsize); // 获取字符串长度指令, key, value
  static std::string getWord(std::string word); // 将字符串末尾加上换行
  static std::string sqlSplice(std::string sql, std::string word, int flag); // 拼接字符串  
 protected:

 private:
  
};

#endif // __KV_ENCODE_H_














  
