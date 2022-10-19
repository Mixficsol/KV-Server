#include <iostream>
#include "hiredis/hiredis.h"

using namespace std;

redisContext* myredis = redisConnect("127.0.0.1",6379);
int main() {
  while (1) {
  if (myredis->err) {
    cout << "Connection Error:" << myredis->err << endl;
  }
  string key, value, sql;
  cin >> key;
  cin >> value;
  redisReply* reply = (redisReply*)redisCommand(myredis, "SET %s %s", key.c_str(), value.c_str());
  if (!reply) {
    cout << "reply空指针" << endl;
  } else {
    cout << reply->str << endl;
  }
  redisReply* reply1 = (redisReply*)redisCommand(myredis, "GET %s", key.c_str());
  if (!reply1) {
    cout << "reply1空指针" << endl;
  } else {
    cout << reply1->str << endl;
  }
  freeReplyObject(reply1);
  freeReplyObject(reply);
  }
  redisFree(myredis);
  return 0;
}

