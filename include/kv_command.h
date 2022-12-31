#ifndef __KV_COMMAND_H_
#define __KV_COMMAND_H_

#include <string>
#include <vector>

class Command {
  public:
   Command();
   ~Command();

  static void Init();
  static void SetCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void fun(); 

  protected:

  private:
   static Command* command_;

   Command(Command& se);
   void operator =(const Command& se);
};

#endif // __KV_COMMAND_H_
