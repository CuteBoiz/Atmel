/*
 * Attiny13-Test.cpp
 *
 * Created: 07-May-19 6:31:39 PM
 * Author : tanph
 */ 

//#include "PS2.h"
#define F_CPU 12000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DDR_USART	DDRB
#define PORT_USART	PORTB
#define USART_TX_PIN	2
#define USART_DELAY		14 //9600

void USART_Init(){
	DDR_USART	|= (1<<USART_TX_PIN);
	PORT_USART	|= (1<<USART_TX_PIN);
}

void USART_Transmit(uint8_t data){
	register int i;
	TCNT0 = 0;
	//Send Start Bit
	PORT_USART &= ~(1<<USART_TX_PIN);
	while (TCNT0 < USART_DELAY);
	
	//Send Data Bits
	for (int i = 0; i < 8; i++){
		if (data & (1 << i)){
			PORT_USART |= (1<<USART_TX_PIN);
		}
		else{
			PORT_USART &= ~(1<<USART_TX_PIN);
		}
		while(TCNT0 < USART_DELAY);
	}
	
	//Send Stop Bit
	PORT_USART &= ~(1<<USART_TX_PIN);
	while (TCNT0 < USART_DELAY);
	
}
int main(void)
{
	
	TCCR0A = 0x02;
	TCCR0B = 0x02;
	OCR0A = USART_DELAY; 
	
	USART_Init();
	
	sei();
    DDRB |= (1<<4);
    while (1) 
    {
		PORTB &= ~(1<<4);
		USART_Transmit(0xF1);
		_delay_ms(300);
		PORTB |= (1<<4);
		_delay_ms(300);
    }
}

