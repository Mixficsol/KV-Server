#ifndef __KV_COON_H_
#define __KV_COON_H_

#include <vector>
#include <string>

class Coon {
 public:
  Coon();
  ~Coon();
  
  static Coon* Init(int fd);
  static Coon* GetCurrent();
  
  static std::vector<std::string> NormalFinterpreter(char* buf);
  static std::vector<std::string> Finterpreter(char* buf);
  static int GetRequest(char* buf);
  static void SendReply(const int& fd, const char line[], const int& size);
  int Get_fd();
  bool Get_auth();
 protected:

 private:
  int coon_fd;
  bool auth;
  static Coon* coon_;
  
};

#endif // __KV_COON_H_

