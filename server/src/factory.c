#include "factory.h"
void* threadfunc(void* p)
{
	pfac pf=(pfac)p;
	pque pq=&pf->que;
	pNode pn;//指向new_fd所存储的节点
	while(1)
	{
		pthread_mutex_lock(&pq->mutex);
		if(0==pq->size)
		{
			pthread_cond_wait(&pf->cond,&pq->mutex);//一旦队列为空，就睡觉
		}
		que_get(pq,&pn);
		pthread_mutex_unlock(&pq->mutex);
		handle_request(pn);
		free(pn);//不要把free放在锁之间,malloc和free函数耗时较久
	}
}
void factory_init(pfac p,pthfunc entry)
{
	p->pth=(pthread_t*)calloc(p->tnum,sizeof(pthread_t));
	p->entry=entry;//线程入口函数初始化
	pthread_cond_init(&p->cond,NULL);
	que_init(&p->que);
	p->start=0;
}

void factroy_start(pfac p)
{
	int i;
	if(0==p->start)
	{
		for(i=0;i<p->tnum;i++)
		{
			pthread_create(&p->pth[i],NULL,p->entry,p);
		}
		p->start=1;
	}else{
		printf("factory has started\n");
	}
}
