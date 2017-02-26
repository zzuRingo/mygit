#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "work_que.h"
#include "normal.h"
#include "handle_request.h"
#include "config_init.h"

typedef void* (*pthfunc)(void*);

typedef struct  factory{
    pthread_t *pth;
    int tnum;//创建的线程数目
    pthfunc entry;//线程入口函数
    pthread_cond_t cond;
    int capibility;//能力
    que_t que;
    int start;
}fac,*pfac;

void set_init(int* psfd, char* ip, int port, int capibility);
void send_file(int);
void factory_init(pfac p,pthfunc entry);
void factroy_start(pfac p);
void* threadfunc(void* p);
#endif
