/*
 * N5110.h
 *
 * Created: 22-Apr-19 11:14:17 PM
 *  Author: tanph
 */ 


#ifndef N5110_H_
#define N5110_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#define LCD_PORT	PORTB
#define LCD_DDR		DDRB

#define LCD_ON		true
#define LCD_OFF		false

#define	RST			1 //Reset(active low)
#define SCE			2 //Chip enable (active low)
#define DC			0 //Data (active high) - Command (active low)
#define SDIN		3 //Serial data
#define SCLK		5 //Serial clock

#define PCD8544_cmd		0
#define PCD8544_data	1

class Nokia5110{
public:
	Nokia5110(void);
	
	void send(int type, uint8_t data);
	void sendCommand(uint8_t cmd);
	void sendData(uint8_t data);
	
	void setCursor(uint8_t x, uint8_t y);
	
	void clear(void);
	void clearLine(void);
	
	void setPower(bool on);
	
	void reset();
	void begin(void);
	
private:
	uint8_t cursor_x;
	uint8_t cursor_y;
	
	bool inverse_output;

};

#endif /* N5110_H_ */