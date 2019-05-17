/*
 * Keyboard.cpp
 *
 * Created: 04-May-19 9:39:42 AM
 *  Author: tanph
 */ 

#include "KeyBoard.h"

uint8_t			 currentBuffer;
volatile uint8_t charBuffer;
volatile uint8_t bufferPos;

volatile bool	kbExtend;
volatile bool	kbRelease;

volatile bool	kbShift;
volatile bool	kbCtrl;
volatile bool	kbAlt;
volatile bool	kbCapslock;
volatile bool	kbNumlock;

volatile bool		cmdInProgress;
volatile int		cmdCount;
uint8_t				cmdValue;
volatile uint8_t	cmdACKValue;
uint8_t				cmdParity;
volatile bool		cmdACKOk;

uint8_t KeyBoard::oddParity(uint8_t data){
	int i, count = 1;
	for(i = 0; i < 8; i++){
		if ((data >> i) & 0x01)
		count++;
	}
	return count & 0x01;
}

uint8_t KeyBoard::sendCommand(uint8_t data){
	cmdInProgress	= true;
	cmdCount		= 0;
	
	cmdValue		= data;
	cmdACKValue		= 1;
	cmdParity		= oddParity(data);
	
	//set DATA line  and CLK as OUTPUT to send data to keyboard
	DATA_DDR	|= (1<<DATA_PIN);
	CLK_DDR		|= (1<<CLK_PIN);
	
	DATA_PORT	|= (1<<DATA_PIN); //IDLE always 1
	CLK_PORT	&= ~(1<<CLK_PIN); //Clear INT0 to LOW to generate a Interrupt request
	
	//Jump to ISR(INT0_vect) to send data
	
	_delay_ms(50);
	
	DATA_PORT	&= ~(1<<DATA_PIN);
	
	CLK_PORT	|= (1<<CLK_PIN);
	CLK_DDR		&= ~(1<<CLK_PIN);
	
	while (cmdACKValue != 0);
	
	cmdInProgress = false;
}
KeyBoard::KeyBoard(){
	
}
void KeyBoard::begin(){
	
	currentBuffer	= 0x00;
	charBuffer		= 0x00;
	bufferPos		= 0x00;
	
	kbShift			= false;
	kbCtrl			= false;
	kbAlt			= false;
	
	kbCapslock		= false;
	kbNumlock		= false;
	
	kbExtend		= false;
	kbRelease		= false;
	
	cmdInProgress	= false;
	cmdCount		= 0;
	cmdValue		= 0x00;
	cmdACKValue		= 1;
	
	CLK_DDR		&= ~(1<<CLK_PIN);	//set CLK as INPUT
	CLK_PORT	|= (1<<CLK_PIN);	//pull-up
	
	DATA_DDR	&= ~(1<<DATA_PIN);	//set DATA as INPUT
	DATA_PORT	|= (1<<DATA_PIN);	//pull-up
	
	MCUCR |= (1<<ISC01);
	GICR	|= (1<<INT0);
}

void KeyBoard::setLight(uint8_t data){
	cmdACKOk = false;
	sendCommand(0xED);
	while(!cmdACKOk);
	sendCommand(data);
}
bool KeyBoard::available(){
	return (charBuffer != 0);
}

uint8_t KeyBoard::read(){
	uint8_t result;
	result = charBuffer;
	switch(result){
		case 0x1C: result = 'a'; break;
		case 0x32: result = 'b'; break;
		case 0x21: result = 'c'; break;
		case 0x23: result = 'd'; break;
		case 0x24: result = 'e'; break;
		case 0x2B: result = 'f'; break;
		case 0x34: result = 'g'; break;
		case 0x33: result = 'h'; break;
		case 0x43: result = 'i'; break;
		case 0x3B: result = 'j'; break;
		case 0x42: result = 'k'; break;
		case 0x4B: result = 'l'; break;
		case 0x3A: result = 'm'; break;
		case 0x31: result = 'n'; break;
		case 0x44: result = 'o'; break;
		case 0x4D: result = 'p'; break;
		case 0x15: result = 'q'; break;
		case 0x2D: result = 'r'; break;
		case 0x1B: result = 's'; break;
		case 0x2C: result = 't'; break;
		case 0x3C: result = 'u'; break;
		case 0x2A: result = 'v'; break;
		case 0x1D: result = 'w'; break;
		case 0x22: result = 'x'; break;
		case 0x35: result = 'y'; break;
		case 0x1A: result = 'z'; break;
		
		case 0x45: result = kbShift ? ')' : '0'; break;
		case 0x16: result = kbShift ? '!' : '1'; break;
		case 0x1E: result = kbShift ? '@' : '2'; break;
		case 0x26: result = kbShift ? '#' : '3'; break;
		case 0x25: result = kbShift ? '$' : '4'; break;
		case 0x2E: result = kbShift ? '%' : '5'; break;
		case 0x36: result = kbShift ? '^' : '6'; break;
		case 0x3D: result = kbShift ? '&' : '7'; break;
		case 0x3E: result = kbShift ? '*' : '8'; break;
		case 0x46: result = kbShift ? '(' : '9'; break;
		
		case 0x0E: result = kbShift ? '~' : '`'; break;
		case 0x4E: result = kbShift ? '_' : '-'; break;
		case 0x55: result = kbShift ? '+' : '='; break;
		case 0x5D: result = kbShift ? '|' : '\\'; break;
		case 0x54: result = kbShift ? '{' : '['; break;
		case 0x5B: result = kbShift ? '}' : ']'; break;
		case 0x4C: result = kbShift ? ':' : ';'; break;
		case 0x52: result = kbShift ? '\"' : '\''; break;
		case 0x41: result = kbShift ? '<' : ','; break;
		case 0x49: result = kbShift ? '>' : '.'; break;
		case 0x4A: result = kbShift ? '?' : '/'; break;
		
		case 0x0D: result = '\t';		break;
		case 0x5A: result = '\n';		break;
		case 0x29: result = ' ';		break;
		
		case 0x66: result = BACKSPAGE;	break;
		case 0x69: result = kbExtend ? END : '1';		break;
		case 0x6B: result = kbExtend ? LEFT : '4';		break;
		case 0x6C: result = kbExtend ? HOME : '7';		break;
		case 0x70: result = kbExtend ? INSERT : '0';		break;
		case 0x71: result = kbExtend ? DEL : '';		break;
		case 0x72: result = kbExtend ? DOWN;		break;
		case 0x74: result = kbExtend ? RIGHT;		break;
		case 0x75: result = kbExtend ? UP;			break;
		case 0x76: result = kbExtend ? ESC;		break;
		case 0x7A: result = kbExtend ? PAGEDOWN;	break;
		case 0x7D: result = kbExtend ? PAGEUP;		break;
		
		case 0x58:
			if (kbCapslock){
				setLight(0x04);
			}
			else{
				setLight(0x00);
			}
			break;
		
		default:
			_delay_ms(500);
	}
	
	if (((result>= 'a') && (result <= 'z')) && ((kbShift && !kbCapslock) || (!kbShift && kbCapslock))){
		result = result + ('A' - 'a');
	}
	charBuffer = 0x00;
	return result;
}


ISR(INT0_vect){
	//Send data
	if (cmdInProgress){
		cmdCount++;
		switch(cmdCount){
			case 1:{ //Start bit
				DATA_PORT &= ~(1<<DATA_PIN);
				break;
			}
			
			case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:{ //Data bits
				if (cmdValue & 0x01){
					DATA_PORT |= (1<<DATA_PIN);
				}
				else{
					DATA_PORT &= ~(1<<DATA_PIN);
				}
				cmdValue = cmdValue >> 1;
				break;
			}
			
			case 10: {
				if (cmdParity){
					DATA_PORT |= (1<<DATA_PIN);
				}
				else{
					DATA_PORT &= ~(1<<DATA_PIN);
				}
				break;
			}
			
			case 11:{ //Stop bit
				DATA_PORT |= (1<<DATA_PIN);
				DATA_DDR  &= ~(1<<DATA_PIN); //Ready to receive data from KB
				break;
			}
			
			case 12:{ //ACK from KB
				if (!(PIND & (1<<DATA_PIN))){
					cmdACKValue = 0;
				}
				else{
					cmdACKValue = 1;
				}
				cmdInProgress = false;
			}
		}
		return ;
	}
	//Get Data
	int value;
	if (!(PIND & (1<<DATA_PIN))){
		value = 0;
	}
	else{
		value = 1;
	}
	
	//shift data from LSB -> MSB
	if (bufferPos > 0 && bufferPos < 11){
		currentBuffer |= (value << (bufferPos - 1));
	}
	bufferPos++;
	
	if (bufferPos == 11){//After get all bits of buffer
		switch(currentBuffer){
			case 0xF0: { //Key release
				kbRelease	= true;
				kbExtend	= false;
				break;
			}
			
			case 0xFA: { //ACK
				cmdACKOk	= true;
				break;
			}
			
			case 0xE0: { //Extended
				kbExtend	= true;
				break;
			}
			
			case 0x12:
			case 0x59: {
				kbShift		= kbRelease ? false : true;
				kbRelease	= false;
				break;
			}
			
			case 0x11: {
				kbAlt		= kbRelease ? false : true;
				kbRelease	= false;
				break;
			}
			
			case 0x14: {
				kbCtrl		= kbRelease ? false : true;
				kbRelease	= false;
				break;
			}
			
			case 0x58: {
				if (kbRelease){
					kbRelease	= false;
				}
				else{
					kbCapslock = !(kbCapslock);
					charBuffer	= currentBuffer;
				}
			}
			
			default: { //A Normal key pressed
				if (kbRelease){
					kbRelease	= false;
				}
				else{
					charBuffer	= currentBuffer;
				}
			}
		}
		currentBuffer	= 0;
		bufferPos		= 0;
	}
}