#include <stdio.h>

float acc_x = 0.0;
float acc_y = 0.0;

float acc_vel_x = 0.0;
float acc_vel_y = 0.0;

float acc_dis_x = 0.0;
float acc_dis_y = 0.0;

float offset_x = 0.0;
float offset_y = 0.0;

void reset_acc_vel()
{
    acc_vel_x = 0.0;
    acc_vel_y = 0.0;
}

void reset_acc_dis()
{
    acc_dis_x = 0.0;
    acc_dis_y = 0.0;
}

void acc_dis_update()
{
    acc_dis_x += acc_vel_x * 0.001;
    acc_dis_y += acc_vel_y * 0.001;
}

void acc_vel_update()
{
    acc_vel_x += acc_x; //acc * d_t * 1000 [mm/s]
//    acc_vel_y += acc_y;
}

void acc_update()
{
    acc_x = ((float)get_accelarator(0) / 2048.0f * 9.8f) - offset_x;
    acc_y = ((float)get_accelarator(1) / 2048.0f * 9.8f) - offset_y;

    acc_vel_update();
//    acc_dis_update();
}

void get_acc_dis_x(float *curr_dis)
{
    if(curr_dis != NULL)
        *curr_dis = acc_dis_x;
}

void get_acc_dis_y(float *curr_dis)
{
    if(curr_dis != NULL)
        *curr_dis = acc_dis_y;
}

void get_acc_vel_xf(float *curr_vel)
{
    float enc_vel = 0.0;

    if(curr_vel != NULL){
        get_vel(&enc_vel);
        *curr_vel = 0.7*acc_vel_x + 0.3*enc_vel;
//        *curr_vel = acc_vel_x;
    }
}

void get_acc_vel_yf(float *curr_vel)
{
    if(curr_vel != NULL)
        *curr_vel = acc_vel_y;
}

void get_acc(float *acc)
{
    if(acc != NULL){
        acc[0] = acc_x;
        acc[1] = acc_y;
        acc[2] = 0.0;
    }
}

void get_acc_vel(float *acc_vel)
{
    if(acc_vel != NULL){
        acc_vel[0] = acc_vel_x;
        acc_vel[1] = acc_vel_y;
        acc_vel[2] = 0.0;
    }
}

void set_acc_offset()
{
    float curr_acc[3];
    long oTime, nTime;
    float tmp_x = 0.0;
    float tmp_y = 0.0;
    float cnt = 0.0;

    oTime = get_time();
    nTime = get_time();
    while(nTime-oTime < 1000){
        get_acc(curr_acc);
        tmp_x += curr_acc[0];
        tmp_y += curr_acc[1];
        cnt += 1.0;
    
        nTime = get_time();
    }
    tmp_x /= cnt;
    tmp_y /= cnt;

    offset_x = tmp_x;
    offset_y = tmp_y;
}