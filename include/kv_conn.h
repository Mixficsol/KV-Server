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
  std::vector<std::string> Finterpreter();
  void ProcessNewConn(const int& listenfd);
  void GetRequest();
  void SendReply();
  int GetFD();
  bool JudgeReadBufferCommand();

 protected:

 private:
  char read_buffer_[MAXLINE];
  char write_buffer_[MAXLINE];
  int fd_;
  int read_buffer_size;
  int write_buffer_size;
  bool auth_;

};

#endif // __KV_CONN_H_

