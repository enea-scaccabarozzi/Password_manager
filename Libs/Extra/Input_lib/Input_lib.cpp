#include "Input_lib.h"
#include <iostream>
#include <cstring>
using namespace std;

int IN_val(int& x) {
	cin >> x;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(10000, '\n');
		return -1;
	}
	return 0;
}

int IN_val(char& x) {
	cin >> x;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(10000, '\n');
		return -1;
	}
	cin.ignore(10000, '\n');
	return 0;
}

int IN_val(float& x) {
	cin >> x;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(10000, '\n');
		return -1;
	}
	return 0;
}

void IN_lim_ricors(int& x, int inf, int sup, const char* message, const char* error) {
	cout << message;
	if (IN_val(x) == -1) {
		cout << error << endl;
		IN_lim_ricors(x, inf, sup, message, error);
	}
	if (x<inf || x>sup) {
		cout<<error<<endl;
		IN_lim_ricors(x, inf, sup, message, error);
	}
		
}

int IN_lim(int& x, int inf, int sup) {
	if (IN_val(x) == -1) 
		return -1;
	if (x<inf || x>sup) 
		return -2;
	return 0;
}

int IN_spec_char(char& x, const char* allowed_chars) {
	if (IN_val(x) == -1)
		return -1;

	int len = strlen(allowed_chars);
	for (int i = 0; i < len; i++)
		if (allowed_chars[i] == x)
			return 0;
	return -2;
}

void IN_spec_char_ricors(char& x, const char* allowed_chars,const char* message, const char* error) {
	cout << message;
	if (IN_val(x) == -1) {
		cout << error << endl;
		IN_spec_char_ricors(x, allowed_chars, message, error);
	}
	int len = strlen(allowed_chars);
	for (int i = 0; i < len; i++)
		if (allowed_chars[i] == x)
			return;
	cout << error << endl;
	IN_spec_char_ricors(x, allowed_chars, message, error);
}
