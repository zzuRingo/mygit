#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include "handle_request.h"

void idToName(uid_t uid, char *reply){
	struct passwd *pwd= getpwuid(uid);
	strcat(reply, " ");
	strcat(reply, pwd->pw_name);
}
void printPmt(struct stat buf, char *reply){
	if ((buf.st_mode & S_IFMT) == S_IFDIR)
		strcat(reply, "d");
	else
		strcat(reply, "-");

	unsigned int n = 1;
	n <<= 8;
	for(int i = 1; i<= 9;i++){
		if(n & buf.st_mode){
			if(!(i%2))
				strcat(reply, "w");
			else if(!(i%3))
				strcat(reply, "x");
			else
				strcat(reply, "r");
		}
		else
			strcat(reply, "-");
		n>>=1;
	}
	strcat(reply, " ");

}
void print_ls(const char *path, char *reply){
	DIR *dir = NULL;
	if((dir = opendir(path)) == NULL){
		perror("opendir");
		return;
	}

	struct dirent *p;
	char cur_file[1024] = {0};
	struct  stat buf;
	while((p = readdir(dir))!=NULL){
		if((!strcmp(p->d_name, ".")) || (!strcmp(p->d_name,".."))){
			continue;
		}

		memset(cur_file, 0, 1024);
		sprintf(cur_file, "%s/%s", path, p->d_name);
		stat(cur_file, &buf);
		printPmt(buf, reply);
		idToName(buf.st_uid, reply);
		idToName(buf.st_uid, reply);

		time_t ti = buf.st_mtime;
		char buf_time[64] = {0};
		sprintf(buf_time,"%s",ctime(&ti));
		buf_time[16] = 0;
		strcat(reply, buf_time);
		strcat(reply, " ");

		strcat(reply, p->d_name);
		strcat(reply, "\n");
	}
	closedir(dir);
}