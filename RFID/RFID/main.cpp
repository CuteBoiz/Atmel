/*
 * RFID.cpp
 *
 * Created: 20-May-19 11:06:18 PM
 * Author : tanph
 */ 

#include "MFRC522.h"

void USART_Init(){
	UBRRL = 51;
	UCSRC |= (1<<URSEL) | (3<<UCSZ0);
	UCSRB |= (1<<TXEN);
}
void SPI_Init(){
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	DDRB |= (1<<MOSI_PIN) | (1<<SS_PIN) | (1<<SCK_PIN);
	PORTB |= (1<<SS_PIN);
}

MFRC522 RFID;

int main(void)
{
	SPI_Init();
	USART_Init();
    RFID.begin();
	DDRC |= (1<<5);
	
	uint8_t data[MAX_LEN];
	int status ;
    while (1) 
    {
		PORTC |= (1<<5);
		USART_Transmit('P');
		_delay_ms(500);
		RFID.WUPA_TAG(data);
		if (status == MI_OK){
			for (int i = 0; i < 2; i++){
				USART_Transmit(data[i]);
			}
			PORTC &= ~(1<<5);
			_delay_ms(2000);
			RFID.HALT_TAG();
		}
		
    }
}

