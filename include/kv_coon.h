#ifndef __KV_COON_H_
#define __KV_COON_H_

#include <vector>
#include <string>

class Coon {
 public:
  Coon();
  ~Coon();
  
  static void Init();
  static Coon* GetCurrent();

  static std::vector<std::string> NormalFinterpreter(char* buf);
  static std::vector<std::string> Finterpreter(char* buf);
  static int GetRequest(const std::vector<std::string>& data, char* buf);
  static void SendReply(const int& fd, const char line[], const int& size);

 protected:

 private:
  int coon_fd;
  bool auth;
  static Coon* coon_;
  
  Coon(Coon& se);
  void operator =(const Coon& se);
};

#endif // __KV_COON_H_

