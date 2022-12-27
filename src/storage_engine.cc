#include "storage_engine.h"

StorageEngine* StorageEngine::storage_engine_ = nullptr;

StorageEngine::StorageEngine()
    : is_open_(false),
      leveldb_(nullptr) {
}

StorageEngine::~StorageEngine() {
  assert(!is_open_);
}

/* This is not a thread-safe function */
void StorageEngine::Init() {
  if (!storage_engine_) {
    storage_engine_ = new StorageEngine();
  }
}

StorageEngine* StorageEngine::GetCurrent() {
  assert(storage_engine_ != nullptr);
  return storage_engine_;
}

Status StorageEngine::Open(const std::string& path) {
  assert(!is_open_);
  is_open_ = true;

  Options options;
  options.create_if_missing = true;
  Status s = leveldb::DB::Open(options, path, &leveldb_);
  return Status::OK();
}

void StorageEngine::Close() {
  assert(is_open_);
  delete leveldb_;
}

Status StorageEngine::FlushAll() {
  return Status::OK();
}

Status StorageEngine::Set(const std::string& key, const std::string& value) {
  return Status::OK();
}

Status StorageEngine::Get(const std::string& key, std::string* const value) {
  return Status::OK();
}

Status StorageEngine::Delete(const std::string& key) {
  return Status::OK();
}
