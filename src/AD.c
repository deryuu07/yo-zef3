#include "iodefine.h"
#include "machine.h"
#include <stdio.h>

#define S_LED_L	PORT4.PODR.BIT.B2
#define S_LED_R	PORT4.PODR.BIT.B6

#define SENS_LF 0
#define SENS_LS 1
#define SENS_RS 2
#define SENS_RF 3

#define min0 948
#define max0 1350
#define io0_thre 1000

#define min1 1807
#define max1 2217
#define io1_thre 600

#define min2 1760
#define max2 2135
#define io2_thre 600

#define min3 1842
#define max3 2279
#define io3_thre 1000

#define LED_SW_TH 3300

short sens_ad[4];
short sens_dis[4];

short check_batt()
{
	short batt = 0;

	if(batt > 3100){ return 0; }
	else		   { return 1;}
}

short get_batt()
{
	short batt;

	S12AD.ADANS0.BIT.ANS0=0x0002;
	S12AD.ADCSR.BIT.ADST = 1;					//A/D�ϊ��J�n
	while (S12AD.ADCSR.BIT.ADST == 1);			//A/D�ϊ����I���܂ő҂�
	batt = S12AD.ADDR1;

	return (batt*3000/4096) * 2;
}

void get_sens_io(short *ad_io)
{
	if(ad_io != NULL){
		if(sens_dis[0] >  80){ ad_io[0] = 0;} //left front
		if(sens_dis[0] <= 80){ ad_io[0] = 1;} 
			   
		if(sens_dis[1] >  40){ ad_io[1] = 0;} //left side
		if(sens_dis[1] <= 40){ ad_io[1] = 1;}
			   
		if(sens_dis[2] >  80){ ad_io[2] = 0;} //right front
		if(sens_dis[2] <= 80){ ad_io[2] = 1;}
				   
		if(sens_dis[3] >  40){ ad_io[3] = 0;} //right side
		if(sens_dis[3] <= 40){ ad_io[3] = 1;}			   
	}
}

void calc_sens_dis()
{
    float cali_sens = 0.0;
    float tmp_sens = 0.0;

	tmp_sens = (float)(sens_ad[0]);
	cali_sens = -0.00000001026*tmp_sens*tmp_sens*tmp_sens +
				   0.0000717*tmp_sens*tmp_sens -
				   0.159*tmp_sens + 187.17;
	sens_dis[0] = (short)cali_sens - 60;

	tmp_sens = (float)(sens_ad[1]);
	cali_sens = -0.000001125*tmp_sens*tmp_sens*tmp_sens +
				   0.00145*tmp_sens*tmp_sens -
				   0.623*tmp_sens + 114.0;
	sens_dis[1] = (short)cali_sens - 20;

	tmp_sens = (float)(sens_ad[2]);
	cali_sens = -0.00000000929*tmp_sens*tmp_sens*tmp_sens +
				   0.0000642*tmp_sens*tmp_sens -
				   0.14*tmp_sens + 169.8;
	sens_dis[2] = (short)cali_sens - 60;

	tmp_sens = (float)(sens_ad[3]);
	cali_sens = -0.000000228*tmp_sens*tmp_sens*tmp_sens +
				   0.000479*tmp_sens*tmp_sens -
				   0.337*tmp_sens + 103.9;
	sens_dis[3] = (short)cali_sens - 20;
}

void run_sens_ad()
{
	short i;
	short ad_off[4];

	S_LED_L=0;
	S_LED_R=0;

	S12AD.ADANS0.BIT.ANS0=0x0400;				//LS
	S12AD.ADCSR.BIT.ADST = 1;					//A/D�ϊ��J�n
	while (S12AD.ADCSR.BIT.ADST == 1);			//A/D�ϊ����I���܂ő҂�
	ad_off[1] = S12AD.ADDR10;

	S12AD.ADANS0.BIT.ANS0=0x0200;				//LF
	S12AD.ADCSR.BIT.ADST = 1;					//A/D�ϊ��J�n
	while (S12AD.ADCSR.BIT.ADST == 1);			//A/D�ϊ����I���܂ő҂�
	ad_off[0] = S12AD.ADDR9;

	S12AD.ADANS0.BIT.ANS0=0x0800;				//RF
	S12AD.ADCSR.BIT.ADST = 1;					//A/D�ϊ��J�n
	while (S12AD.ADCSR.BIT.ADST == 1);			//A/D�ϊ����I���܂ő҂�
	ad_off[3] = S12AD.ADDR11;

	S12AD.ADANS0.BIT.ANS0=0x1000;				//RS
	S12AD.ADCSR.BIT.ADST = 1;					//A/D�ϊ��J�n
	while (S12AD.ADCSR.BIT.ADST == 1);			//A/D�ϊ����I���܂ő҂�
	ad_off[2] = S12AD.ADDR12;

	S_LED_L=1;
	for(i=0; i<800; i++){}
	S12AD.ADANS0.BIT.ANS0=0x0400;				//LS
	S12AD.ADCSR.BIT.ADST = 1;					//A/D�ϊ��J�n
	while (S12AD.ADCSR.BIT.ADST == 1);			//A/D�ϊ����I���܂ő҂�
	sens_ad[1] = S12AD.ADDR10 - ad_off[1];
	S12AD.ADANS0.BIT.ANS0=0x0200;				//LF
	S12AD.ADCSR.BIT.ADST = 1;					//A/D�ϊ��J�n
	while (S12AD.ADCSR.BIT.ADST == 1);			//A/D�ϊ����I���܂ő҂�
	sens_ad[0] = S12AD.ADDR9 - ad_off[0];
	S_LED_L=0;

	S_LED_R=1;
	for(i=0; i<800; i++){}
	S12AD.ADANS0.BIT.ANS0=0x0800;				//RF
	S12AD.ADCSR.BIT.ADST = 1;					//A/D�ϊ��J�n
	while (S12AD.ADCSR.BIT.ADST == 1);			//A/D�ϊ����I���܂ő҂�
	sens_ad[3] = S12AD.ADDR11 - ad_off[3];
	S12AD.ADANS0.BIT.ANS0=0x1000;				//RS
	S12AD.ADCSR.BIT.ADST = 1;					//A/D�ϊ��J�n
	while (S12AD.ADCSR.BIT.ADST == 1);			//A/D�ϊ����I���܂ő҂�
	sens_ad[2] = S12AD.ADDR12 - ad_off[2];
	S_LED_R=0;

	calc_sens_dis();
}

void get_sens_dis(short *ad)
{
	if(ad != NULL){
		ad[0] = sens_dis[0];
		ad[1] = sens_dis[1];
		ad[2] = sens_dis[2];
		ad[3] = sens_dis[3];
	}
}

void get_sens_ad(short *ad)
{
	if(ad != NULL){
		ad[0] = sens_ad[0];
		ad[1] = sens_ad[1];
		ad[2] = sens_ad[2];
		ad[3] = sens_ad[3];
	}
}

short LED_switch()
{
	short ad[4];

	run_sens_ad();
	get_sens_ad(ad);
	if(ad[SENS_RS] > LED_SW_TH){
		return 0;
	}
	else{
		return 1;
	}
}

