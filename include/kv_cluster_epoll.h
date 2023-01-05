#ifndef __KV_CLUSTER_EPOLL_H_
#define __KV_CLUSTER_EPOLL_H_

class ClusterEpoll {
 public:
  static void EpollInit(int listenfd);
  static void SetInit(int connfd);
  static void SetRead(int sockfd);
  static void SetWrite(int sockfd);
  static void SetMiss(int i);
  static bool JudgeFirst(int i, int listenfd);
  static bool JudgeRead(int i);
  static bool JudgeWrite(int i);
  static int GetFD(int i);
  static int WaitEpoll();

 protected:
 
 private:

};

#endif  // __KV_CLUSTER_EPOLL_H_
