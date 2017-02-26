#ifndef __UPLOAD_WORK_H__
#define __UPLOAD_WORK_H__

#include "normal.h"
#include "sql_oper.h"
#include "file_work.h"

typedef struct reply_massage{
	char code[10];
	long start_pos;
}rpymsg, *prpymsg;

void upload_file(int new_fd, const char *work_dir,const char *filename);


#endif
