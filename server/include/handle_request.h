#ifndef __HANDLE_REQUEST_H__
#define __HANDLE_REQUEST_H__
#include "work_que.h"
#include "normal.h"
#include "config_init.h"
#include "download_work.h"
#include "upload_work.h"

void print_ls(const char *path, char *reply);
void handle_request(pNode pthd);
#endif
