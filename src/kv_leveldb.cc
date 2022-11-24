#include "../include/kv_leveldb.h"
#include <leveldb/db.h>
#include <string>
#include <iostream>

leveldb::DB *db;
leveldb::Options options;
leveldb::Status status;
std::string dbpath = "leveldb.db";

bool LevelDB::OpenDatabase() { 
  options.create_if_missing = true;
  status = leveldb::DB::Open(options, dbpath, &db);
  if (status.ok() == false) {  // 判断数据库是否打开成功
    return false;   
  } else {
    return true;
  }
}

std::string LevelDB::LevelDB_set(std::string key, std::string value) {
  status = db->Put(leveldb::WriteOptions(), key, value);
  if (status.ok()) {
     return "insert successful!";
  } else {
     return "leveldb write faile";
  }
}

std::string LevelDB::LevelDB_get(std::string key) {
  std::string result;
  status = db->Get(leveldb::ReadOptions(), key, &result);
  std::cout << "result: " << result << std::endl;
  if (status.ok()) {
    return result;
  } else {
    return "not found";
  }
}

std::string LevelDB::Flushall() {
  int flag = system("rm -rf /root/Git/KV-Server/leveldb.db");
  if (flag) {
    return "clear";
  } else {
    return "clear failed";
  }
}

bool LevelDB::CloseDatabase() {
  delete db;
  return true;
}




