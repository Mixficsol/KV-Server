#ifndef __KV_LEVELDB_H_
#define __KV_LEVELDB_H_

#include <string>
#include <leveldb/db.h>

class LevelDB {
 public:
   bool OpenDatabase(); // 开启数据库
   bool CloseDatabase(); // 关闭数据库
   bool LevelDB_set(std::string key, std::string value); // 执行set
   leveldb::Status LevelDB_get(std::string key, std::string* result); // 执行get
   std::string Flushall(); // 清空数据库
 protected:
 
 private:
};
#endif
