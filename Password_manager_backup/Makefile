all : Setup_prog Password_manager

Setup_prog : Init_database.o
	@echo "[>>]  Unioning objects files for setup program .."
	g++ -o Setup_prog Init_database.o $$(mariadb_config --cflags --libs)
	@echo "[>>]  Running setup program .."
	./Setup_prog
	@echo "[!!]  Deleting setup files .."
	rm -f Init_database.o Setup_prog

Init_database.o : ./Config_files/Init/Init_database.cpp
	@echo "[>>]  Compiling Init_database.o .."
	g++ -c -o Init_database.o ./Config_files/Init/Init_database.cpp $$(mariadb_config --cflags --libs)

Password_manager : Password_manager.o Main_lib.o Input_lib.o Mail_lib.o
	@echo "[>>]  Unioning objects file .."
	@echo "[>>]  Creating Password_manager .."
	g++ -o Password_manager Password_manager.o Main_lib.o Input_lib.o Mail_lib.o $$(mariadb_config --cflags --libs) -lPocoNet -lPocoFoundation -lPocoNetSSL
	@echo "[!!]  Deleting objects file .."
	rm -f *.o
	@echo "[!!]  Deleting Setup program .."
	rm -f Setup_prog
	@echo "[VV]  Installation completed correctly"

Password_manager.o : ./Prog/Password_manager.cpp ./Prog/Password_manager.h Setup_prog
	@echo "[>>]  Compiling Password_manager.o .."
	g++ -c -o Password_manager.o ./Prog/Password_manager.cpp $$(mariadb_config --cflags --libs)

Main_lib.o : ./Libs/Main/Main_lib.cpp ./Libs/Main/Main_lib.h
	@echo "[>>]  Compiling Main_lib.o .."
	g++ -c -o Main_lib.o ./Libs/Main/Main_lib.cpp $$(mariadb_config --cflags --libs)

Input_lib.o : ./Libs/Extra/Input_lib/Input_lib.cpp ./Libs/Extra/Input_lib/Input_lib.h
	@echo "[>>]  Compiling Input_lib.o .."
	g++ -c -o Input_lib.o ./Libs/Extra/Input_lib/Input_lib.cpp

Mail_lib.o : ./Libs/Extra/Mail_lib/Mail_lib.cpp ./Libs/Extra/Mail_lib/Mail_lib.h
	@echo "[>>]  Compiling Mail_lib.o .."
	g++ -c -o Mail_lib.o ./Libs/Extra/Mail_lib/Mail_lib.cpp -lPocoNet -lPocoFoundation -lPocoNetSSL
	
