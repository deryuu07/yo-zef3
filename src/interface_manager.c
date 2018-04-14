#include <stdio.h>
#include "interface_manager.h"

void load_data_set()
{
//	get_data_set(data_set);
	load_cui_data_set(data_set);
}

short get_num_message()
{
	char message = 0;

	while(message == 0){
		message = sci_data_get();
	}

	return message;
}

short get_message()
{
	char message = 0;

	while(message != 'w' && 
		  message != 's' &&
		  message != ENTER){
		message = sci_data_get();
	}

	return message;
}

short get_arrows(short arrow_cnt, short max_cnt, char command)
{
	if(command == 'w'){
		arrow_cnt--;
		if(arrow_cnt < 0) arrow_cnt=0;
	}
	if(command == 's'){
		arrow_cnt++;
		if(arrow_cnt > max_cnt) arrow_cnt = 0;
	}

	return arrow_cnt;
}

short menu_out_START()
{
	short mode_num = 255;
	char msg;
	short i=0;
	static short arrow_cnt = 0;

	for(i=0; i<sizeof(messege_patern)/CHAR_SIZE; i++){
		if(arrow_cnt == i ){print_str("-> ");}
		else{				print_str("   ");}
		print_str(messege_patern[i]);
		print_str("\n");
	}
	print_str("\r");

	msg = get_message();
	arrow_cnt = get_arrows(arrow_cnt, sizeof(messege_patern)/CHAR_SIZE, msg);

	if(msg == ENTER){
		mode_num = arrow_cnt;
	}

	return mode_num;
}

void send_r_command()
{
	print_str("\r");
}

void write_data_set()
{
	unsigned long l_buf;
	unsigned short buf[4];
	short i;

	(void)fld_init_pclk_notification(0);
	fld_blank_check_2KB(0);
	Data_flash_delete(0, 0);
	buf[0] = data_set[0];
	buf[1] = data_set[1];
	buf[2] = data_set[2];
	buf[3] = 0;
	Date_flash_write(0, 0, buf);	

	(void)fld_init_pclk_notification(0);
	fld_blank_check_2KB(0);
	Data_flash_delete(0, 1);
	buf[0] = data_set[3];
	buf[1] = data_set[4];
	buf[2] = data_set[5];
	buf[3] = 0;
	Date_flash_write(0, 1, buf);	

	(void)fld_init_pclk_notification(0);
	fld_blank_check_2KB(0);
	Data_flash_delete(0, 2);
	buf[0] = data_set[6];
	buf[1] = data_set[7];
	buf[2] = 0;
	buf[3] = 0;
	Date_flash_write(0, 2, buf);	

	(void)fld_init_pclk_notification(0);
	fld_blank_check_2KB(0);
	Data_flash_delete(0, 3);
	buf[0] = data_set[8];
	buf[1] = data_set[9];
	buf[2] = data_set[10];
	buf[3] = data_set[11];
	Date_flash_write(0, 3, buf);	
}

short change_parameters(short mode_num)
{
	short loop_end_flag = 0;
	short data = 0;
	char msg = 0;

	while(msg != 'r' && msg != ENTER){
		print_str(messege_patern[mode_num]);
		print_str(" ");
		print_num(data_set[mode_num], 4);
		print_str(" ");
		print_num(data, 4);

		send_r_command();
		msg = get_num_message();
		if(msg-'0' >= 0 && msg-'0' <= 9){
			data *= 10;
			data += (short)(msg - '0');
		}
	}

	data_set[mode_num] = data;
	write_data_set();

	return loop_end_flag;
}

void out_param()
{
	short i;

	for(i=0; i<sizeof(messege_patern)/CHAR_SIZE; i++){
		print_str(messege_patern[i]);
		print_str("  ");
		print_num(data_set[i], 4);
		print_str("\n");
	}
	print_str("\r");
}

void send_msg_type(char msg_type)
{
	print_str(msg_type);
	print_str("\r");
}

void out_log_data2()
{
	LED_reset();
//	send_msg_type(LOG_MSG_TYPE);
	out_log_data();
}

void coms_start()
{
	short mode_num = 255;
	short loop_end_flag = 0;
	LED_reset();
	load_data_set();

//	send_msg_type(PARA_MSG_TYPE);
	out_param();

	while(!loop_end_flag){
		while(mode_num == 255){
 			mode_num = menu_out_START();
		}

		loop_end_flag = change_parameters(mode_num);
		mode_num = 255;
	}
}
