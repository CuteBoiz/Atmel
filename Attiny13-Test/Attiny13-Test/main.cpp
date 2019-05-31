
#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void uart_init(){
	DDRB |= (1<<3);
	PORTB |= (1<<3);
}

void makeDelay(long b){
	for (int i = 0; i < b; i++);
}
void bit_bang_tx(uint8_t data){
	PORTB &= ~(1<<3);
	makeDelay(103);
	
	for (int i = 0; i < 8; i++){
		if ((data >> (7-i)) & 0x01){
			PORTB |= (1<<3);
		}
		else{ 
			PORTB &= ~(1<<3);
		}
		makeDelay(103);
	}
	PORTB |= (1<<3);
	makeDelay(103);
}

int main(void)
{
	DDRB |= (1>>1);
	PORTB |= (1<<1);
	uart_init();
	while (1){
		PORTB &= ~(1<<1);
		makeDelay(1000000);
		bit_bang_tx(0xE3);
		PORTB |= (1<<1);
		makeDelay(1000000);
	}
}