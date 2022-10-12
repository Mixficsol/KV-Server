#include "csapp.h"

void echo(int connfd);

int main(int argc, char **argv) {
  int listenfd, connfd, port;  //侦听描述符，读写描述符，端口
  socklen_t clientlen; //套接字长度
  struct sockaddr_in clientaddr; //套接字结构
  struct hostent *hp; // 
  char *haddrp;
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
  }
  port = atoi(argv[1]);
  listenfd = Open_listenfd(port);
  while (1) {
    clientlen = sizeof(struct sockaddr_storage);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // 返回一个已连接的描述符
	/* Determine the domain name and IP address of the client */
	hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,sizeof(clientaddr.sin_addr.s_addr), AF_INET); //将套接字地址结构转换成主机和服务串
	haddrp = inet_ntoa(clientaddr.sin_addr); //sin_adder成员是一个32位IP地址
	printf("server connected to %s (%s) \n", hp->h_name, haddrp);
	echo(connfd);
	Close(connfd);
  }
  exit(0);
}
