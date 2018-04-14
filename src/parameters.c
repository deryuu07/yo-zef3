#include <stdio.h>

float gyro_K_P = 0.5;
float gyro_K_I = 0.0;
float gyro_K_D = 0.0;

float vel_K_P = 0.1;
float vel_K_I = 0.0;
float vel_K_D = 0.0;

float vel_FF_accel = 0.1;
float vel_FF_vel = 1.0;

float FF_a = 0.590;
float FF_b = 130.75;

float debug_para1 = 0.0;
float debug_para2 = 0.0;
float debug_para3 = 0.0;
float debug_para4 = 0.0;

#define DATA_SET_SIZE 12
short cui_data_set[DATA_SET_SIZE];

void load_cui_data_set2()
{
	unsigned short load[4];
    float ftmp = 0.0;

    //gyro_K_
	Data_flash_read(0, 0, load);
    print_num(load[0], 4);
    print_str("\r\n");
    print_num(load[1], 4);
    print_str("\r\n");
    print_num(load[2], 4);
    print_str("\r\n");
    ftmp = (float)load[0];
    gyro_K_P = ftmp / 1000.0;
    ftmp = (float)load[1];
    gyro_K_I = ftmp / 1000.0;
    ftmp = (float)load[2];
    gyro_K_D = ftmp / 1000.0;

    //vel_K_
	Data_flash_read(0, 1, load);
    print_num(load[0], 4);
    print_str("\r\n");
    print_num(load[1], 4);
    print_str("\r\n");
    print_num(load[2], 4);
    print_str("\r\n");
    ftmp = (float)load[0];
    vel_K_P = ftmp / 1000.0;
    ftmp = (float)load[1];
    vel_K_I = ftmp / 1000.0;
    ftmp = (float)load[2];
    vel_K_D = ftmp / 1000.0;

    //vel_FF_
	Data_flash_read(0, 2, load);
    print_num(load[0], 4);
    print_str("\r\n");
    print_num(load[1], 4);
    print_str("\r\n");
    ftmp = (float)load[0];
    vel_FF_accel = ftmp / 1000.0;
    ftmp = (float)load[1];
    vel_FF_vel = ftmp / 1000.0;

    //debug_para
	Data_flash_read(0, 3, load);
    print_num(load[0], 4);
    print_str("\r\n");
    print_num(load[1], 4);
    print_str("\r\n");
    print_num(load[2], 4);
    print_str("\r\n");
    print_num(load[3], 4);
    print_str("\r\n");
    ftmp = (float)load[0];
    debug_para1 = ftmp;
    ftmp = (float)load[1];
    debug_para2 = ftmp;
    ftmp = (float)load[2];
    debug_para3 = ftmp;
    ftmp = (float)load[3];
    debug_para4 = ftmp;
}

void load_cui_data_set(short *data_set)
{
	unsigned short load[4];

    //gyro_K_
	Data_flash_read(0, 0, load);
	data_set[0] = load[0];
	data_set[1] = load[1];
	data_set[2] = load[2];
    gyro_K_P = (float)load[0] / 1000.0;
    gyro_K_I = (float)load[1] / 1000.0;
    gyro_K_D = (float)load[2] / 1000.0;

    //vel_K_
	Data_flash_read(0, 1, load);
	data_set[3] = load[0];
	data_set[4] = load[1];
	data_set[5] = load[2];
    vel_K_P = (float)load[3] / 1000.0;
    vel_K_I = (float)load[4] / 1000.0;
    vel_K_D = (float)load[5] / 1000.0;

    //vel_FF_
	Data_flash_read(0, 2, load);
	data_set[6] = load[0];
	data_set[7] = load[1];
    vel_FF_accel = (float)load[6] / 1000.0;
    vel_FF_vel = (float)load[7] / 1000.0;

    //debug_para
	Data_flash_read(0, 3, load);
	data_set[8] = load[0];
	data_set[9] = load[1];
	data_set[10] = load[2];
	data_set[11] = load[3];
    debug_para1 = (float)load[0];
    debug_para2 = (float)load[1];
    debug_para3 = (float)load[2];
    debug_para4 = (float)load[3];
}

void get_gyro_FB_gain(float *para)
{
    if(para != NULL){
        para[0] = gyro_K_P;
        para[1] = gyro_K_I;
        para[2] = gyro_K_D;
    }
}

void get_vel_FB_gain(float *para)
{
    if(para != NULL){
        para[0] = vel_K_P;
        para[1] = vel_K_I;
        para[2] = vel_K_D;
    }
}

void get_vel_FF_gain(float *para)
{
    if(para != NULL){
        para[0] = vel_FF_accel;
        para[1] = vel_FF_vel;
    }
}

void get_vel_FF_para(float *para)
{
    if(para != NULL){
        para[0] = FF_a;
        para[1] = FF_b;
    }
}

void get_debug_para(float *para)
{
    if(para != NULL){
        para[0] = debug_para1;
        para[1] = debug_para2;
        para[2] = debug_para3;
        para[3] = debug_para4;
    }
}