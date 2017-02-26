#ifndef __LOGIN_QUE_H__
#define __LOGIN_QUE_H__

typedef struct que_node{
	int fail_times; //登录失败的次数
	int new_fd; //对应的socket;
	struct que_node *pnext;
}lqnode,*plqnode;

typedef struct log_que{
	plqnode log_que;
	plqnode phead;
	plqnode ptail;
	int que_len;
}lgque,plgque;

short queue_empty(lgque queue);


#endif