#include "csapp.h"
#include "kv_encode.h"
#include "kv_io.h"
#include "kv_leveldb.h"
#include "kv_cluster_epoll.h"
#include "kv_coon.h"
#include "storage_engine.h"

#include <string>
#include <iostream>
#include <vector>

#include <glog/logging.h>

#define OPEN_MAX 100
#define SERV_PORT 5000
#define INFTIM 1000

int echo(char* line);

static void ServerGlogInit() {
  FLAGS_log_dir = "./log";
  FLAGS_minloglevel = 0;
  FLAGS_max_log_size = 1800;
  FLAGS_logbufsecs = 0;
  FLAGS_alsologtostderr = true;
  ::google::InitGoogleLogging("kv_server");
}

int main(int argc, char **argv) {
  Cluster_Epoll b;
  EncodeFix c;
  Coon d;
  std::vector<std::string> v;
  char line[MAXLINE];
  ssize_t n;
  int listenfd, connfd, port, i, sum, fd, size; //侦听描述符，读写描述符，端口
  socklen_t clientlen; //套接字长度
  struct sockaddr_in clientaddr; //套接字结构(用于存放套字节地址)
  bool flag;

  /* Glog init */
  ServerGlogInit();
  if (argc != 2) {
    LOG(ERROR) << "usage: " << argv[0] << " <port>";
	exit(-1);
  }

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
  port = atoi(argv[1]); // 端口号
  listenfd = Open_listenfd(port); // 服务器创建一个监听描述符，准备好接受连接请求
  b.Epoll_Init(listenfd);
  while (1) {
    int f = c.JudgeShutdown(line);
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
        LOG(INFO) << "accapt new connection from: " << str;
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
          v = d.NormalFinterpreter(line);
          size = d.GetRequest(v, line);
          // size = echo(line);
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

  StorageEngine::GetCurrent()->Close();
  LOG(INFO) << "Close StorageEngine...";
  return 0;
}
