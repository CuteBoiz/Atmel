/*
 * Nokia G5110.cpp
 *
 * Created: 22-Apr-19 10:37:40 PM
 * Author : tanph
 */ 

#include "N5110.h"

uint8_t line[84];

Nokia5110 lcd;

int main(void)
{	
	DDRC |= (1 << 5);
	PORTC |= (1 << 5);
	lcd.begin();
	while(1){
		lcd.setCursor(0 ,0);
		lcd.fillUp("PHAT");
		lcd.setCursor(0 ,1);
		lcd.fillUp("VINH NGU");
		lcd.setCursor(0, 3);
		lcd.fillUp("Bo Linh");
		_delay_ms(3000);
	}
}
