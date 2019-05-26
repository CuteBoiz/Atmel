/*
 * Nokia5110.cpp
 *
 * Created: 26-May-19 12:13:43 PM
 * Author : tanph
 */ 

#include "Nokia5110.h"

char Menu[][14] = {"1.Phat","2.Vinh", "3.Linh", "4.Tuan"};
PCD8544 LCD(4, 3, 2, 1, 0);


int main(void)
{
	DDRD &= ~((1<<0) | (1<<1));
	PORTD |= (1<<0) | (1<<1);
    LCD.begin();
	LCD.createMenu("RFID", Menu, 4);
	LCD.displayMenu();
    while (1) 
    {	
		if (!(PIND & (1<<0))){
			LCD.increasePointer();
			while(!(PIND & (1<<0)));
		}
		if(!(PIND & (1<<1))){
			LCD.decreasePointer();
			while(!(PIND & (1<<1)));
		}
		
    }
}

