#ifndef __SOCKET_H
#define __SOCKET_H

#define IPADDR "192.168.31.134"
#define IPPORT "8197" //不用8080，摄像头服务已经占用
#define BUF_SIZE 6

#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
//#include <linux/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int socket_init(const char *ipaddr, const char *ipport);

#endif

