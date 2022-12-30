#include "csapp.h"
#include "kv_encode.h"

#include <string>

int main(int argc, char **argv) {
  int clientfd, port, n;
  char *host, buf[MAXLINE];
  std::string str;
  rio_t rio;

  if (argc != 3) {
    fprintf(stderr, "usage: %s <host> <port>\n",argv[0]);
	  exit(0);
  }
  host = argv[1];
  port = atoi(argv[2]);
  clientfd = Open_clientfd(host, port); //返回clientd描述符用于读写
  Rio_readinitb(&rio, clientfd); //初始化描述符,将描述符fd和地址rp处的一个类型为rio_t的读缓冲区联系起来

  while (Fgets(buf, MAXLINE, stdin) != NULL) {
    Rio_writen(clientfd, buf, strlen(buf)); // // 从位置buf处传送strlen(buf)个字节到描述符fd
	  if ((n = Rio_readlineb(&rio, buf, MAXLINE) < 0)) { //从一个内部读缓冲区复制一个文本行，当缓冲区变空时，会自动调用read重新填满缓冲区
      fprintf(stderr, "rio_readlineb error \n");        
    }
    str = EncodeFix::getOrder(buf, 0, 4);
    if (str.compare("exit") == 0) {
      break;
    }
    str = EncodeFix::getOrder(buf, 0, 8);
    if (str.compare("shutdown") == 0) {
      break;
    }
	  Fputs(buf, stdout);
  }
  Close(clientfd);
  exit(0);
}
