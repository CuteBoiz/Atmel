#include "PS2.h"

void USART_Init(){
	UBRRH = 0;
	UBRRL = 51;
	UCSRC |= (1<<URSEL)|(3<<UCSZ0);
	UCSRB |= (1<<TXEN);
}

void USART_Transmit(char data){
	while (!(UCSRA & (1<<UDRE)));
	UDR = data;
}

void USART_SendString(char *data){
	register int i = 0;
	while (data[i]){
		USART_Transmit(data[i]);
		i++;
	}
}

PS2 kb;

int main(void){
	USART_Init();
	kb.begin();
	USART_SendString("Begin!\n");
	sei();
	while (1)
	{
		if (kb.available()){
			USART_Transmit(kb.getChar());
		}
	}
}
