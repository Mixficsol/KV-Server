#ifndef __STORAGE_ENGINE_H_
#define __STORAGE_ENGINE_H_

#include <string>
#include <leveldb/db.h>
#include <vector>

using namespace leveldb;

class StorageEngine {
 public:
  StorageEngine();
  ~StorageEngine();

  static void Init();
  static StorageEngine* GetCurrent();

  Status Open(const std::string& path);
  void Close();

  bool FlushAll();
  Status Set(const std::string& key, const std::string& value);
  Status Get(const std::string& key, std::string* const value);
  Status Delete(const std::string& key);
  void Keys(std::vector<std::string>* const keys);
  void Dbsize(int* const count);
 private:
  bool is_open_;
  leveldb::DB* leveldb_;

  static StorageEngine* storage_engine_;

  StorageEngine(StorageEngine& se);
  void operator =(const StorageEngine& se);
};

#endif  // __STORAGE_ENGINE_H_
