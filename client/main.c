#include "func.h"
#include "passwd.h"
#include "md5.h"
#include <sys/types.h> 
#include <signal.h>

#define LOG_IN "0"
#define SIGN_IN "1"

#define PROSFIX ".msg"
#define CONTIN "0001"
#define STOPIT "0002"
#define FILE_EXIST "0003"

void send_stdin(int sfd, ptd ppack){
	read(STDIN_FILENO, ppack->buf, 1000);
	ppack->len = strlen(ppack->buf)-1;
	ppack->buf[ppack->len] = 0; 
	send_n(sfd ,(char*)ppack ,4+ppack->len);
}

int main(int argc,char* argv[])
{
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port=htons(2000);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");//IP地址转网络字节序
	int ret = -1;
	printf("连接中……\n");
	while(ret != 0){
		ret = connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
		if(-1==ret)
		{
			perror("connect");
			return -1;
		}
		
	}
	printf("连接成功！ ret = %d\n", ret);
	int len;
	td pack;
	memset(&pack, 0, sizeof(pack));

	while(1){
		printf("登录请输入0，注册请输入1\n");
		char is_sign[3] = {0};
		scanf("%s", is_sign);
		printf("233\n");
		if(send_msg(sfd, is_sign, 1) == -1)
			printf("error");

		printf("请输入用户名：\n");
		char name[20] = {0};
		read(STDIN_FILENO, name, 20);
		len = strlen(name);
		name[--len] = 0;
		send_msg(sfd, name, len);

		char psw[20] = {0};
		printf("请输入密码：\n");
		read(0, psw, 20);
		psw[strlen(psw)-1] = 0;

		char rep[3] = {0};
		if(!strcmp(SIGN_IN, is_sign)){	
			printf("this is sign!\n");
			char salt[20] = {0};
			get_salt(salt);
			char *enpsw = crypt(psw, salt);
			send_msg(sfd, enpsw, strlen(enpsw));
			send_msg(sfd, salt, strlen(salt));
			recv_msg(sfd, rep);
			if(rep[0] == '0'){
				fflush(stdout);
				printf("注册失败！\n");
				continue;
			}
			else{
				printf("注册成功！\n");
				break;
			}
		}
		else{
			printf("this is log!\n");
			recv_msg(sfd, rep);

			if(rep[0] == '0'){
				fflush(stdout);
				printf("该用户名不存在！\n");
				continue;
			}
			char salt[20] = {0};
			recv_msg(sfd, salt);
			char *enpsw = crypt(psw, salt);
			send_msg(sfd, enpsw, strlen(enpsw));
			memset(rep, 0, 3);
			recv_msg(sfd, rep);
			fflush(stdout);
			if(rep[0] == '0'){
				fflush(stdout);
				printf("密码错误！\n");
				continue;
			}
			else{
				fflush(stdout);
				printf("登录成功！\n");
				break;
			}
		}
		fflush(stdout);
	}

	char order[50] = {0};
	char legal[3];
	char recv_data[1000];
	int od_len = 0;
	size_t  readlen = 100;
	getchar();
	printf("请输入命令, 输入quit结束。\n");
	while(1){
		printf(">> ");
		memset(order, 0, 50);
		memset(recv_data, 0, 1000);
		gets(order);
		od_len = strlen(order);

		if(order[0] == 'q' && order[1] == 'u'){
			printf("退出成功！\n");
			break;
		}

		memset(legal, 0, sizeof(legal));
		
		printf("%s\n",order);
		if(order[0] == 'g'){ //下载
			int templen = strlen(order);
			while(order[templen-1] == '\n')
				order[--templen] = 0;
			send_msg(sfd, order, od_len);//传送命令
			recv_msg(sfd, recv_data);
			if(!strcmp(recv_data, STOPIT)){
				printf("该文件不存在！\n");
				continue;
			}else{
				printf("准备接收\n");
			}

			long start_pos = 0;
			memset(recv_data, 0, sizeof(recv_data));
			sprintf(recv_data ,"%ld", start_pos);
			send_msg(sfd, recv_data, strlen(recv_data));

			long total_len = 0;
			memset(recv_data, 0, sizeof(recv_data));
			recv_msg(sfd, recv_data);
			sscanf(recv_data, "%ld", &total_len);

			char filename[40] = {0};
			sprintf(filename, "%s", order+5);
			int recvfd;
			if((recvfd = open(filename, O_CREAT|O_RDWR, 0666)) == -1){
				perror("open");
				return 0;
			}
			lseek(recvfd, start_pos, SEEK_SET);

			long cur_len = 0;
			char buf[1000] = {0};
			int len = 0;
			while(cur_len < total_len){
				if(recv_n(sfd, (char*)&len ,4) < 0)
					printf("line:%d\n",__LINE__);
					memset(buf,0,sizeof(buf));
				if(len > 0){
					if(recv_n(sfd ,buf ,len) < 0){ 
						printf("line:%d\n", __LINE__);
						break;
					}
					write(recvfd ,buf ,len);
					cur_len += len;
					printf("len %d\n", cur_len);
				}else{
					break;

				}
			}
		}
	
		else if(order[0] == 'p' && order[1] == 'u'){ //上传
			char value[100] = {0};
			int oddlen = strlen(order);
			while(order[oddlen-1] == '\n')
				order[--oddlen] = 0;
			printf("%s", order);
			Compute_file_md5(order+5, value);
			od_len = strlen(order);
			int i;
			for(i = od_len -1; i>4; i--)
				if(order[i] == '/')
					break;
			char name[20] = {0};
			sprintf(name,"%s", order+i+1);
			printf("filename ::%s\n", name);

			memset(order, 0 , sizeof(order));
			sprintf(order,"puts %s", name);
			send_msg(sfd, order, strlen(order));

			send_msg(sfd, value, strlen(value));
			rpymsg recv_pack;
			memset(&recv_pack, 0, sizeof(recv_pack));
			recv_msg(sfd, (char *)&recv_pack);
			if(!strcmp(recv_pack.code, FILE_EXIST)){
				printf("秒传成功！\n");
				continue;
			}
			else if(!strcmp(recv_pack.code, CONTIN)){
				struct stat statbuff;  
			    stat(order+5, &statbuff);
				long filesize = statbuff.st_size;  
				char clen[50] = {0};
				sprintf(clen, "%ld %ld", filesize, filesize - recv_pack.start_pos);
				printf("len : %s\n", clen);
				send_msg(sfd, clen, strlen(clen));

				int fd = open(order+5, O_RDONLY);
				lseek(fd, recv_pack.start_pos, SEEK_SET);
				if(-1==fd)
				{
					perror("open");
					return 0;
				}

				td t;
				memset(&t,0,sizeof(t));
				while(memset(&t,0,sizeof(t)), (t.len=read(fd,t.buf,sizeof(t.buf)))>0){
					printf("len:%d\n",t.len);
					send_n(sfd,(char *)&t,4+t.len);
				}
				t.len=0;
				printf("finished!\n");
			}
			else{
				printf("文件重名！\n");
				continue;
			}
		}
		else if(strstr(order ,"remove")){
			printf("this is remove!\n");
			send_msg(sfd, order, od_len);

			if(!strcmp(recv_data, STOPIT)){
				printf("该文件不存在！\n");
				continue;
			}else{
				printf("准备接收\n");
			}

		}
		else{
			send_msg(sfd, order, od_len);//传送命令
			memset(recv_data, 0, sizeof(recv_data));
			recv_msg(sfd, recv_data);
			if(!strcmp(recv_data, STOPIT)){
				printf("文件不存在\n");
				continue;
			}else{
				printf("删除成功！\n");
			}
		}
	}
	close(sfd);
}
