/*
 * Nokia5110.h
 *
 * Created: 26-May-19 12:17:39 PM
 *  Author: tanph
 */ 


#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#if defined(F_CPU)
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "Charset.h"

#define PCD8544_CMD		0
#define PCD8544_DATA	1

#define REVERSE		true
#define NON_REVERSE	false

#define DDR_PCD		DDRC
#define PORT_PCD	PORTC

class PCD8544{
public:
	PCD8544(int RST, int SCE, int DC, int SDIN, int SCLK);
	void reset();
	void clearSreen();
	void begin();
	void setCursor(uint8_t x, uint8_t y);
	void charDisplay(int isReverse, char data);
	void lineDisplay(int isReverse, char *data);
	
	void createMenu(char *name, char *Menu[], int length);
	void displayMenu();
	
	int getPointer();
	
	void increasePointer();
	void decreasePointer();
private:
	int RST_PIN, SCE_PIN, DC_PIN, SDIN_PIN, SCLK_PIN;
	
	
	int pointer;
	int current;
	int lenOfMenu;
	char *title;
	char *Menu[];
	
	void sendToPCD(int mode, uint8_t data);
	void sendCommand(uint8_t data);
	void sendData(uint8_t data); 
};



#endif /* NOKIA5110_H_ */