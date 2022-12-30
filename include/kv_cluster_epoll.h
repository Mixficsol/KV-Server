#ifndef __KV_CLUSTER_EPOLL_H_
#define __KV_CLUSTER_EPOLL_H_

class Cluster_Epoll {
 public:
  Cluster_Epoll();
  ~Cluster_Epoll();
  
  static void Init();
  static Cluster_Epoll* GetCurrent();

  static void Epoll_Init(int listenfd);
  static void Set_Init(int connfd);
  static void Set_Read(int sockfd);
  static void Set_Write(int sockfd);
  static void Set_Miss(int i);
  static bool Judge_First(int i, int listenfd);
  static bool Judge_Read(int i);
  static bool Judge_Write(int i);
  static int Get_Fd(int i);
  static int Wait_Epoll();

 protected:
 
 private:
  static Cluster_Epoll* cluster_epoll_;

  Cluster_Epoll(Cluster_Epoll& se);
  void operator =(const Cluster_Epoll& se);
};

#endif  // __KV_CLUSTER_EPOLL_H_
