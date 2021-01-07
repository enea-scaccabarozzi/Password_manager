#include "Main_lib.h"
#include <iostream>
#include <cstring>
#include <ctime>


ges_pwd::ges_pwd() 
{
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		std::cout << "[!!] Error during initialization of sql server. " << std::endl;
		exit(-1);
		std::cin.get();
	}
	
	if ((mysql_real_connect(conn, HOST_MACHINE, DB_USER, DB_PASSWORD, "Password_manager", DB_PORT, NULL, 0)) == NULL) 
	{
		std::cout << "[!!] Error during connection with sql database: " << std::endl;
		fprintf(stderr, "%s\n", mysql_error(conn));
		mysql_close(conn);
		std::cin.get();
		exit(-1);
	}

	UserID = "NULL";
}

int ges_pwd::login(const char* username, const char* pwd) 
{
	std::string encoded_pwd = SHA_256(pwd);
	std::string query;
	query = "SELECT ID FROM user WHERE UserName = '" + std::string(username) + "';";
	
	if (mysql_query(conn, query.c_str())) 
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -3;
	}
	MYSQL_RES* result = mysql_store_result(conn);
	if (mysql_fetch_row(result) == NULL)
	{
		return -1;
	}
		
	query = "SELECT ID FROM user WHERE pwd = '" + encoded_pwd + "';";
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -3;
	}
	result = mysql_store_result(conn);
	MYSQL_ROW row;
	row = mysql_fetch_row(result);
	if (row == NULL)
	{
		return -2;
	}
		
	UserID = row[0];
	int res=initialize_current_data();
	return res;

}

int ges_pwd::create_account(const char* mail, const char* username, const char* pwd )
{
	std::string encoded_pwd(SHA_256(pwd));
	std::string query = "SELECT ID FROM user WHERE UserMail = '" + std::string(mail) + "'";
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		exit(1);
	}
	MYSQL_RES* result = mysql_store_result(conn);
	if (mysql_fetch_row(result) != NULL)
	{
		return -1;
	}
		
	query = "SELECT ID FROM user WHERE UserName = '" + std::string(username) + "'";
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		exit(1);
	}
	result = mysql_store_result(conn);
	if (mysql_fetch_row(result) != NULL)
	{
		return -2;
	}
		
	query = "INSERT INTO user (UserName, UserMail, pwd) ";
	query += "VALUES (";
	query += "'" + std::string(username) + "', ";
	query += "'" + std::string(mail) + "', ";
	query += "'" + encoded_pwd + "' ";
	query += ");";
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		exit(1);
	}
	return 0;
}

int ges_pwd::logout() 
{
	std::string INS_query = "INSERT INTO data(UserID, pwd, tm, service, account, IV) VALUES(";
	std::string SEL_query = "SELECT pwd, tm, service, account, IV FROM current_data WHERE bool = 0";
	
	std::string query="SELECT MAX(DataID) FROM data";
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	MYSQL_RES* result = mysql_store_result(conn);
	MYSQL_ROW row = mysql_fetch_row(result);
	std::string ind;
	if (mysql_fetch_row(result) != NULL)
	{
		std::string ind = row[0];
	}
	else{
		ind = "1";
	}
	mysql_free_result(result);

	query = "ALTER TABLE data AUTO_INCREMENT = " + ind;
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}

	if (mysql_query(conn, SEL_query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	result = mysql_store_result(conn);

	while ((row = mysql_fetch_row(result)))
	{
		query = INS_query + "'" + UserID + "', "
						  + "'" + row[0] + "', " + "'" + row[1] + "', "
						  + "'" + row[2] + "', " + "'" + row[3] + "', "
						  + "'" + row[4] + "');";
	
		if (mysql_query(conn, query.c_str()))
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			std::cin.get();
			mysql_close(conn);
			return -1;
		}
	}
	mysql_free_result(result);

	query = "DELETE FROM current_data WHERE accountID = " + std::string(UserID) + ";";
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	mysql_close(conn);
	return 0;
}

int ges_pwd::insert_element(const char* service, const char* account, const char* pwd)
{
	std::string iv;
	std::string encoded_pwd = AES_enc(pwd, iv);

	std::string query;
	query = "INSERT INTO current_data (pwd, service, account, IV, bool, accountID) ";
	query += "VALUES('" + encoded_pwd + "', '"
						+ std::string(service, strlen(service)) + "', '"
						+ std::string(account, strlen(account)) + "', '"
						+ iv + "', '0', '"
						+ UserID + "');";

	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	return 0;
}

int ges_pwd::search_element(const char* input, char flag1, int flag2)
{
	if (flag1 != 'S' && flag1 != 'A') 
	{
		return -1;
	}
		
	if (flag2 != 0 && flag2 != 1) 
	{
		return -1;
	}
	std::string query;
	if (flag2 == 0) 
	{
		query = "SELECT service, pwd, account, tm, IV, FROM current_data WHERE( accountID = "+ std::string(UserID) +" AND ";
		if (flag1 == 'S')
		{
			query += "service = '" + std::string(input,strlen(input)) + "');";
		}
		else
		{
			query += "account = '" + std::string(input, strlen(input)) + "');";
		}
			
	}
	else 
	{
		query = "SELECT service, pwd, account, tm, IV FROM current_data WHERE( accountID = " + std::string(UserID) + " AND ";
		if (flag1 == 'S')
		{
			query += "service LIKE '%" + std::string(input, strlen(input)) + "%');";
		}
		else
		{
			query += "account LIKE '%" + std::string(input, strlen(input)) + "%');";
		}
			
	}

	int res=print_result(query.c_str());
	return res;
}

int ges_pwd::remove_element(const char* service, char flag)
{
	std::string query1;
	std::string query2;
	if (flag != 'S' && flag != 'A')
	{
		return -1;
	}
		
	if (flag == 'S') 
	{
		query1 = "DELETE FROM current_data WHERE( service = '" + std::string(service, strlen(service)) + "' AND accountID = "+ std::string(UserID) + " );";
		query2 = "DELETE FROM data WHERE ( service = '" + std::string(service, strlen(service)) + "' AND UserID = " + std::string(UserID) + " );";
	}
	else 
	{
		query1 = "DELETE FROM current_data WHERE( account = '" + std::string(service, strlen(service)) + "' AND accountID = "+ std::string(UserID) + " );";
		query2 = "DELETE FROM data WHERE ( account = '" + std::string(service, strlen(service)) + "' AND UserID = " + std::string(UserID) + " );";
	}
	if (mysql_query(conn, query1.c_str())) 
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}

	if (mysql_query(conn, query2.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	return 0;
}

const char* ges_pwd::get_user_mail()
{
	std::string query = "SELECT UserMail FROM user WHERE ID = ";
	query += UserID;
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return nullptr;
	}
	MYSQL_RES* result = mysql_store_result(conn);
	MYSQL_ROW row = mysql_fetch_row(result);
	const char* res= row[0];
	return res;
}

int ges_pwd::print_elements(char flag/* = 'N'*/)
{
	std::string query;
	switch (flag) {
	case 'N':
		query = "SELECT service, pwd, account, tm, IV FROM current_data WHERE accountID = " + std::string(UserID);
		break;
	case 'C':
		query= "SELECT service, pwd, account, tm, IV FROM current_data WHERE accountID = " + std::string(UserID) + " ORDER BY service ASC;";
		break;
	case 'D':
		query= "SELECT service, pwd, account, tm, IV FROM current_data WHERE accountID = " + std::string(UserID) + " ORDER BY service DESC;";
		break;
	default:
		return -1;
	}
	int res = print_result(query.c_str());
	return res;
}

int ges_pwd::initialize_current_data() 
{
	std::string query = "DELETE FROM current_data WHERE accountID = " + std::string(UserID) + ";";
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	std::string ind;
	query = "SELECT MAX(Ind) FROM current_data;";
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	MYSQL_RES* result = mysql_store_result(conn);
	MYSQL_ROW row = mysql_fetch_row(result);
	if ( row[0] == NULL) {
		query = "ALTER TABLE current_data AUTO_INCREMENT = 0";
	}
	else{
		ind = row[0];
		query = "ALTER TABLE current_data AUTO_INCREMENT = "+ind;
	}
	mysql_free_result(result);
	if (mysql_query(conn, query.c_str())) 
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	std::string SEL_query = "SELECT pwd, tm, service, account, UserID, IV FROM data WHERE UserID=" + std::string(UserID) + ";";
	std::string INS_query = "INSERT INTO current_data (pwd, tm, service, account, accountID, IV) ";
	query = INS_query + SEL_query;
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	query = "UPDATE current_data SET bool=1 WHERE accountID=" + std::string(UserID)+";";
	if (mysql_query(conn, query.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	return 0;
}

int ges_pwd::print_result(const char* IN_query) {
	unsigned int SER_len = 7;
	unsigned int PWD_len = 8;
	unsigned int ACC_len = 7;
	unsigned int TIM_len = 4;
	std::string str;
	if (mysql_query(conn, IN_query))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	MYSQL_RES* result = mysql_store_result(conn);
	MYSQL_ROW row;
	std::string recovered_pwd;
	std::string iv_str;
	std::string iv;

	while ((row = mysql_fetch_row(result))) 
	{
		str = row[0];
		if (SER_len < str.length())
		{
			SER_len = str.length();
		}

		std::string iv_str =row[4];
		str = AES_dec(row[1], iv_str);
		if (PWD_len < str.length())
		{
			PWD_len = str.length();
		}
			
		str = row[2];
		if (ACC_len < str.length())
		{
			ACC_len = str.length();
		}
			
		str = row[3];
		if (TIM_len < str.length())
		{
			TIM_len = str.length();
		}
			
	}
	mysql_free_result(result);
	std::cout << "+";
	print_line(SER_len + 2);
	print_line(PWD_len + 2);
	print_line(ACC_len + 2);
	print_line(TIM_len + 2);
	std::cout << std::endl;
	std::cout << "| ";
	print_value("Service", SER_len + 1);
	print_value("Password", PWD_len + 1);
	print_value("Account", ACC_len + 1);
	print_value("Date", TIM_len + 1);
	std::cout << std::endl;
	std::cout << "+";
	print_line(SER_len + 2);
	print_line(PWD_len + 2);
	print_line(ACC_len + 2);
	print_line(TIM_len + 2);
	std::cout << std::endl;

	if (mysql_query(conn, IN_query)) 
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		std::cin.get();
		mysql_close(conn);
		return -1;
	}
	result = mysql_store_result(conn);
	while ((row = mysql_fetch_row(result))) 
	{
		std::cout << "| ";
		print_value(row[0], SER_len + 1);
		std::string iv_str = row[4];
		str = AES_dec(row[1], iv_str);
		print_value(str.c_str(), PWD_len + 1);
		print_value(row[2], ACC_len + 1);
		print_value(row[3], TIM_len + 1);
		std::cout << std::endl;
		std::cout << "+";
		print_line(SER_len + 2);
		print_line(PWD_len + 2);
		print_line(ACC_len + 2);
		print_line(TIM_len + 2);
		std::cout << std::endl;
	}
	std::cout<<std::endl << "\t# END OF VALUES #" << std::endl;
	mysql_free_result(result);
	return 0;
}

void ges_pwd::print_line(int dim)
{
	for (int i = 0; i < dim; i++)
	{
		std::cout << "-";
	}
		
	std::cout << "+";
}

void ges_pwd::print_value(const char* value, int dim){
	std::string val_str = value;
	std::cout << val_str;
	for (int i = val_str.length(); i < dim; i++)
	{
		std::cout << " ";
	}
		
	std::cout << "| ";
}
