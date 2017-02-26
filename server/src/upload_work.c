#include "upload_work.h"
//void get_cache();
void recv_file(pfmsg p, int new_fd, int fd){
	char buf[1000] = {0};
	recv_msg(new_fd, buf);//文件大小，断点位置
	long file_len;
	sscanf(buf,"%ld %ld", &p->file_size, &file_len);
	memset(buf, 0, sizeof(buf));
	printf("len : %ld\n", file_len);
	p->file_size = file_len;

	long recv_len = 0;
	int len;/*
	if (file_len > CACHESIZE){
	//	get_cache();
		;
	}
	else{*/
	while(p->real_size < p->file_size){
		if(recv_n(new_fd, (char*)&len ,4) < 0)
			printf("line:%d\n",__LINE__);
		memset(buf,0,sizeof(buf));
		if(len > 0){
			if(recv_n(new_fd ,buf ,len) < 0){ 
				printf("line:%d\n", __LINE__);
				break;
			}
			write(fd ,buf ,len);
			p->real_size += len;
			printf("len %ld\n", p->real_size);
		}else{
			break;
		}
	}
	p->real_size = recv_len;
}
void write_msg_to_file(pfmsg pmsg_pack, const char *filemsg_pth)
{	
	int fd = open(filemsg_pth, O_CREAT|O_RDWR, 0666);
	printf("%s\n",filemsg_pth);
	printf("%s\n",filemsg_pth);
	if(fd == -1){
		perror("open");
		return;
	}
	ftruncate(fd,0);
	write(fd, pmsg_pack, sizeof(*pmsg_pack));
	printf("索引创建成功！！！！！\n");
	return;
}
void get_real_msg(pfmsg p, const char *filename){
	int fd = open(filename, O_RDWR);

	printf("func:%s  %s\n",__func__,filename);
	if(fd == -1){
		perror("open");
		return;
	}

	read(fd, p, sizeof(*p));
	return;
}
void upload_file(int new_fd, const char *work_dir, const char *filename){
	char filemsg[40] = {0};
	sprintf(filemsg, "%s%s", filename, PROSFIX);
	char filemsg_pth[80] = {0};
	sprintf(filemsg_pth, "%s/%s", work_dir, filemsg);
	char fileroot[80] = {0};
	sprintf(fileroot, "%s/%s", work_dir, filename);

	fmsg msg_pack;
	fmsg recv_pack;
	fmsg real_msg_pack;
	memset(&msg_pack, 0, sizeof(msg_pack));
	memset(&recv_pack, 0, sizeof(recv_pack));
	memset(&real_msg_pack, 0, sizeof(real_msg_pack));
	char reply[20] = {0};
	long start_pos = 0;
	memset(reply, 0, 20);
	recv_msg(new_fd, recv_pack.md5);//md5码

	int msgfd = open(filemsg_pth, O_RDWR);
	rpymsg reply_pack;
	memset(&reply_pack, 0, sizeof(reply_pack));
	char real_msg_root[60] = {0};
	if(msgfd == -1){ //不存在文件	
		printf("checking md5\n");
			//看看是否存在别处
		if(file_exist(recv_pack.md5)){
			printf("you le yi jing!\n");
			printf("line :%d\n",__LINE__);
			file_num_add(recv_pack.md5);
			get_real_path(recv_pack.realroot, recv_pack.md5);

			sprintf(real_msg_root,"%s%s",recv_pack.realroot, PROSFIX);
			get_real_msg(&real_msg_pack, real_msg_root);//ok
			if(real_msg_pack.file_size == real_msg_pack.real_size)
			{
				printf("line :%d\n",__LINE__);
				printf("real_msg_pack.file_size :%ld real_msg_pack.real_size:%ld",real_msg_pack.file_size,real_msg_pack.real_size);
				sprintf(reply_pack.code, "%s", FILE_EXIST);
				printf("miaochuan!\n");
				if(send_msg(new_fd, (char *)&reply_pack, sizeof(reply_pack)) == -1)
					return;
				return;
			}
			else{
				printf("文件需续传\n");
				printf("line :%d\n",__LINE__);
				sprintf(reply_pack.code, "%s", CONTIN);
				sprintf(real_msg_pack.md5, "%s", recv_pack.md5);
				reply_pack.start_pos = real_msg_pack.real_size;
				if(send_msg(new_fd, (char *)&reply_pack, sizeof(reply_pack)) == -1)
					return;
			}
		}else{//文件哪都没有
			printf("哪都没有\n");
			printf("line :%d\n",__LINE__);
			add_md5(recv_pack.md5, fileroot);
			sprintf(reply_pack.code, "%s", CONTIN);
			reply_pack.start_pos = 0;
			if(send_msg(new_fd, (char *)&reply_pack, sizeof(reply_pack)) == -1)
				return;

			sprintf(real_msg_pack.realroot, "%s",fileroot);
			sprintf(real_msg_pack.md5, "%s", recv_pack.md5);
			real_msg_pack.file_size = 0;
			real_msg_pack.real_size = 0;
		}
	}
	else{//存在文件
		//就是这个文件
		printf("line :%d\n",__LINE__);
		read(msgfd, &msg_pack, sizeof(msg_pack));
		if(!strcmp(msg_pack.md5, recv_pack.md5)){
			printf("line :%d\n",__LINE__);
			printf("文件已存在\n");
			get_real_path(msg_pack.realroot, msg_pack.md5);
			sprintf(real_msg_root,"%s%s",msg_pack.realroot, PROSFIX);
			get_real_msg(&real_msg_pack, real_msg_root);//ok

			if(real_msg_pack.file_size == real_msg_pack.real_size)
			{
				printf("line :%d\n",__LINE__);
				sprintf(reply_pack.code, "%s", FILE_EXIST);
				printf("miaochuan!\n");
				if(send_msg(new_fd, (char *)&reply_pack, sizeof(reply_pack)) == -1)
					return;
				return;
			}
			else{
				printf("文件需续传\n");
				printf("line :%d\n",__LINE__);
				sprintf(reply_pack.code, "%s", CONTIN);
				reply_pack.start_pos = real_msg_pack.real_size;
				if(send_msg(new_fd, (char *)&reply_pack, sizeof(reply_pack)) == -1)
					return;
				sprintf(real_msg_pack.realroot, "%s",fileroot);
				sprintf(real_msg_pack.md5, "%s", msg_pack.md5);
			}
		}else{//同名而已
			printf("line :%d\n",__LINE__);
			printf("文件重名，请重命名\n");
			sprintf(reply_pack.code, "%s", STOPIT);
			if(send_msg(new_fd, (char *)&reply_pack, sizeof(reply_pack)) == -1)
				return;
			return;
		}
	}

	int len = strlen(real_msg_pack.realroot);
	while(real_msg_pack.realroot[--len] == '\n')
		real_msg_pack.realroot[len] = 0;
	int fd = open(real_msg_pack.realroot, O_CREAT|O_RDWR, 0666);

	printf("%s",real_msg_pack.realroot);
	if(fd == -1){
		printf("233\n");
		perror("open");
		return;
	}
	lseek(fd, real_msg_pack.real_size, SEEK_SET);
	printf("line :%d\n",__LINE__);
	recv_file(&real_msg_pack, new_fd, fd);
	close(fd);
	write_msg_to_file(&real_msg_pack, filemsg_pth);
	printf("finished!\n");
}


