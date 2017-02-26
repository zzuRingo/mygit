#include "config_init.h"

void config_init(pconf p){
	FILE* fp;
	if((fp = fopen(CONF_FILE, "r")) == NULL){
		printf("file:%s line:%d\n",__FILE__,__LINE__);
		perror("fopen");
		return;
	}

	char buf[128];
	int n = 0;
	char temp[64];
	while(memset(buf, 0, sizeof(buf)) ,fgets(buf, 128, fp)){
		if(n == 0){
			sscanf(buf, "%*s%s", p->ip);
			printf("ip:%s\n",p->ip);
		}else if(n == 1){
			sscanf(buf, "%*s%d", &p->port);
			printf("port:%d\n",p->port);
		}else if(n == 2){
			sscanf(buf, "%*s%d", &p->threadnum);
			printf("threadnum:%d\n",p->threadnum);
		}else if(n == 3){
			sscanf(buf, "%*s%d", &p->capability);
			printf("capability:%d\n",p->capability);
		}else if(n == 4){
			sscanf(buf, "%*s%s", p->work_dir);
			printf("work_dir:%s\n",p->work_dir);
		}
		n++;
	}
	return;

}