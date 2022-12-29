#ifndef __KV_CLUSTER_EPOLL_H_
#define __KV_CLUSTER_EPOLL_H_

class Cluster_Epoll {
 public:
  Cluster_Epoll();
  ~Cluster_Epoll();
  
  static void Init();
  static Cluster_Epoll* GetCurrent();

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
  static Cluster_Epoll* cluster_epoll_;

  Cluster_Epoll(Cluster_Epoll& se);
  void operator =(const Cluster_Epoll& se);
};

#endif  // __KV_CLUSTER_EPOLL_H_
