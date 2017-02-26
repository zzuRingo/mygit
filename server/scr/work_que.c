#include "work_que.h"

void que_init(pque p)
{
	memset(p,0,sizeof(que_t));
	pthread_mutex_init(&p->mutex,NULL);
}

void que_add(pque p,int new_fd, char *path)
{
	pNode pnew=(pNode)calloc(1,sizeof(Node));	
	pnew->new_fd=new_fd;
	sprintf(pnew->massage.work_dir, "%s", path);
	sprintf(pnew->massage.home, "%s", path);
	pnew->massage.len_init = strlen(pnew->massage.home);
	pthread_mutex_lock(&p->mutex);
	if(NULL==p->ptail)
	{
		p->phead=pnew;
		p->ptail=pnew;
	}else{
		p->ptail->pnext=pnew;
		p->ptail=pnew;
	}
	(p->size)++;
	pthread_mutex_unlock(&p->mutex);
}


void que_get(pque p,pNode* pn)
{

	*pn=p->phead;
	if(p->size == 1){
		p->phead = NULL;
		p->ptail = NULL;
	}	
	else
		p->phead=p->phead->pnext;
	(p->size)--;
}
