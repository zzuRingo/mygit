#ifndef __SQL_OPER_H__
#define __SQL_OPER_H__
#include <mysql/mysql.h>
#include "normal.h"

void connect_to_sql();
void store_user(char *name, char *salt, char *enpsw);
short name_exist();
void get_psw_salt(char *name, char *salt, char *enpsw);
short file_exist(char *md5);
void file_num_add(const char *md5);
void get_real_path(char *realroot, const char *md5);
void add_md5(const char *md5, const char *root);
#endif

