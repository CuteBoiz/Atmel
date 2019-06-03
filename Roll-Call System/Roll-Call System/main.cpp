#include "PS2.h"
#include "Nokia5110.h"

char *mySystem[] = {"1.RFID", "2.Setting"};
char *RFID[] = {"1.Tags List","2.Add New Tag","    Back"};
char *Setting[] = {"1.Light", "2.Contrast", "     Back"};
char *List[] = {"1.Phat", "2.Vinh", "3.Tuan"};
void USART_Init();
void USART_Transmit(char data);
void USART_SendString(char *data);

void NextMenu();
void PrevMenu();

PCD8544 LCD(7, 3, 2, 1 ,0);

PS2 kb;

int main(void)
{
	DDRC |= (1<<5);
	USART_Init();
	USART_SendString("Begin!\n");
	
	kb.begin();
	sei();
	
    LCD.begin();
	LCD.createMenu(0, 0, 2, "My System", mySystem);	
	LCD.displayMenu();
    while (1) 
    {
		if (kb.available()){
			uint8_t key = kb.getChar();
			USART_Transmit(key);
			if (key == KB_UP){
				LCD.decreasePointer();
			}
			else if (key == KB_DOWN){
				LCD.increasePointer();	
			}
			else if ((key == '\n') || (key == KB_RIGHT)){
				NextMenu();
			}
			else if ((key == KB_ESC) || (key == KB_LEFT)){
				PrevMenu();
			}
			else{}
			LCD.clearSreen();
			LCD.displayMenu();
		}
    }
}

void USART_Init(){
	UBRRH = 0;
	UBRRL = 51;
	UCSRC |= (1<<URSEL)|(3<<UCSZ0);
	UCSRB |= (1<<TXEN);
}
	
void USART_Transmit(char data){
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
}

void USART_SendString(char *data){
	register int i = 0;
	while(data[i]){
		USART_Transmit(data[i]);
		i++;
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
			case 0: LCD.createMenu(0, 0, 3, "List", List); break;
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
		LCD.createMenu(0, 0, 2, "My System", mySystem);
	}
	else if (name == "Setting"){
		LCD.createMenu(1, 0, 2, "My System", mySystem);
	}
	else if (name == "List"){
		LCD.createMenu(0, 0, 2, "RFID", RFID);
	}
}