/*
 * Test.cpp
 *
 * Created: 22-Apr-19 10:30:43 PM
 * Author : tanph
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRC |= (1<<5);
    while (1) 
    {
		PORTC |= (1<<5);
		_delay_ms(500);
		PORTC &= ~(1<<5);
		_delay_ms(500);
    }
}

