#ifndef __KV_CONN_H_
#define __KV_CONN_H_

#include "csapp.h"

#include <vector>
#include <string>

class Conn {
 public:
  Conn();
  ~Conn();
  
  std::vector<std::string> NormalFinterpreter();
  void Finterpreter();
  void ProcessNewConn(const int& listenfd);
  void GetRequest();
  void SendReply();
  void Splice();
  void Add();
  void Join();
  void Assert();
  void Parsing();
  void Init();
  void AnalyticData(); // 读取处理字符串
  int GetFD();
 protected:

 private:
  char read_buffer_[MAXLINE];
  char order; // 当前指令
  int fd_;
  int left; // 剩余需要读取的字符串大小
  int statement_len_; // $后的数字大小
  int instruction_len_;  // *后的数字大小
  int offset_;  // 偏移量
  bool auth_; // 认证状态
  bool read_; // 读取状态
  bool is_standard_; // 是否规范
  bool flag; // 是否退出解析
  int analysis_pos; // 解析位置
  int read_pos;  // 读取位置
  std::vector<std::string> serialize_data; // 解析容器
  std::string buffer;
  std::string reply; // 回复字符串
  std::string num; // *后的数字字符串
  std::string number; // $后的数字字符串
  std::string data; // 容器字符串
};

#endif // __KV_CONN_H_

