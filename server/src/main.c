#include "factory.h"
#include "log_and_sign.h"
int main()
{
	conf c;
	pconf pc = &c;
	memset(&c, 0, sizeof(c));
	config_init(pc);

	fac f;
	memset(&f,0,sizeof(f));
	f.tnum = pc->threadnum;
	f.capibility = pc->capability;
	factory_init(&f, threadfunc);//数据初始化
	factroy_start(&f);

	int sfd;
	set_init(&sfd ,c.ip, c.port, f.capibility);//bind IP地址及端口号
	
	int new_fd;
	while(1)
	{
		printf("ready to accept\n");
		new_fd = accept(sfd,NULL,NULL);
		printf("new_fd:%d in\n", new_fd);
		char path[100] = {0};
		if(enter_process(new_fd, c.work_dir, path) == -1){
			continue;
		}

		que_add(&f.que, new_fd, path);
		pthread_cond_signal(&f.cond);
	}	
	return 0;
}
