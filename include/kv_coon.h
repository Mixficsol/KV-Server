#ifndef __KV_COON_H_
#define __KV_COON_H_

#include "csapp.h"

#include <vector>
#include <string>

class Coon {
 public:
  Coon();
  ~Coon();
  
  std::vector<std::string> NormalFinterpreter(char* buf);
  std::vector<std::string> Finterpreter(char* buf);
  Coon Establish(int listenfd, struct sockaddr_in* clientaddr, socklen_t* clientlen)
;
  int GetRequest(char* buf);
  void SendReply(const int& fd, const char line[], const int& size);
  void Set_fd(int fd);
  void Set_auth(bool flag);
  void Set_write_buffer_(char buf[]);
  int Get_fd();
  bool Get_auth();
  char* Get_read_buffer_();
  char* Get_write_buffer_();
  bool Fd_Exception(int fd);
  bool Read_Exception(int n);
 protected:

 private:
  char read_buffer_[MAXLINE];
  char write_buffer_[MAXLINE];
  int fd_;
  bool auth_;
  
};

#endif // __KV_COON_H_

