#ifndef __FILE_WORK_H__
#define __FILE_WORK_H__

#include "normal.h"
#include "sql_oper.h"

#define CONTIN "0001"
#define STOPIT "0002"
#define FILE_EXIST "0003"

#define PROSFIX ".msg"

#define CACHESIZE 104857600

typedef struct file{
	char md5[100];
	char realroot[40];
	long file_size;
	long real_size;
}fmsg, *pfmsg;

#endif
