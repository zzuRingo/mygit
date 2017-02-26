#include "log_and_sign.h"
void creat_person_file(char *name, char *work_dir, char *path){
	sprintf(path, "%s/%s", work_dir, name);
	mkdir(path, 0666);
}
int sign_in(int new_fd, char *work_dir, char *name, char *path){
	printf("its sign_in!\n");

	char enpsw[100] = {0};
	char salt[20] = {0};
	recv_msg(new_fd, enpsw);
	recv_msg(new_fd, salt);
	//判断用户名是否已存在
	char msg[3] = {0};
	int exist = name_exist(name);
	if(exist){
		sprintf(msg, "0");
		if(send_msg(new_fd, msg, 1) == -1) //传递失败
			return -1;
		enter_process(new_fd, work_dir, path);
		return 0;
	}

	sprintf(msg, "1");
	if(send_msg(new_fd, msg, 1) == -1) 
		return -1;
	printf("you can use this name\n");
	store_user(name, salt, enpsw);
	creat_person_file(name, work_dir, path);
	return 0;
}
int log_in(int new_fd, char *work_dir, char *name, char *path){
	printf("its log_in!\n");
	int exist = name_exist(name);
	if(!exist){
		printf("doesnt exist!!!!!!!!\n");
		send_msg(new_fd, "0", 1);
		if(enter_process(new_fd, work_dir, path) == -1)
			return -1;
		return 0;
	}
	send_msg(new_fd, "1", 2);
	char salt[20] = {0};
	char enpsw[120] = {0};
	char enpsw_c[120] = {0};
	get_psw_salt(name, salt, enpsw);
	send_msg(new_fd, salt, strlen(salt));
//收到加密密码作对比
	recv_msg(new_fd, enpsw_c);
	if(!strstr(enpsw_c, enpsw)){
		send_msg(new_fd, "0", 2);
		printf("noooo way!\n");
		if(enter_process(new_fd, work_dir, path) == -1)
			return -1;
		return 0;
	}
	send_msg(new_fd, "1", 2);
	printf("send!\n");
	//改变工作目录
	sprintf(path, "%s/%s", work_dir, name);
	return 0;
}
int enter_process(int new_fd, char *work_dir, char *path){
	td pack;
	memset(&pack, 0, sizeof(pack));

	char is_sign[3] = {0};
	recv_msg(new_fd, is_sign);
	printf("%s\n", is_sign);

	char name[20] = {0};
	recv_msg(new_fd, name);
	printf("%s\n", name);

	if(!strcmp(LOG_IN, is_sign)){
		if(log_in(new_fd, work_dir, name, path) == -1)
			return -1;
	}
	else{
		if(sign_in(new_fd, work_dir, name, path) == -1)
			return -1;			
	}
	return 0;
}