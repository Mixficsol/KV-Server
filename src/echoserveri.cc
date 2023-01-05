#include "csapp.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "kv_exception.h"
#include "kv_cluster_epoll.h"
#include "kv_conn.h"
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
  int listenfd, event_total; //侦听描述符，索取，超时时间内epoll处理时间的个数
  struct sockaddr_in clientaddr; //套接字结构(用于存放套字节地址)
  bool flag = true;
  socklen_t clientlen; // 套接字长度
  std::map<int, Conn*> ConnectionMap;
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
    event_total = Cluster_Epoll::Wait_Epoll();
    for (int index = 0; index < event_total; index++) {
      if (Cluster_Epoll::Judge_First(index, listenfd)) {
        Conn* conn = new Conn();
        conn->ProcessNewConn(listenfd); // 这里的Coon以及Coon*我用的是栈变量，用完即释放，保证每个对象的安全性
        if (conn != nullptr) {  
          ConnectionMap[conn->GetFD()] = conn;
          std::cout << "New Conn" << std::endl;
          Cluster_Epoll::Set_Init(conn->GetFD());
        } else {
          continue;
        }
      } else if (Cluster_Epoll::Judge_Read(index)) {   // 如果是已经连接的用户, 并且收到数据,那么进行读入
        if (ConnectionMap.find(Cluster_Epoll::GetFD(index)) != ConnectionMap.end()) {  // 判断获取到的fd是否存在于ConnectionMap集合中
          int fd = Cluster_Epoll::GetFD(index);
          Conn* conn = ConnectionMap[fd];
          if (conn != nullptr) { // 如果对象不为空
            std::cout << "Begin Read" << std::endl;
            conn->GetRequest();  // /*这里需要判断每次读的数据是否读满才能执行Set_Write*/
            Cluster_Epoll::Set_Write(fd);  
          } else {
            ConnectionMap.erase(fd);  // 如果对象为空则删除CoonectionMap中这个fd,释放fd内存;
            close(fd);
          }
        } else {
          continue;
        }
      } else if (Cluster_Epoll::Judge_Write(index)) {  // 如果有数据发送
        if (ConnectionMap.find(Cluster_Epoll::GetFD(index)) != ConnectionMap.end()) {
          int fd = Cluster_Epoll::GetFD(index); 
          Conn* conn = ConnectionMap[fd];
          if (conn != nullptr) {
            std::cout << "begin write" << std::endl;
            conn->SendReply();
            Cluster_Epoll::Set_Read(fd);
          } else {
            ConnectionMap.erase(fd);
            close(fd);
          }
        } else {
          continue;
        }
      }
    }
  }
  StorageEngine::GetCurrent()->Close();
  LOG(INFO) << "Close StorageEngine...";
  return 0;
}
