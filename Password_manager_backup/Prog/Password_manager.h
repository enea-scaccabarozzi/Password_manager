class flags 
{
public:
	flags();
	int login;
	int header;
	int help;
};

void get_line_command(int ar_cont, char* ar_vett[], flags& flg, int ind);
void print_header();
void login();
int secondo_fattore();
void print_help();
void menu();
void inserisci_el();
void usage();
void cancella_el();
void stampa_el();
void cerca_el();
void crea_account();
