#include "PS2.h"

#include "Nokia5110.h"

	
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
	char *Menu[] = {"1.RFID", "2.Setting", "3.About"};
	lcd.MenuInit();
	lcd.displayMenu("My system", Menu, 3);	
    while (1) 
    {
		if (kb.available()){
			uint8_t data = kb.getChar();
			USART_Transmit(data);
			if (data == KB_UP){
				lcd.decreasePointer();
				lcd.displayMenu("My system", Menu, 3);	
			}
			else if (data == KB_DOWN){
				lcd.increasePointer();
				lcd.displayMenu("My system", Menu, 3);	
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
