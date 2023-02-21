#ifndef __KV_CONN_H_
#define __KV_CONN_H_

#include "csapp.h"

#include <vector>
#include <string>

class Conn {
 public:
  Conn();
  ~Conn();
  
  void ProcessNewConn(const int& listenfd);  // 建立新连接
  void GetRequest();
  void SendReply();
  void AnalyticData(); // 读取处理字符串
  int GetFD();
  bool IsConnect(); // 判断是否断连
 protected:

 private:
  void Finterpreter(); // 解析函数 
  void Splice(); // 将数字字符串转为整形数
  void Add(); // 将解析好的指令放入serialize_data容器中
  void Parsing(); // 解析参数数字字符串
  void Reset(); // 重置各个参数
  void Assert(); // 判断一次解析中是否到底
  void Memmove();

  enum StateMachine { // 解析处于的各个状态
    kBluk, // *
    kMultiBluk, // $
    kParseString // str
  };

  StateMachine state_; // 当前的状态
  char read_buffer_[MAXLINE];
  char write_buffer_[214096];
  int read_buffer_size_;
  int fd_;
  int actual_read_; // 每次read实际读的字节数
  int left_; // 剩余需要读取的字符串大小
  int bluk_length_;  // *后的数字大小
  int multibluk_length_; // $后的数字大小
  int offset_;  // 偏移量
  bool auth_; // 认证状态
  bool is_connect_; //是否连接
  bool is_end_parsing_; // 一次解析是否到底了
  bool is_end_read_; // 是否读完了
  int analysis_pos_; // 解析位置
  int read_pos_;  // 读取位置
  std::vector<std::string> argv_; // 解析容器 argv
  std::string reply_; // 回复字符串
  std::string buffer_;
  std::string data_;
  std::string Reply_;
};

#endif // __KV_CONN_H_

