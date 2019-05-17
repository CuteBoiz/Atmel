#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
int main()
{
	DDRD	= 0xFF;
	
	TWSR	= 0;
	TWBR	= 32; // Bit rate
	
	TWCR	= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
	PORTD = 0xFF;
	while(1);
}