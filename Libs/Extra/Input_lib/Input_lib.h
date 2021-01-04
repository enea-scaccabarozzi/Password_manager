
#ifndef INPUTLIB
#define INPUTLIB

int IN_val(int& x);
int IN_val(char& x);
int IN_val(float& x);
void IN_lim_ricors(int& x, int inf, int sup, const char* message, const char* error);
int IN_lim(int& x, int inf, int sup);
int IN_spec_char(char& x, const char* allowed_chars);
void IN_spec_char_ricors(char& x, const char* allowed_chars, const char* message, const char* error);

#endif


