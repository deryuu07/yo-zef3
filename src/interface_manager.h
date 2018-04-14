#ifndef INTERFACE_MANAGER_H_
#define INTERFACE_MANAGER_H_

#define ENTER 13
#define CHAR_SIZE 4
#define PARA_MSG_TYPE "A"
#define LOG_MSG_TYPE "B"

char* messege_patern[]={
    "1. gyro_K_P", 
    "2. gyro_K_I", 
    "3. gyro_K_D", 
    "4. vel_K_P",
    "5. vel_K_I", 
    "6. vel_K_D", 
    "7. vel_FF_accel", 
    "8. vel_FF_vel",
    "9. debug_para1",
    "10. debug_para2",
    "11. debug_para3",
    "12. debug_para4"

};

void menu_out(short arrow_cnt, short menu_num);
void start_menu_out(short arrow_cnt);
void communicate_phase1();
void coms_start();
short get_command(short max_arrow, char *command);

short data_set[12];


#endif /* INTERFACE_MANAGER_H_ */
