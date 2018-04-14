#include <stdio.h> 
#include "utility.h"

float curr_vel = 0.0;
float vel_pool[WINDOW_SIZE];
float sum_vel = 0.0;

float desire_vel = 0.0;
float desire_max_vel = 0.0;
float accel_dis = 0.0;
float accel = 6000.0;
float accel_def = 6000.0;

void init_vel()
{
	short i;

	for(i=0; i<WINDOW_SIZE; i++){
		vel_pool[i] = 0.0;
	}
	sum_vel = 0.0;	
}

void get_vel(float *curr_vel_)
{
    if(curr_vel_ != NULL)
		*curr_vel_ = curr_vel;
}

void get_accel_dis(float *curr_accel_dis)
{
    if(curr_accel_dis != NULL)
		*curr_accel_dis = accel_dis;
}

void get_desire_vel(float *curr_desire_vel)
{
    if(curr_desire_vel != NULL)
		*curr_desire_vel = desire_vel;
}

void get_accel(float *curr_accel)
{
    if(curr_accel != NULL)
		*curr_accel = accel;
}

void set_accel_def(float acc)
{
	accel_def = acc;
	accel = accel_def;
}

void set_accel(float acc)
{
	accel = acc;
}

void update_desire_vel(short state)
{
	switch(state){
		case ACCEL_STEP:
			if(desire_vel < desire_max_vel){
				set_accel(accel_def);
				desire_vel += accel*DELTA_T; 
			}
			else{
				set_accel(0.0);
				desire_vel = desire_max_vel; 
			}
			break;

		case CONST_STEP:
			if(desire_vel < desire_max_vel){
				set_accel(accel_def);
				desire_vel += accel_def*DELTA_T; 
			}
			else{
				set_accel(0.0);
				desire_vel = desire_max_vel; 
			}
			break;

		case DECEL_STEP:
			if(desire_vel > 0.0){
				set_accel(-1.0f*accel_def);
				desire_vel += accel*DELTA_T; 
			}
			else{
				set_accel(0.0);
				desire_vel = 0.0; 
				set_end_flag(0);
			}
			break;
	}
}

short accel_manager()
{
	short result = 0;
	set_accel(accel_def);

	if(desire_vel < desire_max_vel){
		desire_vel += accel*DELTA_T;
	}
	else{
		desire_vel = desire_max_vel;		
		return 1;
	}

	return 0;
}

short deccel_manager()
{
	float curr_vel;
	set_accel(-1.0*accel_def);

	if(desire_vel > 0.0){
		desire_vel += accel*DELTA_T;
	}
	else{
		desire_vel = 0.0;		
		return 1;
	}

	return 0;
}

void update_accel()
{
	float curr_dis = 0.0;
	float curr_accel_dis = 0.0;
	static short step_cnt=0;

	get_dis(&curr_dis);
	get_accel_dis(&curr_accel_dis);

	if(curr_dis < curr_accel_dis){
		update_desire_vel(ACCEL_STEP);
	}
	if((curr_dis >= curr_accel_dis && curr_dis < (270.0 - curr_accel_dis)) && step_cnt == 0){
		update_desire_vel(CONST_STEP);
	}
	if(curr_dis >= (270.0 - curr_accel_dis) || step_cnt == 1){
		update_desire_vel(DECEL_STEP);
		step_cnt = 1;
	}	
}

void update_accel2(short state)
{
	if(state == ACCEL_STEP){
		update_desire_vel(ACCEL_STEP);
	}
	if(state == CONST_STEP){
		update_desire_vel(ACCEL_STEP);
	}
	if(state == DECEL_STEP){
		update_desire_vel(DECEL_STEP);
	}
}

void calc_accel_dis()
{
	accel_dis = (desire_max_vel * desire_max_vel) / (2.0 * accel);
}

void set_max_vel(float val)
{
	desire_max_vel = val;
    calc_accel_dis();
}

void calc_vel(short enc_dis)
{
	short i;
	float tmp_vel = (float)enc_dis * MMPP / DELTA_T;

	// vel_pool[WINDOW_SIZE-1] = tmp_vel;
	// sum_vel += vel_pool[WINDOW_SIZE-1];
	// sum_vel -= vel_pool[0];

	// for(i=0; i<WINDOW_SIZE-1; i++){
	// 	vel_pool[i] = vel_pool[i+1];
	// }

	// curr_vel = sum_vel / WINDOW_SIZE;
	curr_vel = tmp_vel;
}
