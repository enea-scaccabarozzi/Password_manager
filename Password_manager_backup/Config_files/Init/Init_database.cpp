#include <mariadb/mysql.h>
#include <iostream>
#include "../Database.conf.h"
MYSQL* connection;
int main(){
    connection = mysql_init(NULL);
	if (connection == NULL)
	{
		std::cout << "[!!] Error during the initialization of sql server in the. " << std::endl;
		exit(-1);
		std::cin.get();
	}
    else{
        std::cout<<"[DONE]  CONNECTED to the sql server\n";
    }
	if ((mysql_real_connect(connection, INIT_HOST, INIT_DB_USER, INIT_DB_PASSWORD, NULL, INIT_DB_PORT, NULL, 0)) == NULL) 
	{
		std::cout << "[!!] Error during the connection with sql initialize database: " << std::endl;
		fprintf(stderr, "%s\n", mysql_error(connection));
		mysql_close(connection);
		std::cin.get();
		exit(-1);
	}
    else{
        std::cout<<"[DONE]  CONNECTED to the initialize user\n";
    }
    std::string query = "CREATE USER IF NOT EXISTS '";
    query += DB_USER;
    query += "'@'";
    query += HOST_MACHINE;
    query += "' IDENTIFIED BY '";
    query += DB_PASSWORD;
    query += "';";
    if (mysql_query(connection, query.c_str())) 
	{
		fprintf(stderr, "%s\n", mysql_error(connection));
		std::cin.get();
		mysql_close(connection);
		exit(-1);
	}
    else{
        std::cout<<"[DONE]  CREATE the main user\n";
    }

    if(mysql_change_user(connection, DB_USER, DB_PASSWORD, "mysql") != 0){
        std::cout << "[!!] Error during the db user changing: " << std::endl;
        fprintf(stderr, "%s\n", mysql_error(connection));
		mysql_close(connection);
		std::cin.get();
		exit(-1);
    }
    else{
        std::cout<<"[DONE]  CHANGED to the main user\n";
    }
    query = "mariadb -u ";
    query += DB_USER;
    query += " -p";
    query += DB_PASSWORD;
    query += " < ./Config_files/Init/Sql_scripts.sql";
    system(query.c_str());
    std::cout<<"[DONE]  SETTED the database environment for Password_manager\n";
}