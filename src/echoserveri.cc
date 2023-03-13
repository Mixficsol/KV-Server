#include "csapp.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "kv_exception.h"
#include "kv_cluster_epoll.h"
#include "kv_conn.h"
#include "storage_engine.h"
#include "kv_command.h"
#include "conf.h"
#include "define.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include <signal.h>
#include <glog/logging.h>

#define OPEN_MAX 100
#define SERV_PORT 5000
#define INFTIM 1000

extern struct redisCommand rediscommand;
extern struct redisCommand redisCommandTable [];
extern std::map<std::string, struct redisCommand> command_map;

int keepRunning = 1;

static void ServerGlogInit() {
  FLAGS_log_dir = LOG_PATH;
  FLAGS_minloglevel = 0;
  FLAGS_max_log_size = 1800;
  FLAGS_logbufsecs = 0;
  FLAGS_alsologtostderr = true;
  ::google::InitGoogleLogging("kv_server");
}

std::map<int, Conn*> conn_map;

int main(int argc, char **argv) {
  int listenfd, event_total; //侦听描述符，索取，超时时间内epoll处理时间的个数
  /* Glog init */
  conn_map.clear();
  ServerGlogInit();
  Command::MapInitImpl();   // 命令Map初始化
 /* Initializing the storage engine */
  std::string path = DB_PATH;
  StorageEngine::Init();
  Define::Init();
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
  ClusterEpoll::EpollInit(listenfd);
  Define def;
  while (keepRunning) {
    event_total = ClusterEpoll::WaitEpoll();
    Define::GetCurrent()->GetQps();
   // std::cout << "event_total: " << event_total << std::endl;
    for (int index = 0; index < event_total; index++) {
      if (ClusterEpoll::JudgeFirst(index, listenfd)) {
        Conn* conn = new Conn();
        assert(conn != nullptr);
        conn->ProcessNewConn(listenfd); // 这里的Coon以及Coon*我用的是栈变量，用完即释放，保证每个对象的安全性
        conn_map[conn->GetFD()] = conn;
        ClusterEpoll::SetInit(conn->GetFD());
      } else if (ClusterEpoll::JudgeRead(index)) {   // 如果是已经连接的用户, 并且收到数据,那么进行读入
        if (conn_map.find(ClusterEpoll::GetFD(index)) != conn_map.end()) {  // 判断获取到的fd是否存在于ConnectionMap集合中
          int fd = ClusterEpoll::GetFD(index);
          Conn* conn = conn_map[fd];
          assert(conn != nullptr);
          conn->AnalyticData();
          if (!conn->IsConnect()) {
            conn_map.erase(fd);
            close(fd);
            delete conn;
          }
        } else {
          close(ClusterEpoll::GetFD(index));
        }
      } else if (ClusterEpoll::JudgeWrite(index)) {  // 如果有数据发送
        if (conn_map.find(ClusterEpoll::GetFD(index)) != conn_map.end()) {
          int fd = ClusterEpoll::GetFD(index); 
          Conn* conn = conn_map[fd];
          assert(conn != nullptr);
          conn->SendReply();
          if (!conn->IsConnect()) {
            conn_map.erase(fd);
            close(fd);
            delete conn;
          }
        } else {
          close(ClusterEpoll::GetFD(index));
        }
      }
    }
  }
  StorageEngine::GetCurrent()->Close();
  LOG(INFO) << "Close StorageEngine...";
}
