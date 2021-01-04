#pragma once
#ifndef GESTIONE_PASSWORD_FLAG
#define GESTIONE_PASSWORD_FLAG

#include <mariadb/mysql.h>
#include "../../Config_files/Database.conf.h"

class ges_pwd 
{
public:
	ges_pwd();
	int login(const char* username, const char* pwd);
	int create_account(const char* mail, const char* username, const char* pwd );
	int logout();
	int insert_element(const char* service, const char* account, const char* pwd);;
	int search_element(const char* service, char flag, int flag2);
	int print_elements(char flag='N');
	int remove_element(const char* service, char flag);
	const char* get_user_mail();
	const char* UserID;
private:
	MYSQL* conn;
	int initialize_current_data();
	int print_result(const char* IN_query);
	void print_line(int dim);
	void print_value(const char* value, int dim);

};

#endif //GESTIONE_PASSWORD_FLAG
