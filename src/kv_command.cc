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
    *reply = EncodeFix::getWord("insert successful!");
  } else {
    *reply = EncodeFix::getWord("insert failed");
  }
}

void Command::fun() {

}
