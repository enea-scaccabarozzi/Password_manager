#pragma once
#ifndef DATABASE_CONF_FLAG
#define DATABASE_CONF_FLAG

/*
Edit this variables to set the datas of the mariadb user that will set up the
enviroment for the database.
 */ 
#define INIT_HOST "localhost" //host_machine
#define INIT_DB_USER "admin" //username
#define INIT_DB_PASSWORD "Ezio_log" //password
#define INIT_DB_PORT 3306 //mariadb port


/*
Edit this variables to set the datas of the mariadb user that you will
use to run the program [If the user doesn't already exist a new one will be created]
 */ 
#define HOST_MACHINE "localhost" //host_machine
#define DB_USER "Password_manager_admin" //username
#define DB_PASSWORD "Password" //password
#define DB_PORT 3306 //mariadb port


#endif //DATABASE_CONF_FLAG