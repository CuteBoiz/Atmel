#include "N5110.h"
void Nokia5110::send(int type, uint8_t data){
	register int i;
	LCD_PORT &= ~(1 << SCE);
	
	if (type)
		LCD_PORT |= (1 << DC);
	else
		LCD_PORT &= ~(1 << DC );
	
	for (i = 0; i < 8; i++){
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

Nokia5110::Nokia5110(){
	this->cursor_x = 0;
	this->cursor_y = 0;
	this->inverse_output = false;
	LCD_DDR |= (1 << RST) | (1 << SCE) | (1 << DC) | (1 << SDIN) | (1 << SCLK);
	LCD_PORT |= (1 << RST) | (1 << SCE);
	for (int i = 0; i < 504; i++)
		this->screen[i] = 0x00;
}

void Nokia5110::reset(){
	LCD_PORT |= (1 << RST) | (1 << SCE);
	
	LCD_PORT &= ~(1 << RST);
	_delay_ms(100);
	LCD_PORT |= (1 << RST);
}

void Nokia5110::begin(){
	this->reset();
	
	LCD_PORT &= ~(1 << SCE);
	
	this->sendCommand(0x21); // H = 1
	this->sendCommand(0x04); // TC0
	this->sendCommand(0x14); // Bias 1:40 - 1:34
	this->sendCommand(0x80 | 0x21); //VOP[6:0] = (VLCD - 3,06) / 0.06 (Chinh do tuong phan)
	
	this->sendCommand(0x20); //H = 0
	this->sendCommand(0x0C); //Normal Mode
}
