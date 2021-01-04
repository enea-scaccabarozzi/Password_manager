PASSWORD_MANAGER

A small personal project is hosted in this repository, its purpose is to facilitate the management of your passwords.

To do this a mariadb database is used to connect to via the command line, ensuring an easy use with its simple interface.

MAIN FEATURES
.1) A simple menù that you can use to display, search, insert or delete your passwords;
.2) The securness of a two factor identification;
.3) The possibity of change parametrers of your database easily;
.4) A software built to be multi-user in order to help you to keep order in your datas

HOW TO INSTALL IT ON YOUR PC

Requirements:
!! (TESTED ONLY IN UNIX-LIKE SYSTEM)  !!
To run this program you'll only need an mariadb account, but in order to create the executable file you must have installed on your machine some tools:
1.) g++ compiler (To compile the files)
2.) Some libraries belonging to the poco project (In order to send mail with SSl protocol, sudo apt-get install libpoco-dev)
3.) Some librariesbelonging to mariadb c++ connector (In order to access to the database from c++ scripts, sudo apt-get install libmariadbclient-dev)
4.) Make tool istalled on your machine

Installation:
!! Before the installation you have to set the datas contained in the file 'Password_manager/Config_files/Database.conf.h' with the credential of the account that it is used for the database set up (This user is no longer necessary after the istallation); !!

After you have done all the previous steps you have simple to run 'make' and after a few secon you'll find the executable program in the main folder.

If you find some bugs or errors during your use please report them here :)


