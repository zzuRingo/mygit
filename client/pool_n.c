#include "func.h"

int send_n(int fd,char *buf,int len)
{
	int total=0;
	int pos;
	while(total<len)
	{
		pos = send(fd,buf+total,len-total,0);
		if(pos == -1){
			printf("fd:%d lost connection\n", fd);
			return -1;
		}
		total=total+pos;	
	}
	return 0;
}

int recv_n(int fd,char *buf,int len)
{

	int total=0;
	int pos;
	while(total<len)
	{
		pos=recv(fd,buf+total,len-total,0);
		if(pos == 0){
			printf("fd:%d lost connection\n",fd);
			return -1;
		}
		total=total+pos;
	}
	return 0;
}

int send_msg(int new_fd, char *store, int len){
	td pack;
	memset(&pack, 0, sizeof(pack));
	pack.len = len;
	memcpy(pack.buf ,store, len);
	if(send_n(new_fd, (char*)&pack, len+4) == -1)
		return -1;
	return 0;
}

int recv_msg(int new_fd, char *store){
	int packlen;
	if(recv_n(new_fd, (char *)&packlen, 4) == -1)
		return -1;
	if(recv_n(new_fd, store, packlen) == -1)
		return -1;
	return 0;
}

