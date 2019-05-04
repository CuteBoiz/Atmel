/*
 * KeyBoard.h
 *
 * Created: 04-May-19 9:12:07 AM
 *  Author: tanph
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CLK_DDR		DDRD
#define CLK_PORT	PORTD
#define CLK_PIN		2		//INT0

#define DATA_DDR	DDRD
#define DATA_PORT	PORTD
#define DATA_PIN	3

#define BUFF_SIZE	16

#define BACKSPAGE		0X80
#define UP				0x81
#define DOWN			0x82
#define LEFT			0x83
#define RIGHT			0x84
#define PAGEDOWN		0x85
#define PAGEUP			0x86
#define END				0x87
#define HOME			0x88
#define INSERT			0x89
#define DEL				0x8A
#define ESC				0x8B
#define CAPLOCK_ON		0x8C
#define CAPLOCK_OFF		0x8D
#define CTRL			0X8F
#define ALT				0X90




ISR(INT0_vect);

class KeyBoard{
public:
	KeyBoard(void);
	void begin();		//Registering a external interrupt + set Pin modes 
	bool available();   //Return true if there is a char to be read
	void reset();		//Send a reset command to KB and re-initialize all the control
	uint8_t read();		//Return the last char pressed. the buffer will be cleared when the char bhas been read
	uint8_t extraRead();
	void setLight(uint8_t data);

private:
	uint8_t oddParity(uint8_t data);
	uint8_t sendCommand(uint8_t data);
	
};

#endif /* KEYBOARD_H_ */