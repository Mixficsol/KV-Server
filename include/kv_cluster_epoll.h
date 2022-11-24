#ifndef __KV_CLUSTER_EPOLL_H_
#define __KV_CLUSTER_EPOLL_H_

class Cluster_Epoll {
 public:
  void Epoll_Init(int listenfd);
  void Set_Init(int connfd);
  void Set_Read(int sockfd);
  void Set_Write(int sockfd);
  void Set_Miss(int i);
  bool Judge_First(int i, int listenfd);
  bool Judge_Read(int i);
  bool Judge_Write(int i);
  int Get_Fd(int i);
  int Wait_Epoll();

 protected:
 
 private:
};
#endif
