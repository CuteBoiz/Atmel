#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

void INT0_Init(){
	MCUCR |= (1<<ISC01);
	GICR |= (1<<INT0);
}

volatile uint8_t	count;
uint8_t				currentChar; //Hex code every pressed
volatile uint8_t	charReceived; //A char received

void kb_Init(){
	DDRD &= ~((1<<3));
	PORTD |= (1<<3);
	count = 0;
	currentChar = 0;
	charReceived = 0;
}

int main(void){
	USART_Init();
	INT0_Init();
	kb_Init();
	USART_SendString("Begin!\n");
	sei();
	while (1)
	{
		
	}
}

ISR(INT0_vect){
	 //Get Data bit
	 register uint8_t value;
	 if (PIND & (1<<3)){
		 value = 0x01;
	 }
	 else{
		 value = 0x00;
	 }
	 //Add to complete byte
	 if (count > 0 && count < 9){
		 currentChar |= (value << (count - 1));
	 }
	 count ++;
	 if (count == 11){
		 USART_Transmit(currentChar);
		 currentChar = 0;
		 count  = 0;
	 }
	 
}