#include "../include/csapp.h"
#include "../include/kv_encode.h"
#include "../include/kv_io.h"
#include "../include/kv_leveldb.h"
#include "../include/kv_cluster_epoll.h"
#include <string>
#include <iostream>

#define OPEN_MAX 100
#define SERV_PORT 5000
#define INFTIM 1000

int echo(char* line);

int main(int argc, char **argv) {
  LevelDB a;
  Cluster_Epoll b;
  EncodeFix c;
  char line[MAXLINE];
  ssize_t n;
  int listenfd, connfd, port, i, sum, fd, size; //侦听描述符，读写描述符，端口
  socklen_t clientlen; //套接字长度
  struct sockaddr_in clientaddr; //套接字结构(用于存放套字节地址)
  bool flag;
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
  }
  if (a.OpenDatabase()) {
    printf("Open successful\n");
  } else {
    printf("Open failed\n");
    exit(1);
  }
  /*listenfd和connfd的区别:监听描述符是作为客户端连接请求的一个端点.它通常被创建一次，并存在于服务器的整个生命周期.已连接描述符是
   * 客户端和服务器之间已经建立起来了的连接的一个端点.服务器每次接受连接请求时都会被创建一次，它只存在于服务器为一个客户端服务的过程中
  */
  port = atoi(argv[1]); // 端口号
  listenfd = Open_listenfd(port); // 服务器创建一个监听描述符，准备好接受连接请求
  b.Epoll_Init(listenfd);
  while (1) {
    int kf = c.KOOK(line);
    if (line[0] == 's' && line[1] == 'h') {
      break;
    }
    sum = b.Wait_Epoll();
    for (i = 0; i < sum; ++i) {
      if (b.Judge_First(i, listenfd)) {
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        if (connfd < 0) {
          exit(1);
        }
        char *str = inet_ntoa(clientaddr.sin_addr);
        std::cout << "accapt a connection from" << str << std::endl;
        b.Set_Init(connfd);
      } else if (b.Judge_Read(i)) {   // 如果是已经连接的用户, 并且收到数据,那么进行读入
        if ((fd = b.Get_Fd(i)) < 0) {
          continue;
        }
        n = read(fd, line, MAXLINE);
        if (n <= 0) {
          if (errno == ECONNRESET) {
            close(fd);
            b.Set_Miss(i);
          } else {
            std::cout << "readline error" << std::endl;
          }
        } else {
          size = echo(line);
          std::cout << "line: " << line << std::endl;
          b.Set_Write(fd);
        }
      } else if (b.Judge_Write(i)) {  // 如果有数据发送
        fd = b.Get_Fd(i);
        std::cout << "size: " << size << std::endl;
        write(fd, line, size);
        if (line[0] == 's' && line[1] == 'h') {
          break;
        } else {
          b.Set_Read(fd);
        }
      }
    }
  }
  if (a.CloseDatabase()) { // 关闭数据库
    printf("Database close");
  } else {
    printf("Datavase close failed");
  }
  return 0;
}
