#include "download_work.h"
void send_file(int new_fd, int tar_fd, long start_pos, const char *filename)
{
	struct stat statbuff;  
	stat(filename, &statbuff);
	long filesize = statbuff.st_size;

	char report[20] = {0};
	sprintf(report, "%ld", filesize - start_pos);
	send_msg(new_fd, report, strlen(report));

	lseek(tar_fd, start_pos, SEEK_SET);

	td t;
	memset(&t,0,sizeof(t));
	while(memset(&t, 0, sizeof(t)), (t.len = read(tar_fd, t.buf ,sizeof(t.buf)))>0){
		printf("len:%d\n", t.len);
		send_n(new_fd,(char *)&t,4+t.len);
	}
	printf("finished!\n");
}

void download_file(int new_fd ,const char * work_dir, const char *filename)
{
	char file_msg_name[100] = {0};
	char report[20] = {0};

	sprintf(file_msg_name, "%s/%s%s", work_dir, filename, PROSFIX);
	printf("%s\n", file_msg_name);
	if(access(file_msg_name, 0)){
		sprintf(report, "%s", STOPIT);
	}
	else
		sprintf(report, "%s", CONTIN);

	send_msg(new_fd, report, strlen(report));
	long start_pos;
	memset(report, 0, 20);
	recv_msg(new_fd, report);
	sscanf(report, "%ld", &start_pos);

	fmsg buf;
	memset(&buf, 0, sizeof(buf));
	int fd;
	if((fd = open(file_msg_name, O_RDWR)) == -1)
	{
		perror("open");
		printf("line ：%d\n", __LINE__);
		return;
	}
	read(fd, &buf, sizeof(buf));

	int tar_fd;
	if((tar_fd = open(buf.realroot,  O_RDONLY)) == -1)
	{
		perror("open");
		printf("line ：%d\n", __LINE__);
		return;
	}

	send_file(new_fd, tar_fd, start_pos, buf.realroot);

 }