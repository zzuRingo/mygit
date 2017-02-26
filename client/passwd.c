#include "passwd.h"
void get_salt(char *salt){
	srand((unsigned) time(NULL));
	char number; 
	int choice;
	salt[0] = '$';
	salt[1] = '6';
	salt[2] = '$';
	for(int i = 3; i<16; i++){
		choice = rand() % 3;
		if(choice == 0)
    		salt[i] =  rand()%('9'-'0'+ 1) + '0';
    	else if(choice == 1){
    		salt[i] =  rand()%('Z'-'A'+ 1) + 'A';
    	}
    	else if(choice == 2){
    		salt[i] =  rand()%('z'-'a'+ 1) + 'a';
    	}
   	}
   	return;   
}

void encrypt_psw(char *ecypsw, char *psw, char *salt){
	sprintf(ecypsw, "%s",crypt(psw, salt));
	return;
}
