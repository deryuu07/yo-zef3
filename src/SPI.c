#include "machine.h"
#include "iodefine.h"

//! for debug
#define WHO_AM_I 0x75
//! accel
#define OUT_AX_H  0x3B
#define OUT_AX_L  0x3C
#define OUT_AY_H  0x3D
#define OUT_AY_L  0x3E
#define OUT_AZ_H  0x3F
#define OUT_AZ_L  0x40
//!gyro
#define OUT_X_H  0x43
#define OUT_X_L  0x44
#define OUT_Y_H  0x45
#define OUT_Y_L  0x46
#define OUT_Z_H  0x47
#define OUT_Z_L  0x48
//! spi port
#define CS       PORTA.PODR.BIT.B1
#define SDI      PORTA.PODR.BIT.B4
#define SCLK     PORTA.PODR.BIT.B3
#define SDO      PORTA.PIDR.BIT.B6
#define HIGH     1
#define LOW      0
#define _ICLK_		96000

void spi_wait_usec_(int t){
	int i;
	t = t*_ICLK_/5000-4;
	for(i=0;i<t;i++);
}

void write_data(unsigned short addr, unsigned short data)
{
	char i,j=0;	
	CS=1;
	SCLK=1;
	SDI=0;

	/***SPI start***/
	CS=0;
	spi_wait_usec_(10);

	/***RW指定***/
	SCLK=LOW;
	SDI=LOW;			//HIGH:read mode，LOW:write mode
	spi_wait_usec_(10);
	SCLK=HIGH;
	spi_wait_usec_(10);

	/***アドレス送信***/
	j=6;
	for(i=0; i<7; i++){
		SCLK=LOW;
		if((addr>>j) & 0x01 == 1){
			SDI=HIGH;
		}
		else{
			SDI=LOW;
		}
		spi_wait_usec_(10);
		SCLK=HIGH;
		spi_wait_usec_(10);
		j--;			
	}
		
	/***書き込み***/
	j=7;
	for(i=0; i<8; i++){
		SCLK=LOW;
		if((data>>j) & 0x01 == 1){
			SDI=HIGH;
		}
		else{
			SDI=LOW;
		}
		spi_wait_usec_(10);
		SCLK=HIGH;
		spi_wait_usec_(10);
		j--;			
	}
	CS=HIGH;
	SCLK=HIGH;
	SDI=LOW;
	
}

unsigned char read_data(unsigned short addr)
{
	char i,j=0;	
	unsigned char  tmp=0;
	unsigned char SPI_out=0;
	CS=1;
	SCLK=1;
	SDI=0;

	/***SPI start***/
	CS=0;
//	spi_wait_usec_(1);

	/***RW指定***/
	SCLK=LOW;
	SDI=HIGH;
//	spi_wait_usec_(1);
	SCLK=HIGH;
//	spi_wait_usec_(1);

	/***アドレス送信***/ //0111 0101
	j=6;
	for(i=0; i<7; i++){
		SCLK=LOW;
		if((addr>>j) & 0x01 == 1){
			SDI=HIGH;
		}
		else{
			SDI=LOW;
		}
//		spi_wait_usec_(1);
		SCLK=HIGH;
//		spi_wait_usec_(1);
		j--;			
	}
		
	/***読出し***/
	SPI_out=0;
	for(j=0; j<8; j++){	
		SCLK=LOW;
//		spi_wait_usec_(1);
		SCLK=HIGH;
		tmp=SDO;
		SPI_out=SPI_out|tmp;
		//最後のデータはソフトしない
		if(j!=7){
			SPI_out=SPI_out<<1;
		}
//		spi_wait_usec_(1);
		tmp=0;
	}
	CS=HIGH;
	SCLK=HIGH;
	SDI=LOW;
	
	return SPI_out;
}

signed short get_accelarator(short axis)
{
	unsigned char out_L, out_H=0;
	signed short out_data=0;

	switch(axis){
	case 0:
		out_H=read_data(OUT_AX_H);
		out_L=read_data(OUT_AX_L);
		break;
	case 1:
		out_H=read_data(OUT_AY_H);
		out_L=read_data(OUT_AY_L);
		break;
	case 2:
		out_H=read_data(OUT_AZ_H);
		out_L=read_data(OUT_AZ_L);
		break;
	}

	out_data=(signed short)out_H;
	out_data=out_data<<8;
	out_data=out_data|(signed short)out_L;

	return out_data;
}

signed short get_gyro(short axis)
{
	unsigned char out_L, out_H=0;
	signed short out_data=0;

	switch(axis){
	case 0:
		out_H=read_data(OUT_X_H);
//		spi_wait_usec_(1);
		out_L=read_data(OUT_X_L);
		break;
	case 1:
		out_H=read_data(OUT_Y_H);
//		spi_wait_usec_(1);
		out_L=read_data(OUT_Y_L);
		break;
	case 2:
		out_H=read_data(OUT_Z_H);
//		spi_wait_usec_(1);
		out_L=read_data(OUT_Z_L);
		break;
	}

	out_data=(signed short)out_H;
	out_data=out_data<<8;
	out_data=out_data|(signed short)out_L;

	return out_data;
}

void gyro_init()
{
	short init_test;

	write_data (0x6B, 0x00); 
    spi_wait_usec_ (100);
    write_data (0x1B, 0x18); //gyro 2000deg/s
    spi_wait_usec_ (100);
    write_data (0x1C, 0x18); //acc 16g

	init_test = read_data(WHO_AM_I);
// 	print_num(init_test, 4);
// 	print_str("\r\n");
}