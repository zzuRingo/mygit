#ifndef __NORMAL_H__
#define __NORMAL_H__
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>


#include "protocals.h"
#define FILENAME "file"
typedef struct tdata{
	int len;//代表后面真实数据长度
	char buf[1000];//火车车厢
}td,*ptd;

typedef struct reply_massage{
	char code[10];
	long start_pos;
}rpymsg, *prpymsg;

int send_n(int fd,char *buf,int len);
int recv_n(int fd,char *buf,int len);
int send_msg(int new_fd, char *store, int len);
int recv_msg(int new_fd, char *store);
#endif
