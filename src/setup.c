/*
 * setup.c
 *
 *  Created on: 2017/06/26
 *      Author: nomura
 */
#include "machine.h"
#include "iodefine.h"

void Clock_initialize(){
	volatile unsigned int i;

	SYSTEM.PRCR.WORD = 0xA50B;			// プロテクトレジスタ解除

	// 入力クロック12MHz
	SYSTEM.PLLCR.BIT.PLIDIV	= 0x00;		// PLL入力分周比(1分周)    = 12MHz
	SYSTEM.PLLCR.BIT.STC	= 0x0F;		// PLL周波数逓倍率(16逓倍) = 12 * 16 = 192
	SYSTEM.SCKCR2.BIT.UCK	= 0x03;		// USBクロック(4分周) = 192/4 = 48MHz
	SYSTEM.SCKCR2.BIT.IEBCK	= 0x03;		// IECLK = 192/8 = 24MHz

	SYSTEM.MOSCCR.BYTE = 0x00;			// メインクロック発振動作
	SYSTEM.PLLCR2.BYTE = 0x00;			// PLL回路を動作開始

	for(i = 0;i< 0x14A;i++)				// wait over 11ms
	{
		;
	}

	SYSTEM.SCKCR.LONG = 0x21022211;		// FCK:Flashクロック			= 192 / 4 = 48MHz
										// ICK:システムクロック			= 192 / 2 = 96MHz
										// BCK:外部バスクロック			= 192 / 4 = 48MHz
										// PCKA:周辺モジュールクロックA	= 192 / 4 = 48MHz
										// PCKB:周辺モジュールクロックB	= 192 / 4 = 48MHz

	SYSTEM.SCKCR3.WORD = 0x0400;		// PLL選択
}

void CMT_initialize()
{
	MSTP_CMT0 = 0;				//CMT モジュールスタンバイ解除
	MSTP_CMT2 = 0;				//CMT モジュールスタンバイ解除

	CMT0.CMCR.BIT.CMIE = 1; 	//CMTの割り込み許可
	CMT1.CMCR.BIT.CMIE = 1; 	//CMTの割り込み許可
	CMT2.CMCR.BIT.CMIE = 1; 	//CMTの割り込み許可

	CMT0.CMCR.BIT.CKS = 0;  /*	周辺モジュールクロック（PCLK）を分周して得られるカウントクロックを選択
								φ/8 = 6MHz
								0 0：PCLK/8
								0 1：PCLK/32
								1 0：PCLK/128
								1 1：PCLK/512															*/
	CMT1.CMCR.BIT.CKS = 0;  /*	周辺モジュールクロック（PCLK）を分周して得られるカウントクロックを選択
								φ/8 = 6MHz
								0 0：PCLK/8
								0 1：PCLK/32
								1 0：PCLK/128
								1 1：PCLK/512															*/
	CMT2.CMCR.BIT.CKS = 0;  /*	周辺モジュールクロック（PCLK）を分周して得られるカウントクロックを選択
								φ/8 = 6MHz
								0 0：PCLK/8
								0 1：PCLK/32
								1 0：PCLK/128
								1 1：PCLK/512															*/

	CMT0.CMCOR = 6000-1;      	//1ms周期で割り込み
	CMT1.CMCOR = 6000-1;      	//1ms周期で割り込み
	CMT2.CMCOR = 6000-1;      	//1ms周期で割り込み

	ICU.IER[0x03].BIT.IEN4 = 1;	//割り込み要求許可
	ICU.IER[0x03].BIT.IEN5 = 1;	//割り込み要求許可
	ICU.IER[0x03].BIT.IEN6 = 1;	//割り込み要求許可

	ICU.IPR[0x04].BIT.IPR = 11;	//割り込み優先度を10に設定
	ICU.IPR[0x05].BIT.IPR = 10;	//割り込み優先度を10に設定
	ICU.IPR[0x06].BIT.IPR = 9;	//割り込み優先度を10に設定

	CMT.CMSTR0.BIT.STR0 = 0;	//CMT0カウントスタート
	CMT.CMSTR0.BIT.STR1 = 0; 	//CMT1カウントスタート
	CMT.CMSTR1.BIT.STR2 = 0; 	//CMT2カウントスタート
}

void AD_initialize()
{
	SYSTEM.MSTPCRA.BIT.MSTPA17=0;	//12ビットA/D変換の使用を許可
	S12AD.ADCSR.BIT.ADST=0;			//A/D変換停止
	S12AD.ADCSR.BIT.CKS=3;			//変換クロックを96MHzに設定
	S12AD.ADCSR.BIT.ADCS=0;			//シングルモード
	S12AD.ADANS0.BIT.ANS0=0x03fe;	//AN001とAN013をA/D変換の対象とする
}

void MTU_initialize(){

	SYSTEM.MSTPCRA.BIT.MSTPA9=0;
	// Set MPC
	PORT1.PMR.BIT.B7 = 1;			// Set P17: Peripheral
	PORT1.PMR.BIT.B6 = 1;			// Set P16: Peripheral
	MPC.PWPR.BIT.B0WI = 0;			// Release protect
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P17PFS.BIT.PSEL = 1;		// Set P17: MTIOC3A
	MPC.P16PFS.BIT.PSEL = 1;		// Set P16: MTIOC3C
	MPC.PWPR.BIT.PFSWE = 0;			// Protect
	MPC.PWPR.BIT.B0WI = 1;
	// PWM Settings
	MTU.TOER.BIT.OE3B = 1;			// Enable MTIOC3A Output
	MTU.TOER.BIT.OE3D = 1;			// Enable MTIOC3A Output
	MTU3.TCR.BIT.TPSC = 0;			// PCLK/256
	MTU3.TCR.BIT.CKEG = 0;			// Count rising edge
	MTU3.TCR.BIT.CCLR = 1;			// Clear TCNT count when compare match TGRA
	MTU3.TMDR.BIT.MD = 2;			// MTU3 PWM mode1
	MTU3.TMDR.BIT.BFA = 0;			// TGRA, TGRC normal mode
	MTU3.TMDR.BIT.BFB = 0;			// TGRB, TGRD normal mode
//	MTU3.TIORH.BIT.IOA = 5;			// Compare Output High
//	MTU3.TIORH.BIT.IOB = 6;			// Compare Output Low
//	MTU3.TIORL.BIT.IOC = 5;			// Compare Output High
//	MTU3.TIORL.BIT.IOD = 6;			// Compare Output Low
	MTU3.TIORH.BIT.IOA = 6;			// Compare Output High
	MTU3.TIORH.BIT.IOB = 5;			// Compare Output Low
	MTU3.TIORL.BIT.IOC = 6;			// Compare Output High
	MTU3.TIORL.BIT.IOD = 5;			// Compare Output Low
	MTU3.TGRA = 1499;					// 1kHz
	MTU3.TGRB = 0;				//
	MTU3.TGRC = 1499;					//
	MTU3.TGRD = 0;				//
	MTU3.TCNT = 0;					// Clear MTU3 count
	MTU.TSTR.BIT.CST3 = 1;			// Start MTU3 count
}

void encoder_initialize()
{
	SYSTEM.MSTPCRA.BIT.MSTPA9=0;	//MTU2モジュールの使用を許可
	PORT1.PDR.BIT.B4 = 0;			//ポートA4 入力に設定	ENC_L
	PORT1.PDR.BIT.B5 = 0;			//ポートA6 入力に設定	ENC_L
	PORTC.PDR.BIT.B4 = 0;			//ポートA1 入力に設定	ENC_R
	PORTC.PDR.BIT.B5 = 0;			//ポートA3 入力に設定	ENC_R

	PORT1.PMR.BIT.B4 = 1;			//ポートの端子機能を周辺機能として使用
	PORT1.PMR.BIT.B5 = 1;			//ポートの端子機能を周辺機能として使用
	PORTC.PMR.BIT.B4 = 1;			//ポートの端子機能を周辺機能として使用
	PORTC.PMR.BIT.B5 = 1;			//ポートの端子機能を周辺機能として使用

	MPC.PWPR.BIT.B0WI=0;			//PFSWEビット書き込み許可
	MPC.PWPR.BIT.PFSWE=1;			//PFSレジスタ書き込み許可
	MPC.P14PFS.BIT.PSEL=2;			//P14をMTCLKAとして使用する
	MPC.P15PFS.BIT.PSEL=2;			//P15をMTCLKBとして使用する
	MPC.PC4PFS.BIT.PSEL=2;			//P22をMTCLKCとして使用する
	MPC.PC5PFS.BIT.PSEL=2;			//P23をMTCLKDとして使用する
	MPC.PWPR.BIT.PFSWE=0;			//PFSWEビット書き込み禁止
	MPC.PWPR.BIT.B0WI=1;			//PFSレジスタ書き込み禁止

	MTU.TSTR.BIT.CST1=0;			//カウント停止
	MTU.TSTR.BIT.CST2=0;			//カウント停止

	MTU1.TMDR.BIT.MD=4;				//MTU1の位相計数モードで左エンコーダを使用
	MTU2.TMDR.BIT.MD=4;				//MTU2の位相計数モードで右エンコーダを使用

	MTU1.TCNT=0;
	MTU2.TCNT=0;

	MTU.TSTR.BIT.CST1=1;			//カウントスタート
	MTU.TSTR.BIT.CST2=1;			//カウントスタート
}

void IO_initialize()
{
	/***LED***/
	PORTB.PDR.BIT.B0 = 1;
	PORTB.PDR.BIT.B1 = 1;
	PORTB.PDR.BIT.B3 = 1;
	PORTB.PDR.BIT.B5 = 1;

	/***赤外線LED***/
	PORT4.PDR.BIT.B2 = 1;
	PORT4.PDR.BIT.B6 = 1;

	/***モータドライバ***/
	PORT3.PDR.BIT.B1 = 1;
	PORTC.PDR.BIT.B7 = 1;

	/***Gyro***/
	PORTA.PDR.BIT.B1 = 1;		// �|�[�g �o��
	PORTA.PDR.BIT.B4 = 1;		// �|�[�g �o��
	PORTA.PDR.BIT.B3 = 1;		// �|�[�g �o��
	PORTA.PDR.BIT.B6 = 0;		// �|�[�g ����

}

void hardware_initialize()
{
	Clock_initialize();
	CMT_initialize();
	AD_initialize();
	MTU_initialize();
	encoder_initialize();
	SCI_initialize();
	IO_initialize();

	gyro_init();
}
