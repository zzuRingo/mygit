#include "handle_request.h"
void do_cd(pNode pthd, char *reply, char *order);
void handle_request(pNode pthd){
	char order[1000];
	char reply[1000];
	char path[1000];
	int new_fd = pthd->new_fd;
	int len_init = pthd ->massage.len_init;
	while(1){
		memset(order, 0, 1000);
		memset(reply, 0, 1000);
		memset(path, 0, 1000);
		if(recv_msg(new_fd, order)<0)
			return;
		printf("this is order: %s\n", order);
		if(strstr(order, "ls") == order){
			printf("this is ls!\n");
			printf("%s\n", pthd->massage.work_dir);
			print_ls( pthd->massage.work_dir, reply);
			printf("%s\n", reply);
			if(send_msg(new_fd, reply, strlen(reply)) < 0)
				return;
		}
		else if(strstr(order, "remove") == order){

		}
		else if(strstr(order, "pwd") == order){
			printf("this is pwd!\n");
			if(strlen(pthd->massage.work_dir) == pthd->massage.len_init)
				sprintf(reply, "/");
			else
				sprintf(reply, "%s", pthd->massage.work_dir + pthd->massage.len_init);
			printf("reply:%s\n", reply);
			send_msg(new_fd, reply, strlen(reply));
		}
		else if(strstr(order, "cd") == order){
			printf("this is cd!\n");
			do_cd(pthd,reply, order);
			if(send_msg(new_fd, reply, strlen(reply)) == -1)
				break;
		}else if(strstr(order, "puts") == order){
			char filename[40] ={0};
			int i;
			int ord_len = strlen(order)-1;
			for(i = ord_len; i>=6; i--)
				if(order[i] == '/') break;
			sprintf(filename, "%s\n", order + i);
			int templen = strlen(filename);
			while(filename[templen-1] == '\n')
				filename[--templen] = 0; 
			upload_file(new_fd,pthd->massage.work_dir, filename);
		}else if(strstr(order, "gets") == order){
			char filename[40] = {0};
			strcpy(filename, order+5);
			int templen = strlen(filename);
			while(filename[templen-1] == '\n')
				filename[--templen] = 0;
			download_file(new_fd,pthd->massage.work_dir, filename);
		}else{
			sprintf(reply, "wrong order!");
			send_msg(new_fd, reply, strlen(reply));
		}
	}

}

void do_cd(pNode pthd, char *reply, char *order){
	if(order[3] == '.' && order[4] == '.'){
		printf("cd ..!\n");
		int len = strlen(pthd->massage.work_dir);
		printf("work_dir:%s\n", pthd->massage.work_dir);
		printf("home_dir:%s\n", pthd->massage.home);
		printf("work_dir len :%d, len_init:%d\n",
			len, pthd->massage.len_init);
		if(pthd->massage.work_dir[len-1] == '/'){
			len--;
		}

		while(len >= 0 && pthd->massage.work_dir[--len]!='/');
		
		if(len < pthd->massage.len_init)
			sprintf(reply, "wrong order!");
		else if(len == pthd->massage.len_init){
			sprintf(reply, "successfully to /");
			memset(pthd->massage.work_dir, 0, sizeof(pthd->massage.work_dir));
			sprintf(pthd->massage.work_dir, "%s", pthd->massage.home);
		}
		else{
			int i,j;
			for(i = pthd->massage.len_init, j = 0; i<len; i++,j++)
				reply[j] = pthd->massage.work_dir[j];
			memset(pthd->massage.work_dir+i, 0, strlen(pthd->massage.work_dir) - pthd->massage.len_init);
			sprintf(reply, "successfully to %s!", order);
		}
	}
	else{
		printf("else!\n");
		printf("order : %s\n", order);
		if(order[3] == '/')
			printf(reply, "%s%s", pthd->massage.home,  order + 3);
		else{
			printf("this is child dir!\n");
			char root[1000] = {0};
			int len = strlen(pthd->massage.work_dir) - 1;
			if(pthd->massage.work_dir[len] == '/')
				sprintf(root, "%s%s", pthd->massage.work_dir, order+3);
			else
				sprintf(root, "%s/%s", pthd->massage.work_dir, order+3);
			printf("root:%s\n" , root);

			if(access(root, 0))
				sprintf(reply, "not such file!\n");
			else{
				memset(pthd->massage.work_dir, 0, 100);
				sprintf(pthd->massage.work_dir, "%s", root);
				sprintf(reply, "successfully to %s!", order);
			}
		}
	}
	printf("cur work dir :%s\n", pthd->massage.work_dir);
}