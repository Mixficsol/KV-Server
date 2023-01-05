#ifndef __KV_IO_H_
#define __KV_IO_H_

#include <string>

class IO {
 public: 
  static void set(std::string order, std::string key, std::string value); // set函数参数指令,key,value
  static void Flushall(); // 清空leveldb数据库
  static std::string get(std::string Key); // get函数参数指令，key
  static std::string flushall();  // 使用文件清空
  static std::string Extract(std::string templine, int &position); // 提取字符串中的key，value

 protected:

 private:

};

#endif // __KV_IO_H_
