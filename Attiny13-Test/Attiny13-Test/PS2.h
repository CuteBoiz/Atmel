/*
 * PS2.h
 *
 * Created: 07-May-19 6:51:01 PM
 *  Author: tanph
 */ 


#ifndef PS2_H_
#define PS2_H_

#define F_CPU 12000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DDR_DATA	DDRB
#define PORT_DATA	PORTB
#define PIN_DATA	PINB
#define DATA_PIN	0

#define DDR_CLK		DDRB
#define PORT_CLK	PORTB
#define PIN_CLK		PINB
#define DATA_CLK	1


class PS2{
public:
	PS2();
	bool available();
	void sendCommand();
	//void 	
};



#endif /* PS2_H_ */