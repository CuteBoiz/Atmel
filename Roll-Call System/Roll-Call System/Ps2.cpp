#include "PS2.h"

uint8_t				char_Current;
volatile uint8_t	char_Received;
volatile uint8_t	char_Count;

volatile bool		kb_Extend;
volatile bool		kb_Release;

volatile bool		kb_Shift;
volatile bool		kb_Capslock;
volatile bool		kb_Numlock;
volatile bool		kb_ScrollLock;

volatile bool		cmd_InProgress;
volatile int		cmd_Count;
uint8_t				cmd_Value;
volatile uint8_t	cmd_ACKValue;
uint8_t				cmd_Parity;
volatile bool		cmd_ACKOk;

PS2::PS2(){
	this->Lights = 0x00;
}
uint8_t PS2::oddParity(uint8_t data){
	int i, count = 1;
	for(i = 0; i < 8; i++){
		if ((data >> i) & 0x01)
		count++;
	}
	return count & 0x01;
}

void PS2::sendCommand(uint8_t data){
	cmd_InProgress	= true;
	cmd_Count		= 0;
	
	cmd_Value		= data;
	cmd_ACKValue		= 1;
	cmd_Parity		= oddParity(data);
	
	//set DATA line  and CLK as OUTPUT to send data to keyboard
	DDR_KB	|= (1<<DATA_PIN);
	DDR_KB		|= (1<<CLK_PIN);
	
	//Inhibit all communication
	PORT_KB	|= (1<<DATA_PIN);
	PORT_KB	&= ~(1<<CLK_PIN); 

	_delay_ms(50);
	
	PORT_KB	&= ~(1<<DATA_PIN);
	
	PORT_KB	|= (1<<CLK_PIN);
	DDR_KB		&= ~(1<<CLK_PIN);
	
	//An INT0 Interrupt will generate here
	
	while (cmd_ACKValue != 0);
	
	cmd_InProgress = false;
}

void PS2::begin(){
	
	char_Current	= 0x00;
	char_Received	= 0x00;
	char_Count		= 0x00;
	
	kb_Shift		= false;
	
	kb_Capslock		= false;
	kb_Numlock		= false;
	
	kb_Extend		= false;
	kb_Release		= false;
	
	cmd_InProgress	= false;
	cmd_Count		= 0;
	cmd_Value		= 0x00;
	cmd_ACKValue	= 1;
	
	DDR_KB		&= ~(1<<CLK_PIN);	
	
	DDR_KB	&= ~(1<<DATA_PIN);	
	PORT_KB	|= (1<<DATA_PIN);
	
	MCUCR |= (1<<ISC01);
	GICR	|= (1<<INT0);
}

void PS2::setLight(){
	cmd_ACKOk = false;
	sendCommand(0xED);
	while(!cmd_ACKOk);
	sendCommand(this->Lights);
}
bool PS2::available(){
	return (char_Received != 0);
}

uint8_t PS2::getChar(){
	uint8_t result;
	result = char_Received;
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
		
		case 0x45: result = kb_Shift ? ')' : '0'; break;
		case 0x16: result = kb_Shift ? '!' : '1'; break;
		case 0x1E: result = kb_Shift ? '@' : '2'; break;
		case 0x26: result = kb_Shift ? '#' : '3'; break;
		case 0x25: result = kb_Shift ? '$' : '4'; break;
		case 0x2E: result = kb_Shift ? '%' : '5'; break;
		case 0x36: result = kb_Shift ? '^' : '6'; break;
		case 0x3D: result = kb_Shift ? '&' : '7'; break;
		case 0x3E: result = kb_Shift ? '*' : '8'; break;
		case 0x46: result = kb_Shift ? '(' : '9'; break;
		
		case 0x0E: result = kb_Shift ? '~' : '`'; break;
		case 0x4E: result = kb_Shift ? '_' : '-'; break;
		case 0x55: result = kb_Shift ? '+' : '='; break;
		case 0x5D: result = kb_Shift ? '|' : '\\'; break;
		case 0x54: result = kb_Shift ? '{' : '['; break;
		case 0x5B: result = kb_Shift ? '}' : ']'; break;
		case 0x4C: result = kb_Shift ? ':' : ';'; break;
		case 0x52: result = kb_Shift ? '\"' : '\''; break;
		case 0x41: result = kb_Shift ? '<' : ','; break;
		case 0x49: result = kb_Shift ? '>' : '.'; break;
		case 0x4A: result = kb_Shift ? '?' : '/'; break;
		
		case 0x0D: result = '\t';		break;
		case 0x5A: result = '\n';		break;
		case 0x29: result = ' ';		break;
		case 0x66: result = KB_BACKSPACE; break;
		case 0x76: result = KB_ESC; break;
		
		case 0x58:{
			if (kb_Capslock){
				Lights |= (1<<2);
			}
			else{
				Lights &= ~(1<<2);
			}
			setLight();
			result = 0x00;
			break;
		}
		
		case 0x77:{
			if (kb_Numlock){
				Lights |= (1<<1);
			}
			else{
				Lights &= ~(1<<1);
			}
			setLight();
			result = 0x00;
			break;
		}
		
		case 0x7E:{
			if (kb_ScrollLock){
				Lights |= (1<<0);
			}
			else{
				Lights &= ~(1<<0);
			}
			setLight();
			result = 0x00;
			break;	
		}
		case 0x7C: result = '*'; break;
		case 0x7B: result = '-'; break;
		case 0x79: result = '+'; break;
		case 0x71:
			if(kb_Extend) result = KB_DELETE;
			else result = kb_Numlock ? '.' : 0x00;
			break;
		case 0x70:
			if(kb_Extend) result = KB_INSERT;
			else result = kb_Numlock ? '0' : 0x00;
			break;
		case 0x69:
			if(kb_Extend) result = KB_END;
			else result = kb_Numlock ? '1' : 0x00;
			break;
		case 0x72:
			if(kb_Extend) result = KB_DOWN;
			else result = kb_Numlock ? '2' : 0x00;
			break;
		case 0x7A:
			if(kb_Extend) result = KB_PG_DN;
			else result = kb_Numlock ? '3' : 0x00;
			break;
		case 0x6B:
			if(kb_Extend) result = KB_LEFT;
			else result = kb_Numlock ? '4' : 0x00;
			break;
		case 0x73:
			result = kb_Numlock ? '5' : 0x00;
			break;
		case 0x74:
			if(kb_Extend) result = KB_RIGHT;
			else result = kb_Numlock ? '6' : 0x00;
			break;
		case 0x6C:
			if(kb_Extend) result = KB_HOME;
			else result = kb_Numlock ? '7' : 0x00;
			break;
		case 0x75:
			if(kb_Extend) result = KB_UP;
			else result = kb_Numlock ? '8' : 0x00;
			break;
		case 0x7D:
			if(kb_Extend) result = KB_PG_UP;
			else result = kb_Numlock ? '9' : 0x00;
			break;
		
		
		default:{
			result = 0x00;
			_delay_ms(500);
		}
	}
	if ((result>= 'a') && (result <= 'z')){
		if((kb_Shift && !kb_Capslock) || (!kb_Shift && kb_Capslock)){
			result = result + ('A' - 'a');
		}
	}
	char_Received = 0x00;
	return result;
}


ISR(INT0_vect){
	//Send data
	if (cmd_InProgress){
		cmd_Count++;
		switch(cmd_Count){
			case 1:{ //Start bit
				PORT_KB &= ~(1<<DATA_PIN);
				break;
			}
			
			case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:{ //Data bits
				if (cmd_Value & 0x01){
					PORT_KB |= (1<<DATA_PIN);
				}
				else{
					PORT_KB &= ~(1<<DATA_PIN);
				}
				cmd_Value = cmd_Value >> 1;
				break;
			}
			
			case 10: {
				if (cmd_Parity){
					PORT_KB |= (1<<DATA_PIN);
				}
				else{
					PORT_KB &= ~(1<<DATA_PIN);
				}
				break;
			}
			
			case 11:{ //Stop bit
				PORT_KB |= (1<<DATA_PIN);
				DDR_KB  &= ~(1<<DATA_PIN); //Ready to receive data from KB
				break;
			}
			
			case 12:{ //ACK from KB
				if (!(PIND & (1<<DATA_PIN))){
					cmd_ACKValue = 0;
				}
				else{
					cmd_ACKValue = 1;
				}
				cmd_InProgress = false;
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
	
	if (char_Count > 0 && char_Count < 11){
		char_Current |= (value << (char_Count - 1));
	}
	char_Count++;
	
	if (char_Count == 11){
		switch(char_Current){
			case 0xF0: { 
				kb_Release = true;
				break;
			}
			
			case 0xFA: { 
				cmd_ACKOk = true;
				break;
			}
			
			case 0x12:
			case 0x59: {
				kb_Shift = kb_Release ? false : true;
				kb_Release	= false;
				break;
			}
			
			case 0xE0:{
				kb_Extend = !kb_Extend;
				kb_Release = false;
				break;
			}
			case 0x7E:{
				if (kb_Release){
					kb_Release	= false;
				}
				else{
					kb_ScrollLock = !(kb_ScrollLock);
					char_Received = char_Current;
				}
				break;
			}
			case 0x77: {
				if (kb_Release){
					kb_Release	= false;
				}
				else{
					kb_Numlock = !(kb_Numlock);
					char_Received = char_Current;
				}
				break;
			}
			case 0x58: {
				if (kb_Release){
					kb_Release	= false;
				}
				else{
					kb_Capslock = !(kb_Capslock);
					char_Received = char_Current;
				}
				break;
			}
			
			default: { 
				if (kb_Release){
					kb_Release = false;
				}
				else{
					char_Received = char_Current;
				}
			}
		}
		char_Current = 0;
		char_Count = 0;
	}
}