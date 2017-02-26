#ifndef __CONFIG_INIT_H__
#define __CONFIG_INIT_H__
#include "normal.h"

#define CONF_FILE "/mnt/hgfs/test/v3.0/server/config/config"
typedef struct config{
	char ip[12];
	int port;
	int threadnum;
	int capability;
	char work_dir[40];
}conf, *pconf;

void config_init(pconf p);
#endif