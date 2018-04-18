#include <stdio.h>
#include "utility.h"
//#define DELTA_T 0.001

float gyro = 0;
float gyro_angle = 0.0; // ∫omega
float gyro_vel = 0.0;   // ∫accel
float gyro_dis = 0.0;   // ∫gyro_vel

float desire_gyro = 0.0;
float desire_max_gyro = 0.0;
float gyro_accel = 0.0;

void reset_angle()
{
	gyro_angle = 0.0;
}

void get_ang_vel(float *curr_gyro)
{
    if(curr_gyro != NULL)
		*curr_gyro = gyro;
}

void get_angle(float *curr_angle)
{
    if(curr_angle != NULL)
		*curr_angle = gyro_angle;
}

void get_desire_gyro(float *curr_desire)
{
    if(curr_desire != NULL)
		*curr_desire = desire_gyro;
}

void get_gyro_accel(float *curr_accel)
{
    if(curr_accel != NULL)
		*curr_accel = gyro_accel;
}

void gyro_update()
{
	gyro = (float)get_gyro(2) / 16.4;
}

void angle_update()
{
	gyro_angle += (gyro * 0.001);
}

void set_max_gyro(float value)
{
	desire_max_gyro = value;
}

short update_desire_gyro(short run_state)
{
	if(run_state == LEFT_SLALOM){
		if(gyro_angle < 66.7){
			if(desire_gyro < desire_max_gyro){
				gyro_accel = 6500.0;
				desire_gyro += gyro_accel*DELTA_T; 
			}
			else{
				gyro_accel = 0.0;
				desire_gyro = desire_max_gyro; 
			}
		}
		else{
			if(desire_gyro > 0.0){
				gyro_accel = -6500.0;
				desire_gyro += gyro_accel*DELTA_T; 
			}
			else{
				gyro_accel = 0.0;
				desire_gyro = 0.0; 
				return 1;
			}
		}
	}
	if(run_state == RIGHT_SLALOM){
		if(gyro_angle > -66.7){
			if(desire_gyro > -desire_max_gyro){
				gyro_accel = -6500.0;
				desire_gyro += gyro_accel*DELTA_T; 
			}
			else{
				gyro_accel = 0.0;
				desire_gyro = -desire_max_gyro; 
			}
		}
		else{
			if(desire_gyro < 0.0){
				gyro_accel = 6500.0;
				desire_gyro += gyro_accel*DELTA_T; 
			}
			else{
				gyro_accel = 0.0;
				desire_gyro = 0.0; 
				return 1;
			}
		}		
	}
	if(run_state == U_TURN){
		if(gyro_angle < 144.5){
			if(desire_gyro < 800.0){
				desire_gyro += 9000.0*DELTA_T; 
			}
			else{
				desire_gyro = 800.0; 
			}
		}
		else{
			if(desire_gyro > 0.0){
				desire_gyro -= 9000.0*DELTA_T; 
			}
			else{
				desire_gyro = 0.0; 
				return 1;
			}
		}
	}

	return 0;
//	desire_gyro = 0.0; 
}