#include "iodefine.h"
#include "machine.h"
#include "utility.h"

#define	START_CMT0 CMT.CMSTR0.BIT.STR0
#define	START_CMT1 CMT.CMSTR0.BIT.STR1
#define	START_CMT2 CMT.CMSTR1.BIT.STR2
unsigned long timer_count=0;

void timer_stop(short num)
{
	switch(num){
		case 0: START_CMT0 = 0; break;
		case 1: START_CMT1 = 0; break;
		case 2: START_CMT2 = 0; break;
	}
}

void timer_start(short num)
{
	switch(num){
		case 0: START_CMT0 = 1; break;
		case 1: START_CMT1 = 1; break;
		case 2: START_CMT2 = 1; break;
	}
}

void timer_wait(unsigned short ms)
{
	unsigned long TimeNow = timer_count;
	while((timer_count-TimeNow) <= ms );
}

unsigned long get_time()
{
	return timer_count;
}

short timer_free_wait(short wait_time)
{
	static short time_flag = 0;
	static unsigned long oTime = 0;
	unsigned long nTime = 0;
	
	if(time_flag == 0){
		time_flag = 1;
		oTime = get_time();
	}

	nTime = get_time();
	if(wait_time != -1){
		if (nTime - oTime > wait_time){
			time_flag = 0;
			oTime = 0;
			nTime = 0;
			return 1;
		}
	}
	else{
	}

	return 0;
}


void timer_call0()
{
	timer_count++;
	gyro_update();
	angle_update();

	acc_update();
}

void timer_call1()
{
	short ad_io[4];
	float curr_vel = 0.0;
	float curr_desire_vel = 0.0;

	/***計測***/
	calc_vel(get_vel_ENC());	// 速度を計算
	update_ENC_dis();			// トータルの走行距離更新

	get_sens_io(ad_io);
	set_sdata(0, ad_io[0]);
	set_sdata(1, ad_io[1]);
	set_sdata(2, ad_io[2]);
	set_sdata(3, ad_io[3]);

	/***制御***/
	PID_ctr();

	/***処理***/
	run_manager();
	reset_timer_enc();			// エンコーダリセット

	get_vel(&curr_vel);
	get_desire_vel(&curr_desire_vel);
	if(curr_desire_vel - curr_vel > 300.0){
		set_end_flag(0);	
	}
}

void timer_call2()
{
	run_sens_ad();
}

