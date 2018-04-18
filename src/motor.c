#include "iodefine.h"
#include "machine.h"
#include "utility.h"

#define MOT_L 	  MTU3.TGRB
#define MOT_R 	  MTU3.TGRD
#define	MOT_CTR_L PORTC.PODR.BIT.B7
#define	MOT_CTR_R PORT3.PODR.BIT.B1
#define CW 0
#define CCW 1
#define PWM_MIN 1.0
#define PWM_MAX 1499.0

#define FF_R_a 0.517
#define FF_R_b 224.0
#define FF_L_a 0.611
#define FF_L_b 158.0

float vel_sum_err = 0.0;
float vel_pre_val = 0.0;
float gyro_sum_err = 0.0;
float gyro_pre_val = 0.0;
short Uturn_flag = 0;

void set_Uturn_flag(short num)
{
	Uturn_flag = num;
}

void reset_gyro_I()
{
	gyro_sum_err = 0.0;
	gyro_pre_val = 0.0;
}

void reset_vel_I()
{
	vel_sum_err = 0.0;
	vel_pre_val = 0.0;
}

void mot_drive(short mot_r, short mot_l)
{
	if(mot_r > 0){
		MOT_CTR_R = CW;
		MOT_R = mot_r;
	}
	if(mot_r <= 0){
		MOT_CTR_R = CCW;
		MOT_R = -1*mot_r;		
	}	

	if(mot_l > 0){
		MOT_CTR_L = CW;
		MOT_L = mot_l;
	}
	if(mot_l <= 0){
		MOT_CTR_L = CCW;
		MOT_L = -1*mot_l;		
	}	
}

void vel_ctr(float *u_vel_R, float * u_vel_L)
{
	float u_r, u_l;
	float u_ff, u_ff_a, u_ff_v;

	float err, pre_err = 0.0;
	float FB_gain[3];
	float FF_para[2];

	float curr_vel = 0.0;
	float curr_desire_vel = 0.0;
	float curr_accel = 0.0;

	float dummy = 5.0;

	get_vel(&curr_vel);
//	get_acc_vel_xf(&curr_vel);
	get_desire_vel(&curr_desire_vel);
	get_accel(&curr_accel);
	set_fdata(0, curr_vel);
	set_fdata(1, curr_desire_vel);

	//FB
	vel_pre_val = err;
	err = curr_vel - curr_desire_vel;
	vel_sum_err += err;
	pre_err = err - vel_pre_val;
	get_vel_FB_gain(FB_gain);
	u_r = -FB_gain[0]*err - FB_gain[1]*vel_sum_err - FB_gain[2]*pre_err;
	u_l = u_r;

	//FF
	get_vel_FF_para(FF_para);
	u_ff_v = FF_para[0]*curr_desire_vel + FF_para[1];
	u_ff_a = curr_accel;
	get_vel_FF_gain(FF_para);
	// u_ff = FF_para[0]*u_ff_a + FF_para[1]*u_ff_v;
	// u_r += u_ff;
	// u_l += u_ff;
	u_r += (FF_para[0]*u_ff_a + FF_para[1]*330.0);
	u_l += (FF_para[0]*u_ff_a + FF_para[1]*370.0);

	*u_vel_R = u_r;
	*u_vel_L = u_l;
}

void gyro_ctr(float *u_gyro)
{
	float curr_gyro = 0.0;
	float curr_desire_gyro = 0.0;
	float curr_gyro_accel = 0.0;
	float u = 0.0;
	float err, pre_err = 0.0;
	float para[3];

	get_ang_vel(&curr_gyro);
	get_desire_gyro(&curr_desire_gyro);
	set_fdata(2, curr_gyro);
	set_fdata(3, curr_desire_gyro);

	gyro_pre_val = err;
	err = curr_gyro - curr_desire_gyro;	
	gyro_sum_err += err;
	pre_err = err - gyro_pre_val;

	get_gyro_FB_gain(para);
	u = para[0]*err + para[1]*gyro_sum_err + para[2]*pre_err;

	get_gyro_accel(&curr_gyro_accel);
	u -= 0.02*curr_gyro_accel;
	u -= 0.25*curr_gyro;

	*u_gyro = u;
}

void PID_ctr()
{
	short sbatt;
	float batt;
	float u_r, u_l = 0.0;
	float u_gyro = 0.0;
	float u_vel_r, u_vel_l = 0.0;
	short mot_r, mot_l = 0;
	float ff_para[2];
	float debug_para[4];

	sbatt = get_batt();
	batt = (float)sbatt;
	
	/***速度制御***/
	vel_ctr(&u_vel_r, &u_vel_l);
	u_r += u_vel_r;
	u_l += u_vel_l;

	/***姿勢制御***/
	gyro_ctr(&u_gyro);
	u_r -= u_gyro;
	u_l += u_gyro;

	// get_debug_para(&debug_para);
	// u_r = debug_para[2];
	// u_l = debug_para[3];

	u_r *= PWM_MAX / batt;
	u_l *= PWM_MAX / batt;

	mot_r = (short)u_r;
	mot_l = (short)u_l;

	mot_drive(mot_r, mot_l);
}
