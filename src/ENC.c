#include "iodefine.h"
#include "machine.h"
#include <stdio.h>

#define enc_r MTU2.TCNT
#define ENC_R 65535-enc_r
#define ENC_L MTU1.TCNT
#define MMPP  0.0095820
#define WINDOW_SIZE 100

long sum_ENC_R = 0;
long sum_ENC_L = 0;
long sum_ENC = 0;

void reset_dis()
{
	sum_ENC_R = 0;
	sum_ENC_L = 0;
	sum_ENC = 0;	
}

void reset_timer_enc()
{
	enc_r = 61535;
	ENC_L = 4000;
}

void init_enc()
{
	reset_timer_enc();
	sum_ENC_R = 0;
	sum_ENC_L = 0;
	sum_ENC = 0;
}

short get_ENC_R()
{
	return ENC_R;
}

short get_ENC_L()
{
	return ENC_L;
}

long get_sum_ENC()
{
	return sum_ENC;
}

short get_vel_ENC()
{
	short tmp = (ENC_R + ENC_L - 8000)/2;
	return tmp;
}

void get_dis(float *curr_dis)
{
    if(curr_dis != NULL)
		*curr_dis = (float)sum_ENC * MMPP;
}

void update_ENC_dis()
{
	sum_ENC_R += (ENC_R - 4000);
	sum_ENC_L += (ENC_L - 4000);
	sum_ENC = (sum_ENC_R + sum_ENC_L)/2;
}

short wheel_vel_counter()
{
	unsigned long oTime, nTime;
	short Time = 0;

	ENC_L = 4000;
	oTime = get_time();
	while(Time < 100){
		nTime = get_time();
		Time = nTime - oTime;
	}

	if(ENC_L > 4800){
		return 1;
	}
	else{
		return 0;
	}
}


