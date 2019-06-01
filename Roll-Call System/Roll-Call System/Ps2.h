/*
 * PS2.h
 *
 * Created: 30-May-19 9:02:19 PM
 *  Author: tanph
 */ 


#ifndef PS2_H_
#define PS2_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DDR_KB		DDRD
#define PORT_KB		PORTD
#define CLK_PIN		2
#define DATA_PIN	3

ISR(INT0_vect);

class PS2{
public:
	PS2();
	void begin();
	bool available();
	uint8_t getChar();
private:
	void sendCommand(uint8_t data);
	uint8_t oddParity(uint8_t data);
	void setLight();
	uint8_t Lights;//bit 0 -> Scroll; bit 1 -> Num; bit2 -> Cap
};


#define KB_BACKSPACE	0x80

#define KB_UP			0x81
#define KB_DOWN			0x82
#define KB_LEFT			0x83
#define KB_RIGHT		0x84

#define KB_INSERT		0x83
#define KB_DELETE		0x84
#define KB_HOME			0x85
#define KB_END			0x86
#define KB_PG_UP		0x87
#define KB_PG_DN		0x88

#define KB_ESC			0x89


#endif /* PS2_H_ */