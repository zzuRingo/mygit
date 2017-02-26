#include <sys/types.h>
 #include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

  int main(){
int fd = open("/mnt/hgfs/test/v3.0/server/home/zhang/mytest.txt", O_CREAT|O_RDWR, 0666);
if(fd == -1){
		printf("233\n");
		perror("open");
		return 0 ;
	}
}