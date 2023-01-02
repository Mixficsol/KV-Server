#include "csapp.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "kv_cluster_epoll.h"
#include "kv_coon.h"
#include "storage_engine.h"
#include "conf.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include <glog/logging.h>

#define OPEN_MAX 100
#define SERV_PORT 5000
#define INFTIM 1000

static void ServerGlogInit() {
  FLAGS_log_dir = "./log";
  FLAGS_minloglevel = 0;
  FLAGS_max_log_size = 1800;
  FLAGS_logbufsecs = 0;
  FLAGS_alsologtostderr = true;
  ::google::InitGoogleLogging("kv_server");
}

int main(int argc, char **argv) {
  char line[MAXLINE];
  ssize_t n;
  int listenfd, connfd,  i, sum, fd, size; //侦听描述符，读写描述符，端口
  struct sockaddr_in clientaddr; //套接字结构(用于存放套字节地址)
  bool flag = true;
  Coon coon;
  socklen_t clientlen; // 套接字长度
  std::map<int, Coon> m;
  /* Glog init */
  ServerGlogInit();
 /* Initializing the storage engine */
  std::string path = "./db";
  StorageEngine::Init();
  Status s = StorageEngine::GetCurrent()->Open(path);
  if (s.ok()) {
    LOG(INFO) << "Open Storage engine success...";
  } else {
    LOG(ERROR) << "Open Storage engine failed: " << s.ToString();
    exit(-1);
  }
  
  /*listenfd和connfd的区别:监听描述符是作为客户端连接请求的一个端点.它通常被创建一次，并存在于服务器的整个生命周期.已连接描述符是
   * 客户端和服务器之间已经建立起来了的连接的一个端点.服务器每次接受连接请求时都会被创建一次，它只存在于服务器为一个客户端服务的过程中 */
  listenfd = Open_listenfd(PORT); // 服务器创建一个监听描述符，准备好接受连接请求
  Cluster_Epoll::Epoll_Init(listenfd);
  while (flag) {
    sum = Cluster_Epoll::Wait_Epoll();
    for (i = 0; i < sum; ++i) {
      if (Cluster_Epoll::Judge_First(i, listenfd)) {
        Coon connection = Establish(listenfd, clientaddr, clientlen);
        m[connfd] = connection;
        Cluster_Epoll::Set_Init(connfd);
      } else if (Cluster_Epoll::Judge_Read(i)) {   // 如果是已经连接的用户, 并且收到数据,那么进行读入
        coon = m[connfd];
        coon.GetRequest(Cluster_Epoll::Get_Fd(i));
        Cluster_Epoll::Set_Write(fd);
      } else if (Cluster_Epoll::Judge_Write(i)) {  // 如果有数据发送
        coon = m[connfd];
        coon.SendReply(fd);
        Cluster_Epoll::Set_Read(fd);
      }
    }
  }
  StorageEngine::GetCurrent()->Close();
  LOG(INFO) << "Close StorageEngine...";
  return 0;
}
