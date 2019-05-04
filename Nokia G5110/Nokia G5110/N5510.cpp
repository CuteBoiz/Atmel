#include "N5110.h"
void Nokia5110::send(int type, uint8_t data){
	LCD_PORT &= ~(1 << SCE);
	
	if (type)
		LCD_PORT |= (1 << DC);
	else
		LCD_PORT &= ~(1 << DC );
	
	for (int i = 0; i < 8; i++){
		if ((data >> (7 - i)) & 0x01)
			LCD_PORT |= (1 << SDIN);
		else
			LCD_PORT &= ~(1 << SDIN);
			
		LCD_PORT |= (1 << SCLK);
		LCD_PORT &= ~(1 << SCLK);
	}
	
	LCD_PORT |= (1 << SCE);
}

void Nokia5110::sendCommand(uint8_t cmd){
	send(PCD8544_cmd, cmd);
}
void Nokia5110::sendData(uint8_t data){
	send(PCD8544_data, data);
}

void Nokia5110::setCursor(uint8_t x, uint8_t y){
	this->cursor_x = x;
	this->cursor_y = y;
	sendCommand(0x20);
	sendCommand(0x80 | this->cursor_x);
	sendCommand(0x40 | this->cursor_y);
}
Nokia5110::Nokia5110(){
	this->cursor_x = 0;
	this->cursor_y = 0;
	this->inverse_output = false;
	LCD_DDR |= (1 << RST) | (1 << SCE) | (1 << DC) | (1 << SDIN) | (1 << SCLK);
	LCD_PORT |= (1 << RST) | (1 << SCE);
}

void Nokia5110::reset(){
	LCD_PORT |= (1 << RST) | (1 << SCE);
	
	LCD_PORT &= ~(1 << RST);
	LCD_PORT |= (1 << RST);
}

void Nokia5110::clearSreen(){
	setCursor(0, 0);
	for (int i = 0; i < 504; i++)
		sendData(0x00);
}

void Nokia5110::begin(){
	this->reset();
	
	this->sendCommand(0x21); // H = 1
	this->sendCommand(0x80 | 0x5); //VOP[6:0] = (VLCD - 3,06) / 0.06 (Chinh do tuong phan)
	this->sendCommand(0x04); // TC0			
	this->sendCommand(0x14); // Bias 1:40 - 1:34
	
	this->sendCommand(0x20); //H = 0
	this->sendCommand(0x80);
	this->sendCommand(0x40);
	this->sendCommand(0x0C); //Normal Mode
	
	this->clearSreen();
}

void Nokia5110::fillUp(char *data){
	register int i = 0;
	int length = 0;
	while (data[i]){
		length++;
		i++;
	}
	for (i = 0; i < length; i++){
		this->sendChar(data[i]);
	}
	for (i = 0; i < (14-length)*6; i++){
		this->sendData(0x00);
	}
}
void Nokia5110::sendChar(char data)
{
	for(int i = 0; i < 5; i++)
	{
		this->sendData(CHARSET[int(data) - 0x20][i]);
	}
	this->sendData(0x00);
}

	
