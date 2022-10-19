#include "csapp.h"
#include <map>
#include <string>
#include <iostream>
#include <mutex>
#include "hiredis/hiredis.h"

using namespace std;

mutex my_mutex;
map<string,string> mp;
redisContext* myredis = redisConnect("127.0.0.1",6379);
void echo(int connfd) {
  size_t n;
  char buf[MAXLINE];
  string key, value, sql;
  rio_t rio;
  Rio_readinitb(&rio, connfd); //初始化描述符
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    if (myredis->err) {
      cout << "Connection Error:" << myredis->errstr << endl;
    }
	int j = 0;
  	int k = 0;
    int i = 4;
  	key.clear();
    value.clear();;
	char *p = buf;
  	int count = 0;
    while (*p++ != '\0') {
	  count++;
	}
	if (buf[0] == 's' || buf[0] == 'S') {
	  for (i = 4 ; i < count; i++) {
	    if (buf[i] != ' ') {
		  char c = buf[i];
		  key.push_back(c);
		} else {
		  break;
	  	}
   	  }
	  for (int cnt = i + 1; cnt < count; cnt++) {
  		if (buf[cnt] != ' ') {
          char c = buf[cnt];
		  value.push_back(c);	
		} else {
		  break;
		}
	  }
      cout << "key: " << key << ",value: " << value << endl;
      redisReply* reply = (redisReply*)redisCommand(myredis, "SET %s %s", key.c_str(), value.c_str());
      string a = "OK";
      cout << a << endl;
	  strcpy(buf, a.c_str());
	  Rio_writen(connfd, buf, a.size());
      freeReplyObject(reply);
	  // mp[key] = value;
   	}
	else if (buf[0] == 'g' || buf[0] == 'G') {
	  for (i = 4 ; i < count; i++) {
	    if (buf[i] != ' ' && buf[i] != '\n') {
		  char c = buf[i];
		  key.push_back(c);				
		} else {
		  break;
		}
	  }
      cout << "key: " << key << endl;
      redisReply* reply1 = (redisReply*)redisCommand(myredis, "GET %s", key.c_str());
      string b;
      b = reply1->str;
      strcpy(buf, b.c_str());
      Rio_writen(connfd, buf, b.size());
      freeReplyObject(reply1);
	} else {
      string a = "nothing";
      strcpy(buf, a.c_str());
      Rio_writen(connfd, buf, a.size());
	}
  }
  redisFree(myredis);
}
