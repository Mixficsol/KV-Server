#ifndef __KV_COMMAND_H_
#define __KV_COMMAND_H_

#include <string>
#include <vector>

class Command {
  public:
  static void MapInitImpl();
  static void SetCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void GetCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void DeleteCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void FlushAllCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void ExitCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void ShutDownCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void ErrorCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void AutherrorCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void FirstCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static bool AuthCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void MsetCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void MgetCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void KeysCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void ClientCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static void DbsizeCommandImpl(const std::vector<std::string>& argv, std::string* const reply);
  static struct redisCommand lookupCommand(std::string& cmd, const std::vector<std::string>& argv);
  protected:

  private:

};

#endif // __KV_COMMAND_H_
