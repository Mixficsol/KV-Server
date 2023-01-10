#include "csapp.h"
#include "kv_encode.h"
#include "conf.h"

#include <string>
#include <glog/logging.h>

int main(int argc, char **argv) {
  int clientfd, n;
  char buf[3 * MAXLINE];
  rio_t rio;
  clientfd = Open_clientfd(HOST, PORT); //返回clientd描述符用于读写
  Rio_readinitb(&rio, clientfd); //初始化描述符,将描述符fd和地址rp处的一个类型为rio_t的读缓冲区联系起来
  while (Fgets(buf, MAXLINE, stdin) != NULL) {
    Rio_writen(clientfd, buf, strlen(buf)); // // 从位置buf处传送strlen(buf)个字节到描述符fd
    if ((n = Rio_readlineb(&rio, buf, MAXLINE * 3) < 0)) {
      fprintf(stderr, "rio_readlineb error \n");        
    }
	  Fputs(buf, stdout);
  }
  Close(clientfd);
  exit(0);
}
