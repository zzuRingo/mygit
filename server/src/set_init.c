#include "normal.h"

void set_init(int* psfd, char* ip, int port, int capibility)
{
	*psfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1 == *psfd)
	{
		perror("socket");
		exit(-1);
	}

	int sfd = *psfd;
	int on=1;  
    if((setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }

	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(port);//port转网络字节序
	ser.sin_addr.s_addr=inet_addr(ip);//IP地址转网络字节序
	int ret;
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));//绑定ip地址和端口号
	if(-1==ret)
	{
		perror("bind");
		return; 
	}
	listen(sfd ,capibility);
	printf("successful build!\n");
}
