#ifndef __DOWNLOAD_WORK_H__
#define __DOWNLOAD_WORK_H__

#include "normal.h"
#include "sql_oper.h"
#include "file_work.h"

void download_file(int new_fd ,const char * work_dir, const char *filename);

#endif