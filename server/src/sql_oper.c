#include "sql_oper.h"
void connect_to_sql(MYSQL **pconn){
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="user";

	*pconn = mysql_init(NULL);
	if(!mysql_real_connect(*pconn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(*pconn));
	}else{
		printf("Connected...\n");
	}
}
void store_user(char *name, char *salt, char *enpsw){
	MYSQL *conn = NULL;
	connect_to_sql(&conn);
	char query[200] = {0};
	sprintf(query,"insert into user values('%s','%s','%s')",name,salt,enpsw);

	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	t = mysql_query(conn, query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}
	mysql_close(conn);
	return;
}
short name_exist(char *usrname){
	MYSQL *conn = NULL;
	connect_to_sql(&conn);
	char query[200] = {0};
	sprintf(query,"select * from user where username = '%s'",usrname);
	printf("%s\n",query);

	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	int ret = 1;
	t = mysql_query(conn, query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("Query made...\n");
		res = mysql_use_result(conn);
		if(res)
		{
			if((row=mysql_fetch_row(res))==NULL){	
				ret = 0;
			}
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return ret;
}

void get_psw_salt(char *name, char *salt, char *enpsw){
	MYSQL *conn = NULL;
	connect_to_sql(&conn);
	char query[200] = {0};
	sprintf(query,"select encrypsw,salt from user where username = '%s'",name);


	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	t = mysql_query(conn ,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("Query made...\n");
		res = mysql_use_result(conn);
		if(res)
		{
			row = mysql_fetch_row(res);	
			sprintf(enpsw,"%s",row[0]);
			sprintf(salt,"%s",row[1]);
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return;
}

short file_exist(char *md5){
	MYSQL *conn = NULL;
	connect_to_sql(&conn);
	char query[200] = {0};
	sprintf(query,"select * from md5 where md5 = '%s'",md5);
	printf("%s\n",query);
	printf("%s\n",query);

	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	int ret = 1;
	t = mysql_query(conn, query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			if((row=mysql_fetch_row(res))==NULL){	
				ret = 0;
			}
		}
		mysql_free_result(res);
	}
	return ret;
}


void file_num_add(const char *md5){
	MYSQL *conn = NULL;
	connect_to_sql(&conn);
	char query[200] = {0};
	sprintf(query ,"update md5 set times = times + 1 where md5 = %s", md5);printf("%s\n",query);

	MYSQL_RES *res;
	MYSQL_ROW row;
	int t,r;
	int ret = 1;
	t = mysql_query(conn, query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}
	return;
}

void get_real_path(char *realroot, const char *md5)
{
	MYSQL *conn = NULL;
	connect_to_sql(&conn);
	char query[150] = {0};
	sprintf(query,"select root from md5 where md5 = '%s'", md5);

	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	t = mysql_query(conn ,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("Query made...\n");
		res = mysql_use_result(conn);
		if(res)
		{
			row = mysql_fetch_row(res);	
			sprintf(realroot,"%s",row[0]);
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return;

}

void add_md5(const char *md5, const char *root){
	MYSQL *conn = NULL;
	connect_to_sql(&conn);
	char query[150] = {0};
	sprintf(query,"insert into md5 values('%s','%s','0')",md5, root);
	printf("%s\n",query);

	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	t = mysql_query(conn, query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}
	mysql_close(conn);
	return;
}
