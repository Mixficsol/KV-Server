#include "kv_command.h"
#include "storage_engine.h"
#include "kv_encode.h"
#include "conf.h"
#include "kv_conn.h"

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

#include <glog/logging.h>
using namespace leveldb;

extern std::map<int, Conn*> conn_map;

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
  {(char*)"command", 1, Command::FirstCommandImpl},
  {(char*)"mset", 1, Command::MsetCommandImpl},
  {(char*)"mget", 1, Command::MgetCommandImpl},
  {(char*)"keys", 1, Command::KeysCommandImpl},
  {(char*)"clientlist", 1, Command::ClientlistCommandImpl},
};

std::string path = "./db";
std::map<std::string, struct redisCommand> command_map;

void Command::MapInitImpl() {
  int ordertotal = sizeof(redisCommandTable) / sizeof(redisCommand);
  for (int i = 0; i < ordertotal; i++) {
    char* name = redisCommandTable[i].name; 
    command_map[name] = redisCommandTable[i];
  }
}

bool Command::AuthCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  std::string password = argv[1];
  if (!password.compare(PASSWORD)) {
    *reply = "+OK\r\n";
    return true;
  } else {
    *reply = "-Password Error\r\n";
    return false;
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
    *reply = "-Insert failed\r\n";
  }
}

void Command::MsetCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  Status s;
  for (int i = 1; i < argv.size(); i += 2) {
    std::string key = argv[i];
    std::string value = argv[i + 1];
    s = StorageEngine::GetCurrent()->Set(key, value);
    if (!s.ok()) {
      break;
    }
  }
  if (s.ok()) {
    *reply = "+OK\r\n";
  } else {
    *reply = "-Insert failed\r\n";
  }
}

void Command::GetCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  Status s;
  std::string key = argv[1];
  std::string result;
  s = StorageEngine::GetCurrent()->Get(key, &result);
  if (s.ok()) {
    *reply = "+" + result + "\r\n";
  } else {
    *reply = "$-1\r\n";
  }
}

void Command::MgetCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  Status s;
  std::string str = "";
  for (int i = 1; i < argv.size(); i++) {
    std::string key = argv[i];
    std::string result;
    s = StorageEngine::GetCurrent()->Get(key, &result);
    if (s.ok()) {
      str = str + "$" + std::to_string(result.size()) + "\r\n" + result + "\r\n";
    } else {
      str = str + "$-1\r\n";
    }
  }
  *reply = "*" + std::to_string(argv.size() - 1) + "\r\n" + str;
}

void Command::KeysCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  Status s;
  std::vector<std::string> keys;
  StorageEngine::GetCurrent()->Keys(keys);
  std::string str = "";
  for (int i = 0; i < keys.size(); i++) {
    str = str + "$" + std::to_string(keys[i].size()) + "\r\n" + keys[i] + "\r\n";
  }
  *reply = "*" + std::to_string(keys.size()) + "\r\n" + str;
}

void Command::ClientlistCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  std::string str = "";
  int cnt = 0;
  for (int i = 0; i < conn_map.size(); i++) {
    Conn* conn = conn_map[i];
    if (conn == nullptr) {
      continue;
    } else {
       cnt++;
       std::string s = std::to_string(conn->GetFD());
       str = str + "$" + std::to_string(s.size() + 3) + "\r\n" + "fd:" + std::to_string(conn->GetFD()) + "\r\n";
    }
  }
  *reply = "*" + std::to_string(cnt) + "\r\n" + str;
}

void Command::DeleteCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  Status s;
  std::string key = argv[1];
  s = StorageEngine::GetCurrent()->Delete(key);
  if (s.ok()) {
    *reply = "+delete successful!\r\n";
  } else {
    *reply = "-delete failed\r\n";
  }
}

void Command::FlushAllCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  bool flag;
  flag = StorageEngine::GetCurrent()->FlushAll();
  if (flag) {
    *reply = "+OK\r\n";
  } else {
    *reply = "-Clear Error\r\n";
  }
}

void Command::ExitCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  *reply = Encode::getWord("bye");
}

void Command::ShutDownCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  *reply = Encode::getWord("connection closed by server");
}

void Command::AutherrorCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  *reply = "-NOAUTH Authentication required.\r\n";
}

void Command::ErrorCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  *reply = "-Error\r\n";
}

void Command::FirstCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
   std::ifstream ifs("echo.txt");
   std::string content( (std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>() ) );
   *reply = content;
}

struct redisCommand Command::lookupCommand(std::string& cmd) {
  struct redisCommand rediscommand;
  std::map<std::string, struct redisCommand>::iterator iter;
  iter = command_map.find(cmd);
  if (iter != command_map.end()) {
    rediscommand = iter->second;
  } else {
    rediscommand = command_map["error"];
  }
  return rediscommand;
}

