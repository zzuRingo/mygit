#ifndef __LOG_AND_SIGN_H__
#define __LOG_AND_SIGN_H__
#include "work_que.h"
#include "sql_oper.h"
#include "normal.h"
#include "protocals.h"

#define LOG_IN "0"
#define SIGN_IN "1"
#define NAME_EXIST "1"
#define NAME_NOT_EXIST "0"

int enter_process(int new_fd, char *work_dir, char *path);
#endif