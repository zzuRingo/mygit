#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "normal.h"
//队列节点
typedef struct masge{
	char work_dir[100];
	char home[100];
	int len_init;
}msg,* pmsg;

typedef struct list{
    int new_fd;
    msg massage;
    struct list *pnext;
}Node,*pNode;

//队列结构体
typedef struct work_que{
    pNode phead,ptail;
    pthread_mutex_t mutex;
    int size;//标示队列实时的长度
}que_t,*pque;

void que_init(pque p);
void que_add(pque p,int new_fd, char *path);
void que_get(pque p, pNode* pn);
#endif
