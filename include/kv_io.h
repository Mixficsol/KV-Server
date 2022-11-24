#ifndef __KV_IO_H_
#define __KV_IO_H_
#include <string>

class IO_server {
 public:  // 公共部分函数    
  void set(std::string order, std::string key, std::string value); // set函数参数指令,key,value
  void Flushall(); // 清空leveldb数据库

  std::string get(std::string Key); // get函数参数指令，key
  std::string flushall();  // 使用文件清空
  std::string Extract(std::string templine, int &position); // 提取字符串中的key，value

 protected:

 private:
};
#endif
