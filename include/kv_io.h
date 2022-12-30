#ifndef __KV_IO_H_
#define __KV_IO_H_

#include <string>

class IO_server {
 public: 
  IO_server();
  ~IO_server();
  
  static void Init();
  static IO_server* GetCurrent();

  static void set(std::string order, std::string key, std::string value); // set函数参数指令,key,value
  static void Flushall(); // 清空leveldb数据库

  static std::string get(std::string Key); // get函数参数指令，key
  static std::string flushall();  // 使用文件清空
  static std::string Extract(std::string templine, int &position); // 提取字符串中的key，value

 protected:

 private:
  static IO_server* io_server_;

  IO_server(IO_server& se);
  void operator =(const IO_server& se);
};

#endif // __KV_IO_H_
