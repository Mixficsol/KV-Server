#include "kv_command.h"
#include "storage_engine.h"
#include "kv_encode.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

#include <glog/logging.h>

using namespace leveldb;

typedef struct redisCommand {
  char* name;
  int parameter_num;
  void (*pf)(const std::vector<std::string>&, std::string* const);
} redisCommand;

struct redisCommand redisCommandTable [] = {
  {(char*)"set", 3, Command::SetCommandImpl},
  {(char*)"get", 2, Command::GetCommandImpl},
  {(char*)"delete", 1, Command::DeleteCommandImpl},
  {(char*)"flushall", 1, Command::FlushAllCommandImpl},
  {(char*)"exit", 1, Command::ExitCommandImpl},
  {(char*)"shutdown", 1, Command::ShutDownCommandImpl},
  {(char*)"error", 1, Command::ErrorCommandImpl},
  {(char*)"command", 1, Command::FirstCommandImpl}
};

std::map<std::string, struct redisCommand> command_map;

void Command::MapInitImpl() {
  for (int i = 0; i < 8; i++) {
    char* name = redisCommandTable[i].name;
    command_map[name] = redisCommandTable[i];
  }
}

void Command::SetCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  Status s;
  std::string key = argv[1];
  std::string value = argv[2];
  s = StorageEngine::GetCurrent()->Set(key, value);
  if (s.ok()) {
    *reply = "+OK\r\n";
  } else {
    *reply = Encode::getWord("insert failed");
  }
}

void Command::GetCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  Status s;
  std::string key = argv[1];
  std::string result;
  s = StorageEngine::GetCurrent()->Get(key, &result);
  if (s.ok()) {
    *reply = Encode::getWord(result);
  } else {
    *reply = Encode::getWord("not found");
  }
}

void Command::DeleteCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  Status s;
  std::string key = argv[1];
  s = StorageEngine::GetCurrent()->Delete(key);
  if (s.ok()) {
    *reply = Encode::getWord("delete successful!");
  } else {
    *reply = Encode::getWord("delete failed");
  }
}

void Command::FlushAllCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  bool flag;
  flag = StorageEngine::GetCurrent()->FlushAll();
  if (flag) {
    *reply = Encode::getWord("clear successful!");
  } else {
    *reply = Encode::getWord("clear failed");
  }
}

void Command::ExitCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  *reply = Encode::getWord("bye");
}

void Command::ShutDownCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  *reply = Encode::getWord("connection closed by server");
}

void Command::ErrorCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  *reply = Encode::getWord("Error");
}

void Command::FirstCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
   std::ifstream ifs("echo.txt");
   std::string content( (std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>() ) );
   *reply = content;
}

struct redisCommand Command::lookupCommand(std::string& cmd) {
  struct redisCommand rediscommand;
  Encode::orderTolower(cmd);
  std::map<std::string, struct redisCommand>::iterator iter;
  iter = command_map.find(cmd);
  if (iter != command_map.end()) {
    rediscommand = iter->second;
  } else {
    rediscommand = command_map["error"];
  }
  return rediscommand;
}
