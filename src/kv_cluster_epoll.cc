#include "../include/kv_cluster_epoll.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define OPEN_MAX 100
#define SERV_PORT 5000
#define INFTIM 1000

int sockfd, epfd, nfd;
struct epoll_event ev, events[20];
Cluster_Epoll* Cluster_Epoll::cluster_epoll_ = nullptr;

Cluster_Epoll::Cluster_Epoll() {

}

Cluster_Epoll::~Cluster_Epoll() {
  
}

void Cluster_Epoll::Init() {
  if (!cluster_epoll_) {
    cluster_epoll_ = new Cluster_Epoll();
  }
}

Cluster_Epoll* Cluster_Epoll::GetCurrent() {
  assert(cluster_epoll_ != nullptr);
  return cluster_epoll_;
}

void Cluster_Epoll::Epoll_Init(int listenfd) {
  epfd = epoll_create(256);
  ev.data.fd = listenfd; // 设置与要处理的事件相关的文件描述符
  ev.events = EPOLLIN|EPOLLET; // 设置要处理的事件类型
  epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev); // 注册epoll事件
}

void Cluster_Epoll::Set_Init(int connfd) {
  ev.data.fd = connfd; // 设置用于读操作的文件描述符
  ev.events = EPOLLIN|EPOLLET; //设置用于注册的读操作事件
  epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev); // 注册ev
}

void Cluster_Epoll::Set_Read(int sockfd) {
  ev.data.fd = sockfd; // 设置用于读操作的文件描述符
  ev.events = EPOLLIN|EPOLLET; // 修改sockfd上要处理的事件为EPOLIN
  epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
}

void Cluster_Epoll::Set_Write(int sockfd) {
  ev.data.fd = sockfd; // 设置用于写操作的文件描述符
  ev.events = EPOLLOUT|EPOLLET; //设置sockfd上要处理的事件为EPOLLOT
  epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
}

void Cluster_Epoll::Set_Miss(int i) {
  events[i].data.fd = -1;
}

bool Cluster_Epoll::Judge_First(int i, int listenfd) {
  if (events[i].data.fd == listenfd) {
    return true;
  } else {
    return false;
  }
}

bool Cluster_Epoll::Judge_Read(int i) {
  if (events[i].events&EPOLLIN) {
    return true;
  } else {
    return false;
  }
}

bool Cluster_Epoll::Judge_Write(int i) {
  if (events[i].events&EPOLLOUT) {
    return true;
  } else {
    return false;
  }
}

int Cluster_Epoll::Get_Fd(int i) {
  return events[i].data.fd;
}

int Cluster_Epoll::Wait_Epoll() {
  return epoll_wait(epfd, events, 20, 500);
}













