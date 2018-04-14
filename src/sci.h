#ifndef __SCI_HEADER__
#define __SCI_HEADER__

void SCI_initialize();
void sci_print(unsigned char data);
char sci_data_get(void);
void print_str(unsigned char *str);
void print_num(long data,short n);
void print_clear(void);
void back_space(char n);
void pointer_move(unsigned char line,unsigned char column);
void clear_pointer_to_eol(void);
void clear_sol_to_pointer(void);
float scan_fnum(void);
short scan_num(void);
char scan_vector(void);
void scan_enter(void);
short my_atos(char s[]);
float my_atof(char s[]);
char my_isspace(char c);
char my_isdigit(char c);
void out_float(float data);

struct send_data{
	short data[10];

	enum mode{
		SENS,
		GYRO,
		MAP
	};
};

#endif
