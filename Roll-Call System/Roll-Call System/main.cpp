#include "PS2.h"
#include "Nokia5110.h"

char *mySystem[] = {"1.RFID", "2.Setting"};
char *RFID[] = {"1.List","2.Add","3.Delete","4.Edit"};
	
void USART_Init();
void USART_Transmit(char data);
void USART_SendString(char *data);

PCD8544 lcd(4, 3, 2, 1 ,0);

PS2 kb;

int main(void)
{
	
	USART_Init();
	USART_SendString("Begin!\n");
	
	kb.begin();
	sei();
	
    lcd.begin();
	lcd.createMenu("My System:", mySystem, 2);	
	lcd.displayMenu();
    while (1) 
    {
		if (kb.available()){
			uint8_t key = kb.getChar();
			USART_Transmit(key);
			if (key == KB_UP){
				lcd.decreasePointer();
			}
			else if (key == KB_DOWN){
				lcd.increasePointer();	
			}
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
