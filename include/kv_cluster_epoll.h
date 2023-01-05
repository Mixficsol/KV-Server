#ifndef __KV_CLUSTER_EPOLL_H_
#define __KV_CLUSTER_EPOLL_H_

class Cluster_Epoll {
 public:
  static void Epoll_Init(int listenfd);
  static void Set_Init(int connfd);
  static void Set_Read(int sockfd);
  static void Set_Write(int sockfd);
  static void Set_Miss(int i);
  static bool Judge_First(int i, int listenfd);
  static bool Judge_Read(int i);
  static bool Judge_Write(int i);
  static int GetFD(int i);
  static int Wait_Epoll();

 protected:
 
 private:

};

#endif  // __KV_CLUSTER_EPOLL_H_
