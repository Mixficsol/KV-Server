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

  std::vector<std::string> NormalFinterpreter(char* buf);
  std::vector<std::string> Finterpreter(char* buf);
  int GetRequest(std::vector<std::string> data, char* buf);
  void SendReply();

 protected:

 private:
  static Coon* coon_;

  Coon(Coon& se);
  void operator =(const Coon& se);
};

#endif // __KV_COON_H_

