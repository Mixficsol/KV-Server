#include <iostream>
#include "csapp.h"
#include "Encode.h"

using namespace std;

int main(int argc, char **argv) {
  int clientfd, port, n;
  string str;
  char *host, buf[MAXLINE];
  rio_t rio;

  if (argc != 3) {
    fprintf(stderr, "usage: %s <host> <port>\n",argv[0]);
	exit(0);
  }
  host = argv[1];
  port = atoi(argv[2]);
  clientfd = Open_clientfd(host, port); //返回clientd描述符用于读写
  Rio_readinitb(&rio, clientfd);

  while (Fgets(buf, MAXLINE, stdin) != NULL) {
    EncodeFix b;
    Rio_writen(clientfd, buf, strlen(buf));
	if ((n = Rio_readlineb(&rio, buf, MAXLINE) < 0)) {
      fprintf(stderr, "rio_readlineb error \n");        
    }
    str = b.getOrder(buf, 0, 4);
    if (str.compare("exit") == 0) {
      cout << "str: " << str << endl;
      break;
    }
	Fputs(buf, stdout);
  }
  Close(clientfd);
  exit(0);
}
