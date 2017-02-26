#ifndef __PASSWD_H__
#define __PASSWD_H__

#define _XOPEN_SOURCE
#define _GNU_SOURCE     
#include <crypt.h>
#include "func.h"

void get_salt(char *salt);
void encrypt_psw(char *ecypsw, char *psw, char *salt);
#endif