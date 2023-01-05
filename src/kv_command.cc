#include "kv_command.h"
#include "storage_engine.h"
#include "kv_encode.h"

#include <string>
#include <vector>

using namespace leveldb;

void Command::SetCommandImpl(const std::vector<std::string>& argv, std::string* const reply) {
  Status s;
  std::string key = argv[1];
  std::string value = argv[2];
  s = StorageEngine::GetCurrent()->Set(key, value);
  if (s.ok()) {
    *reply = Encode::getWord("insert successful!");
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
