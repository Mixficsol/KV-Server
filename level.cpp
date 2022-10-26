#include <iostream>
#include "leveldb/db.h"

using namespace std;

int main() {
  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing = true;
  string dbpath = "./leveldb1.db";
  for (int i = 0; i < 3; i++) {
  leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);
  if (status.ok() == false) {
    cout << "leveldb open failed" << endl;
    return -1;
  }
  string key;
  string result;
  string value;
  cout << "Open db ok" << endl;
  cin >> key >> value;
  if (key.compare("exit") == 0) {
    cout << key << endl;
    delete db;
    return 0;        
  }
  status = db->Put(leveldb::WriteOptions(), key, value);
  if (status.ok() == false) {
     cout << "leveldb write failed " << endl;
     delete db;
     return -1;
  }
  cout << "insert successful" << endl;
  status = db->Get(leveldb::ReadOptions(), key, &result);
  if (status.ok() == false) {
    cout << "leveldb read failed" << endl;
    delete db;
    return -1;
   }
   cout << "result: " << result << endl;
   delete db;
  }
   return 0;
}
