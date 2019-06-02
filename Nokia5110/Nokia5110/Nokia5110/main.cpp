/*
 * Nokia5110.cpp
 *
 * Created: 26-May-19 12:13:43 PM
 * Author : tanph
 */ 

#include "Nokia5110.h"

char *Menu[] = {"1.RFID","2.Setting"};
char *RFID[] = {"1.List","2.Add Tag","3.Remove Tag","4.Edit Info","    Back"};
char *Setting[] = {"1.Light", "2.Contrast", "    Back"};
PCD8544 LCD(4, 3, 2, 1, 0);

void NextMenu();
void PrevMenu();

int main(void)
{
	DDRD &= ~((1<<0) | (1<<1));
	PORTD |= (1<<0) | (1<<1);
    LCD.begin();
	LCD.createMenu(0, 0, 2, "My System", Menu);
	
    while (1) 
    {	
		LCD.displayMenu();
		_delay_ms(500);
		while ((PIND & (1<<0)) && (PIND & (1<<1)) && (PIND & (1<<2)) && (PIND & (1<<3)));
		if (!(PIND & (1<<0))){
			LCD.increasePointer();
			while(!(PIND & (1<<0)));
		}
		if(!(PIND & (1<<1))){
			LCD.decreasePointer();
			while(!(PIND & (1<<1)));
		}
		if (!(PIND & (1<<2))){
			NextMenu();
			while(!(PIND & (1<<2)));
		}
		if (!(PIND & (1<<3))){
			
			while(!(PIND & (1<<3)));
		}
    }
}

void NextMenu(){
	char *name  = LCD.getTitle();
	if (name == "My System"){
		switch (LCD.getPointer()){
			case 0: LCD.createMenu(0, 0, 5,"RFID", RFID); break;
			case 1: LCD.createMenu(0, 0, 3, "Setting", Setting); break;
		}
	}
	else if (name == "RFID"){
		switch (LCD.getPointer()){
			case 0: break;
			case 1: break;
			case 2: break;
			case 3: break;
			case 4: PrevMenu(); break;
		}
	}
	else if (name == "Setting"){
		switch(LCD.getPointer()){
			case 0: break;
			case 1: break;
			case 2: PrevMenu(); break;
		}
	}
}

void PrevMenu(){
	char *name  = LCD.getTitle();
	if (name == "RFID"){
		LCD.createMenu(0, 0, 2, "My System", Menu);
	}
	else if (name == "Setting"){
		LCD.createMenu(1, 0, 2, "My System", Menu);
	}
}