#include "csapp.h"

int echo(int connfd);

int main(int argc, char **argv) {
   int listenfd, connfd, port;  //侦听描述符，读写描述符，端口
   socklen_t clientlen; //套接字长度
   struct sockaddr_in clientaddr; //套接字结构(用于存放套字节地址)
   struct hostent *hp; // 主机名
   char *haddrp; // ip地址
  
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }
  /*listenfd和connfd的区别:监听描述符是作为客户端连接请求的一个端点.它通常被创建一次，并存在于服务器的整个生命周期.已连接描述符是
    客户端和服务器之间已经建立起来了的连接的一个端点.服务器每次接受连接请求时都会被创建一次，它只存在于服务器为一个客户端服务的过
    程中*/
  port = atoi(argv[1]); // 端口号
  listenfd = Open_listenfd(port); // 服务器创建一个监听描述符，准备好接受连接请求
  while (1) {
    clientlen = sizeof(struct sockaddr_storage); // 套字节长度
    /*1.服务器阻塞在Accept,等待监听描述符listenfd上的连接请求
      2.客户端通过调用和阻塞在connect,创建连接请求
      3.服务器从accept返回connfd.客户端从connect返回.现在在clientfd和coonfd之间已经建立起了连接*/
    connfd = Accept(listenfd, (SA *)&clientaddr,&clientlen);   //等待来自客户端的连接请求到达侦听描述符listenfd,然后在addr中填写客户端的套接字地址     返回一个已连接的描述符,这个描述符可被用来利用UnixI/O函数与客户端通信
    /* Determine the domain name and IP address of the client */
    hp = Gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET); //将套接字地址结构转换成主机和服务串
    haddrp = inet_ntoa(clientaddr.sin_addr);  //sin_adder成员是一个32位IP地址
    printf("server connected to %s (%s) \n", hp->h_name, haddrp); // server connected to VM-0-2-centos (127.0.0.1)
    int flag = echo(connfd);
    if (flag) {
      break;
    }
    Close(connfd);
  }
  exit(0);
}
