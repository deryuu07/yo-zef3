#include "iodefine.h"
#include "machine.h"

#define LED1	PORTB.PODR.BIT.B0
#define LED2	PORTB.PODR.BIT.B1
#define LED3	PORTB.PODR.BIT.B3
#define LED4	PORTB.PODR.BIT.B5
#define LED_ON	 0
#define LED_OFF	 1

void LED_all_on()
{
	LED1 = LED_ON;
	LED2 = LED_ON;
	LED3 = LED_ON;
	LED4 = LED_ON;
}

void LED_reset()
{
	LED1 = LED_OFF;
	LED2 = LED_OFF;
	LED3 = LED_OFF;
	LED4 = LED_OFF;
}

void LED_manager(short num)
{
	switch(num){
		case 0: LED1 = LED_OFF; LED2 = LED_OFF; LED3 = LED_OFF; LED4 = LED_OFF; break;
		case 1: LED1 = LED_ON;  LED2 = LED_OFF; LED3 = LED_OFF; LED4 = LED_OFF; break;
		case 2: LED1 = LED_OFF; LED2 = LED_ON;  LED3 = LED_OFF; LED4 = LED_OFF; break;
		case 3: LED1 = LED_ON;  LED2 = LED_ON;  LED3 = LED_OFF; LED4 = LED_OFF; break;
		case 4: LED1 = LED_OFF; LED2 = LED_OFF; LED3 = LED_ON;  LED4 = LED_OFF; break;
		case 5: LED1 = LED_ON;  LED2 = LED_OFF; LED3 = LED_ON;  LED4 = LED_OFF; break;
		case 6: LED1 = LED_OFF; LED2 = LED_ON;  LED3 = LED_ON;  LED4 = LED_OFF; break;
		case 7: LED1 = LED_ON;  LED2 = LED_ON;  LED3 = LED_ON;  LED4 = LED_OFF; break;
		case 8: LED1 = LED_OFF; LED2 = LED_OFF; LED3 = LED_OFF; LED4 = LED_ON;  break;
	}
}

void LED_off(short num)
{
	switch(num){
		case 0: LED1 = LED_OFF; break;
		case 1: LED2 = LED_OFF; break;
		case 2: LED3 = LED_OFF; break;
		case 3: LED4 = LED_OFF; break;
	}
}

void LED_on(short num)
{
	switch(num){
		case 0: LED1 = LED_ON; break;
		case 1: LED2 = LED_ON; break;
		case 2: LED3 = LED_ON; break;
		case 3: LED4 = LED_ON; break;
	}
}

void finish_indicate()
{
	LED_reset();
	LED_on(0);	
	timer_wait(100);
	LED_on(1);	
	timer_wait(100);
	LED_on(2);	
	timer_wait(100);
	LED_on(3);	
	timer_wait(1000);
	LED_reset();
}

void count_down()
{
	int i=0;

	for(i=0; i<4; i++){
		LED_reset(); 
		timer_wait(100); 
		LED_all_on(); 
		timer_wait(200); 
	}
	timer_wait(800); 
	LED_reset(); 
	timer_wait(200);
}
