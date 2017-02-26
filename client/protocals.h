#ifndef __PROTOCALS_H__
#define __PROTOCALS_H__

typedef struct logandsign{
	int is_sign;
	char name[12];
}las, *plas;

typedef struct sign{
	char salt[16];
	char encodepsw[124];
}sig, *psig;


#endif