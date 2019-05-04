/*
 * Keyboard.cpp
 *
 * Created: 03-May-19 10:14:24 PM
 * Author : tanph
 */ 

#include "KeyBoard.h"

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

void sendString(char *data){
	int i = 0;
	while(data[i] != 0x00){
		USART_Transmit(data[i]);
		i++;
	}
}

KeyBoard abc;

int main(void)
{
	USART_Init();
    sendString("Begin!\n");
	DDRC |= (1<<5);
	abc.begin();
	
	sei();
    while (1) 
    {
		PORTC |= (1<<5);
		if (abc.available()){
			char data = abc.read();
			USART_Transmit(data);
			if (data == 'f'){
				PORTC &= ~(1<<5);
				_delay_ms(500);
			}
		}
		
    }
}


