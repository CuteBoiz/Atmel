/*
 * Nokia5110.cpp
 *
 * Created: 26-May-19 12:29:03 PM
 *  Author: tanph
 */ 

#include "Nokia5110.h"

PCD8544::PCD8544(int RST, int SCE, int DC, int SDIN, int SCLK){
	RST_PIN = RST;
	SCE_PIN = SCE;
	DC_PIN = DC;
	SDIN_PIN = SDIN;
	SCLK_PIN = SCLK;
	DDR_PCD |= (1<<RST_PIN) | (1<<SCE_PIN) | (1<<DC_PIN) | (1<<SDIN_PIN) | (1<<SCLK_PIN);
	PORT_PCD |= (1<<SCE_PIN) | (1<<RST_PIN);
	PORT_PCD &= ~(1<<SCLK);
}

//Send data or command to PCD8544
void PCD8544::sendToPCD(int type, uint8_t data){
	PORT_PCD &= ~(1<<SCE_PIN);
	
	if (type == PCD8544_DATA){
		PORT_PCD |= (1<<DC_PIN);
	}
	else{
		PORT_PCD &= ~(1<<DC_PIN);
	}
	
	for (int i = 0; i < 8; i++){
		if ((data >> (7-i)) & 0x01){
			PORT_PCD |= (1<<SDIN_PIN);
		}
		else{
			PORT_PCD &= ~(1<<SDIN_PIN);
		}
		
		PORT_PCD |= (1<<SCLK_PIN);
		PORT_PCD &= ~(1<<SCLK_PIN);
	}
	PORT_PCD |= (1<<SCE_PIN);
	return ;
}

void PCD8544::sendCommand(uint8_t data){
	sendToPCD(PCD8544_CMD, data);
	return ;
}

void PCD8544::sendData(uint8_t data){
	sendToPCD(PCD8544_DATA, data);
	return ;
}



// 0 <= x < 84; 0 <= y < 6
void PCD8544::setCursor(uint8_t x, uint8_t y){
	this->sendCommand(0x20);
	this->sendCommand((0x80 | x));
	this->sendCommand((0x40 | y));	
}

void PCD8544::reset(){
	PORT_PCD |= (1<<RST_PIN) | (1<<SCE_PIN);
	PORT_PCD &= ~(1<<RST_PIN);
	_delay_ms(50);
	PORT_PCD |= (1<<RST_PIN);
}

void PCD8544::clearSreen(){
	this->setCursor(0x00, 0x00);
	for (int i = 0; i < 504; i++){
		this->sendData(0x00);
	}
}

//Initial for display in LCD
void PCD8544::begin(){
	this->reset();
	
	this->sendCommand(0x20 | 0x01); //H = 1
	this->sendCommand(0x01 | 0x03); //Bias 1:48
	this->sendCommand(0x80 | 0xC5); //Set contrast
	this->sendCommand(0x04 | 0x00); //Set TC0
	
	this->sendCommand(0x20 | 0x00); //H = 0
	this->sendCommand(0x08 | 0x04); //Normal Mode
	this->sendCommand(0x80 | 0x00); //x-address = 0
	this->sendCommand(0x40 | 0x00); //y-address = 0
	
	this->clearSreen();
}

//Display a single character
void PCD8544::charDisplay(int isReverse, char data){
	register uint8_t reverse;
	if (isReverse){
		reverse = 0xFF;
	}
	else{
		reverse = 0x00;
	}
	
	for (int i = 0; i < 5; i++){
		this->sendData((CHARSET[int(data) - 0x20][i]) ^ reverse);
	}
	this->sendData(reverse);
}

//Display a line of LCD
void PCD8544::lineDisplay(int isReverse, char *data){
	register uint8_t space;
	if (isReverse){
		space = 0xFF;
	}
	else{
		space = 0x00;
	}
	
	//Get string's length
	register int i = 0;
	register int length = 0;
	while (data[i]){
		length++;
		i++;
	}
	if (length > 14){
		length = 14;
	}
	for (i = 0; i < length; i++){
		this->charDisplay(isReverse, data[i]);
	}
	for (i = 0; i < (14 - length)*6 ; i++){
		this->sendData(space);
	}
}

void PCD8544::MenuInit(){
	this->pointer = 0;
	this->current = 0;
}

void PCD8544::displayMenu(char *title, char *Menu[], int length){
	this->lenOfMenu = length;
	this->setCursor(0, 0);
	this->lineDisplay(NON_REVERSE, title);
	
	register int i;
	if (lenOfMenu < 5){
		for (i = 0; i < lenOfMenu; i++){
			this->setCursor(0, i + 1);
			this->lineDisplay(NON_REVERSE, Menu[current + i]);
		}
		for(i = lenOfMenu; i < 5; i++){
			this->setCursor(0, i + 1);
			this->lineDisplay(NON_REVERSE, " ");
		}
	}
	else{
		for (i = 0; i < 5; i++){
			this->setCursor(0, i + 1);
			this->lineDisplay(NON_REVERSE, Menu[current + i]);
		}
	}
	this->setCursor(0, (pointer - current) + 1);
	lineDisplay(REVERSE, Menu[pointer]);
}

void PCD8544::increasePointer(){
	if ((pointer + 1) < lenOfMenu){
		pointer++;
		if (((lenOfMenu - current ) > 5) && ((pointer - current) == 5)){
			current++;
		}
	}
}

void PCD8544::decreasePointer(){
	if (pointer > 0){
		pointer--;
		if (((lenOfMenu - pointer) > 4) && (pointer < current)){
			current--;
		}
	}
}
