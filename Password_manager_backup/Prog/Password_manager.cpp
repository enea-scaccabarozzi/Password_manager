#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "../Libs/Extra/Input_lib/Input_lib.h"
#include "../Libs/Extra/Mail_lib/Mail_lib.h"
#include "../Libs/Main/Main_lib.h"
#include "Password_manager.h"
using namespace std;

ges_pwd gestore;
bool is_log=false;

int main(int argc, char** argv) 
{
	flags input_flags;
	
	get_line_command(argc, argv, input_flags, 1);
	if (input_flags.help == 1)
	{
		print_help();
	}
		
	if (input_flags.header == 1)
	{
		print_header();
	}
		
	if (input_flags.login == 1)
	{
		login();
	}

	menu();
}

flags::flags()
{
	login = -1;
	header = 1;
	help = -1;
}

void get_line_command(int ar_cont,char* ar_vett[], flags &flg, int ind) 
{
	if (ind < ar_cont) 
	{
		if (ar_vett[ind] == "-s") 
		{
			flg.header = -1;
			get_line_command(ar_cont, ar_vett, flg, ind + 1);
		}
		else if (ar_vett[ind] == "-log")
		{
			flg.login = 1;
			get_line_command(ar_cont, ar_vett, flg, ind + 1);
		}
		else if (ar_vett[ind] == "-h") 
		{
			flg.help = 1;
			get_line_command(ar_cont, ar_vett, flg, ind + 1);
		}
		else {
			usage();
		}
			
	}
}

void usage() 
{
	cout << "Usage: eg_prog [command1] [command2] [..]" << endl;
	cout << "For help type <-h>" << endl;
	cin.get();
	exit(0);
}

void print_header() 
{
	cout << "    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-" << endl
		<< "-+-+-+-+-+-+-+-+[ Gestore password ]-+-+-+-+-+-+-+-+-+" << endl
		<< "     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-" << endl << endl;
}

void print_help() 
{
	cout << "USABLE LINE COMMANDS:\n"<<endl;
	cout << "<COMMAND> = explanation"<<endl;
	cout << "  <-h> = type this to display the manual of all the commands" << endl;
	cout << "  <-s> = type this to disable the programm's header to display" << endl;
	cout << "  <-log> = type this to start the program in the login interface" << endl;
	cout << "Usage: eg_prog [command1] [command2] [..]" << endl;
	cout << "For help type <-h>" << endl;
	cin.get();
}

void login() 
{
	is_log = false;
	string UserName, password;
	cout << "User name: ";
	cin >> UserName;
	cout << "Password: ";
	cin >> password;
	int res = gestore.login(&UserName[0], &password[0]);
	if (res == -3) 
	{
		cout << "Error during the connection with the database" << endl;
		return;
	}
		
	else if (res == -2) 
	{
		cout << "Invalid password" << endl;
		return;
	}
	else if (res == -1) 
	{
		cout << "Invalid username" << endl;
		return;
	}
	is_log = true;
	res = secondo_fattore();
	if (res == 0) 
	{
		cout << "Successfully logged in" << endl;
		is_log = true;
	}
	else if (res == -1)
	{
		cout << "[!!] Wrong code, access denied" << endl;
		gestore.logout();
	}
	else
	{
		gestore.logout();
	}
	cin.get();
}

int secondo_fattore() 
{
	srand(unsigned(time(NULL)));
	int code= rand()%9999+1;
	string mail = gestore.get_user_mail();
	string content;
	content = "Your temporary access code is: " + code;
	content += "/n If you haven't tried to acces at this service please consider to change your password account or simply ignore this mail";
	int res = send_mail(mail.c_str(),"Password Manager: access code", content.c_str());
	if(res != 0)
	{
		return -2;
	}
	cout<<"Insert it: ";
	int input;
	IN_val(input);
	if(input == code)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void menu() 
{
	int choice;
	int res;
	do{
		cout << "+----------------------[ MENU ]----------------------+" << endl
			<< "| [1]  Login                                         |" << endl
			<< "| [2]  Insert new element                            |" << endl
			<< "| [3]  Delete an element                             |" << endl
			<< "| [4]  Display elements                              |" << endl
			<< "| [5]  Search for an element                         |" << endl
			<< "| [6]  Help                                          |" << endl
			<< "| [7]  Create account                                |" << endl
			<< "| [0]  Exit                                          |" << endl
			<< "+----------------------------------------------------+" << endl;
		do 
		{
			cout << "Insert command: ";
			res = IN_lim(choice, 0, 7);
			if (res != 0)
				cout << "[!!] Error, insert a valid input" << endl;
			cin.ignore(1000, '\n');
		}
		while (res != 0);
		
		switch (choice) {
		case 1:
			login();
			break;
		case 2:
			inserisci_el();
			break;
		case 3:
			cancella_el();
			break;
		case 4:
			stampa_el();
			break;
		case 5:
			cerca_el();
			break;
		case 6:
			print_help();
			break;
		case 7:
			crea_account();
			break;
		case 0:
			gestore.logout();
			cout << "Bye" << endl;
			break;
		}
	}
	while(choice != 0);
}

void inserisci_el() 
{
	if (is_log==false)
	{
		cout << "You have to login first" << endl;
		cin.get();
		return;
	}
	string servizio, account, pws;
	cout << "Insert name of the service:" << endl;
	getline(cin, servizio, '\n');
	cout << "Insert name of the account:" << endl;
	getline(cin, account, '\n');
	cout << "Insert password:" << endl;
	getline(cin, pws, '\n');
	int res=gestore.insert_element(servizio.c_str(), account.c_str(), pws.c_str());
	if (res == -1)
	{
		cout << "[!!] FAILURE\nError in the sql database" << endl;
	}
		
	else
	{
		cout << "Data succesfuly stored" << endl;
	}
		
	cin.get();
}

void cancella_el() {
	if (is_log == false)
	{
		cout << "You have to login first" << endl;
		cin.get();
		return;
	}
	int res = -1;
	char choice;
	while (res != 0)
	{
		cout << "Would you like to delete an element identified with service name or with account name? [S/A]: ";
		res = IN_spec_char(choice, "SA");
		cin.ignore(1000, '\n');
		if (res != 0)
		{
			cout << "[!!] Error, insert a valid input" << endl;
		}
			
	}
	switch (choice) {
	case 'S':
		cout << "Insert the service name: ";
		break;
	case 'A':
		cout << "Insert the account name: ";
		break;
	default:
		cout << "Invalid input" << endl;
		return;
	}
	string service;
	cin.sync();
	getline(cin, service, '\n');
	res = gestore.remove_element(service.c_str(), choice);
	if (res == -1)
	{
		cout << "[!!] FAILURE\nError in the sql database" << endl;
	}
		
	else
	{
		cout << "Data succesfuly deleted" << endl;
	}
	cin.get();
}

void stampa_el() {
	if (is_log == false)
	{
		cout << "You have to login first" << endl;
		cin.get();
		return;
	}
	int res = -1;
	char choice;
	while (res != 0)
	{
		cout << "Would you like to display elements sorted decreasingly or increasingly? [D/C]" << endl;
		res = IN_spec_char(choice, "CD");
		cin.ignore(1000, '\n');
		if (res != 0)
			cout << "[!!] Error, insert a valid input" << endl;
	}
	switch (choice) {
	case 'C':
		res = gestore.print_elements('C');
		if (res == -1)
		{
			cout << "[!!] FAILURE\nError in the sql database" << endl;
		}
			
		break;
	case 'D':
		res = gestore.print_elements('D');
		if (res == -1)
		{
			cout << "[!!] FAILURE\nError in the sql database" << endl;
		}
			
		break;
	default:
		cout << "Invalid input" << endl;
		return;
	}
	cin.get();
}

void cerca_el() 
{
	if (is_log == false) 
	{
		cout << "You have to login first" << endl;
		cin.get();
		return;
	}
	int res = -1;
	char fl_1;
	while (res != 0) 
	{
		cout << "Would you like to search an element identified with service name or with account name? [S/A]: ";
		res = IN_spec_char(fl_1, "SA");
		cin.ignore(1000, '\n');
		if (res != 0)
		{
			cout << "[!!] Error, insert a valid input" << endl;
		}
			
	}
	int fl_2;
	res = -1;
	while (res != 0)
	{
		cout << "Would you like to search an element exaltly as your string or similar? [0/1]: ";
		res = IN_lim(fl_2, 0, 1);
		cin.ignore(1000, '\n');
		if (res != 0)
		{
			cout << "[!!] Error, insert a valid input" << endl;
		}
			
	}
	string input;
	cout << "Insert the string to search: ";
	getline(cin, input, '\n');
	res = gestore.search_element(input.c_str(), fl_1, fl_2);
	if (res == -1)
	{
		cout << "[!!] FAILURE\nError in the sql database" << endl;
	}
	cin.get();
}

void crea_account() 
{
	string username, mail, password1, password2;
	cout << "Insert your username:" << endl;
	cin >> username;
	cout << "Insert your e-mail:" << endl;
	cin >> mail;
	do 
	{
		cout << "Insert your new password: " << endl;
		cin >> password1;
		cout << "Reinsert your password: " << endl;
		cin >> password2;
		if (strcmp(password1.c_str(), password2.c_str()) != 0)
		{
			cout << "Your password are different" << endl;
		}
			
	}
	while(strcmp(password1.c_str(), password2.c_str()) != 0);
	gestore.create_account(mail.c_str(), username.c_str(), password1.c_str());
	cout << "Account successefully created" << endl;
}
