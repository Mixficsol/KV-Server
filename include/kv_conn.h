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
  std::vector<std::string> Finterpreter(int* const cnt, int* const cur_pos);
  int Finterpretersize();
  void ProcessNewConn(const int& listenfd);
  void GetRequest();
  void SendReply();
  int GetFD();

 protected:

 private:
  char read_buffer_[MAXLINE];
  int fd_;
  bool auth_;
  std::string reply;

};

#endif // __KV_CONN_H_

