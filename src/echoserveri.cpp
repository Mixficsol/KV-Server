#include "csapp.h"
#include "include/Encode.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define OPEN_MAX 100
#define SERV_PORT 5000
#define INFTIM 1000
#define LISTENQ 20

using namespace std;

void echo(char* line);

int main(int argc, char **argv) {
  char line[MAXLINE];
  ssize_t n;
  int listenfd, connfd, port, i, sockfd, epfd, nfds;  //侦听描述符，读写描述符，端口
  socklen_t clientlen; //套接字长度
  struct sockaddr_in clientaddr; //套接字结构(用于存放套字节地址)
  struct epoll_event ev, events[20]; //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
  EncodeFix a;
  bool flag;
  epfd = epoll_create(256); //生成用于处理accept的epoll专用的文件描述符
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
  }
  /*listenfd和connfd的区别:监听描述符是作为客户端连接请求的一个端点.它通常被创建一次，并存在于服务器的整个生命周期.已连接描述符是
   * 客户端和服务器之间已经建立起来了的连接的一个端点.服务器每次接受连接请求时都会被创建一次，它只存在于服务器为一个客户端服务的过程中
  */
  port = atoi(argv[1]); // 端口号
  listenfd = Open_listenfd(port); // 服务器创建一个监听描述符，准备好接受连接请求
  ev.data.fd = listenfd; // 设置与要处理的事件相关的文件描述符
  ev.events = EPOLLIN|EPOLLET; // 设置要处理的事件类型
  epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev); // 注册epoll事件
  while (1) {
    KOOK(line);
    if (line[0] == 's' && line[1] == 'h') {
      break;
    }
    nfds = epoll_wait(epfd, events, 20, 500);
    for (i = 0; i < nfds; ++i) {
      if (events[i].data.fd == listenfd) {
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        if (connfd < 0) {
          perror("connfd < 0");
          exit(1);
        }
        char *str = inet_ntoa(clientaddr.sin_addr);
        cout << "accapt a connection from" << str << endl;
        ev.data.fd = connfd; // 设置用于读操作的文件描述符
        ev.events = EPOLLIN|EPOLLET; //设置用于注册的读操作事件
        epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev); // 注册ev
      }
      else if (events[i].events&EPOLLIN) {// 如果是已经连接的用户, 并且收到数据,那么进行读入
        cout << "EPOLLIN" << endl;
        if ((sockfd = events[i].data.fd) < 0) {
          continue;
        }
        n = read(sockfd, line, MAXLINE);
        if (n < 0) {
          if (errno == ECONNRESET) {
            close(sockfd);
            events[i].data.fd = -1;
          } else {
            cout << "readline error" << endl;
          }
        } 
        else if (n == 0) {
          close(sockfd);
          events[i].data.fd = -1;
        } else {
          echo(line);
          ev.data.fd = sockfd; // 设置用于写操作的文件描述符
          ev.events = EPOLLOUT|EPOLLET; //设置sockfd上要处理的事件为EPOLLOT
          epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
        }
      }
      else if (events[i].events&EPOLLOUT) {  // 如果有数据发送
        sockfd = events[i].data.fd;
        n = a.getCharLength(line);
        write(sockfd, line, n);
        if (line[0] == 's' && line[1] == 'h') {
          break;
        } else {
          ev.data.fd = sockfd; // 设置用于读操作的文件描述符
          ev.events = EPOLLIN|EPOLLET; // 修改sockfd上要处理的事件为EPOLIN
          epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
        }
      }
    }
  }
  return 0;
}
