#include "csapp.h"
#include <map>
#include <string>
#include <iostream>

using namespace std;
void echo(int connfd) {
  size_t n;
  printf("Hello");
  char buf[MAXLINE];
  string key, value;
  rio_t rio;
  map<string,string> mp;
  Rio_readinitb(&rio, connfd); //初始化描述符
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
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
	if (buf[0] == 's') {
	  string a = "OK\n";
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
	  mp[key] = value;
	  strcpy(buf,a.c_str());
	  Rio_writen(connfd, buf, a.size());
	}
	else if (buf[0] == 'g') {
	  for (i = 4 ; i < count; i++) {
	    if (buf[i] != ' ' && buf[i] != '\n') {
		  char c = buf[i];
		  key.push_back(c);				
		} else {
		  break;
		}
	  }
      cout << "key: " << key << ",value: " << value << endl;
	  if (mp.find(key) == mp.end()) {
	    string a = "ERROR\n";
		strcpy(buf,a.c_str());
		Rio_writen(connfd, buf, a.size());
	  } else {
        cout << "find" << endl;
		strcpy(buf,mp[key].c_str());
		Rio_writen(connfd, buf, mp[key].size());
	  }
	} else {
	  Rio_writen(connfd, buf, n);
	}
  }
}
