#ifndef __KV_COMMAND_H_
#define __KV_COMMAND_H_

#include <string>
#include <vector>

class Coomand {
  public:
  static void SetCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  
  protected:

  private:

};

#endif // __KV_COMMAND_H_
