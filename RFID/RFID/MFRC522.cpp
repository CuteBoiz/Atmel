/*
 * MFRC522.cpp
 *
 * Created: 21-May-19 4:52:02 PM
 *  Author: tanph
 */ 
#include "MFRC522.h"

uint8_t SPI_Transceive(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void USART_Transmit(char data){
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
	_delay_ms(5);
}

void USART_SendString(char *data){
	register int i = 0;
	while(data[i] != 0x00){
		USART_Transmit(data[i]);
		i++;
	}
}

MFRC522::MFRC522(){
	DDR_MFRC	|= (1<<SS_PIN) | (1<<RST_PIN);
	PORT_MFRC	|= (1<<SS_PIN) | (1<<RST_PIN);
}

uint8_t MFRC522::readFromRegister(uint8_t regAddr){
	PORT_MFRC &= ~(1<<SS_PIN);
	SPI_Transceive(((regAddr << 1) & 0xF7) | 0x80);
	register uint8_t data = SPI_Transceive(0x00);
	PORT_MFRC |= (1<<SS_PIN);
	return data;
}

void MFRC522::writeToRegister(uint8_t regAddr, uint8_t val){
	PORT_MFRC &= ~(1<<SS_PIN);
	SPI_Transceive((regAddr << 1) & 0x77);
	SPI_Transceive(val);
	PORT_MFRC |= (1<<SS_PIN);
}

void MFRC522::setRegBit(uint8_t regAddr, uint8_t bitLocation){
	register uint8_t current = readFromRegister(regAddr);
	writeToRegister(regAddr, (current |= (1<<bitLocation)));
}

void MFRC522::clearRegBit(uint8_t regAddr, uint8_t bitLocation){
	register uint8_t current = readFromRegister(regAddr);
	writeToRegister(regAddr, (current &= ~(1<<bitLocation)));
}

void MFRC522::begin(){
	PORT_MFRC |= (1<<SS_PIN);
	writeToRegister(CommandReg, SOFT_RESET);
	writeToRegister(TModeReg, 0x8D);
	writeToRegister(TPrescalerReg, 0x3E);
	writeToRegister(TReloadRegL, 48);
	setRegBit(TxASKReg, 6);
	writeToRegister(ModeReg, 0x29);
	setRegBit(TxControlReg, 0);
	setRegBit(TxControlReg, 1);
}

int MFRC522::sendToTag(uint8_t *data, int dlen, uint8_t *recvData,int *rlen){
	//Initial
	writeToRegister(ComIEnReg, 0xF7);
	clearRegBit(ComIrqReg, 7);
	setRegBit(FIFOLevelReg, 7);
	writeToRegister(CommandReg, IDLE);
	
	//Write Data To FIFO Buffer
	register int i;
	register uint8_t n;
	for (i = 0; i < dlen; i++){
		writeToRegister(FIFODataReg, data[i]);
	}
	
	//Send Data
	writeToRegister(CommandReg, TRANSCEIVE);
	setRegBit(BitFramingReg, 7);
	
	//Wait Until Sent Complete
	i = 25;
	do{
		n = readFromRegister(ComIrqReg);
		i--;
		_delay_ms(5);
	} while ((i != 0) && !(n & 0x31));
	
	//Check Errors
	clearRegBit(BitFramingReg, 7);
	if (i == 0){
		return MI_ERR; // TIME_OUT
	}
	
	n = readFromRegister(ErrorReg);
	if (n & 0x1D){
		return MI_ERR; //ERROR
	}
	
	n = readFromRegister(ComIrqReg);
	if (n & 0x01){
		return MI_ERR; //No_Tag
	}
	
	//Get result's length
	register int lastBits;
	n = readFromRegister(FIFOLevelReg);
	lastBits = readFromRegister(ControlReg)  & 0x07;
	if (lastBits){
		*rlen = (n-1) * 8 + lastBits;	
	}
	else{
		*rlen = n * 8;
	}
	
	if (n > MAX_LEN){
		n = MAX_LEN;
	}
	if (n == 0){
		n = 1;
	}
	
	for (i = 0; i < n; i++){
		recvData[i] = readFromRegister(FIFODataReg);
	}
	
	writeToRegister(CommandReg, IDLE);
	return MI_OK;
}

int MFRC522::calculateCRC(uint8_t *data, int dlen, uint8_t *result){
	clearRegBit(DivIrqReg, 2);
	setRegBit(FIFOLevelReg, 7);
	writeToRegister(CommandReg, IDLE);
	
	register int i;
	register uint8_t n;
	for (int i = 0; i < dlen; i++){
		writeToRegister(FIFODataReg, data[i]);
	}
	
	writeToRegister(CommandReg, CALC_CRC);
	
	i = 25;
	do 
	{
		n = readFromRegister(DivIrqReg);
		i--;
		_delay_ms(5);
	} while ((i != 0) && !(n & 0x04));
	
	if (i == 0){
		return MI_ERR;
	}
	
	result[0] = readFromRegister(CRCResultRegL);
	result[1] = readFromRegister(CRCResultRegM);
	return MI_OK;	
}

int MFRC522::WUPA_TAG(uint8_t *ATQA){
	register uint8_t buffer[1];
	register int rlen, status;
	buffer[0] = MF_WUPA;
	writeToRegister(BitFramingReg, 0x07);
	
	status = this->sendToTag(buffer, 1, ATQA, &rlen);
	
	if (!status){
		return MI_ERR;
	}
	if (rlen != 16){
		USART_SendString("DO DAI SAI!");
	}
	
	writeToRegister(BitFramingReg, 0x00);
	return MI_OK;
}

void MFRC522::HALT_TAG(){
	register uint8_t buffer[4];
	register int rlen;
	
	buffer[0] = MF_HALT;
	buffer[1] = 0x00;
	
	this->calculateCRC(buffer, 2, &buffer[2]);
	sendToTag(buffer, 4, buffer, &rlen);
}