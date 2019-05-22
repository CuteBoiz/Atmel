/*
 * MFRC522.h
 *
 * Created: 20-May-19 11:12:14 PM
 *  Author: tanph
 */ 


#ifndef MFRC522_H_
#define MFRC522_H_
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define MAX_LEN 16
//Pins
#define PORT_MFRC	PORTB
#define DDR_MFRC	DDRB
#define SS_PIN		2
#define RST_PIN		1
#define SCK_PIN		5
#define MOSI_PIN	3
#define MISO_PIN	4

//Status
#define MI_OK		1
#define MI_ERR		0

//MIFARE Commands
#define MF_REQA		0x26
#define MF_WUPA		0x52
#define MF_ANTICOLL	0x93
#define MF_SELECT	0x93
#define MF_HALT		0x50

//MFRC Commands
#define IDLE		0x00
#define MEM			0x01
#define GENID		0x02
#define CALC_CRC	0x03
#define TRANSMIT	0x04
#define RECEIVE		0x07
#define TRANSCEIVE	0x0C
#define AUTHENT		0x0E
#define SOFT_RESET	0x0F

//MFRC Registers
//Page 0: Command and status
#define CommandReg		0x01
#define ComIEnReg		0x02
#define DivIEnReg		0x03
#define ComIrqReg		0x04
#define DivIrqReg		0x05
#define ErrorReg		0x06
#define	Status1Reg		0x07
#define Status2Reg		0x08
#define FIFODataReg		0x09
#define FIFOLevelReg	0x0A
#define WaterLevelReg	0x0B
#define ControlReg		0x0C
#define BitFramingReg	0x0D
#define CollReg			0x0F

//Page 1:Command
#define ModeReg			0x11
#define TxModeReg		0x12
#define RxModeReg		0x13
#define TxControlReg	0x14
#define TxASKReg		0x15
#define TxSelReg		0x16
#define RxSelReg		0x17
#define RxThresholdReg	0x18
#define DemodReg		0x19
#define MfTxReg			0x1C
#define MfRxReg			0x1D
#define SerialSpeedReg	0x1F

//Page 2: Configuration
#define CRCResultRegM	0x21
#define CRCResultRegL	0x22
#define ModWidthReg		0x24
#define RFCfgReg		0x26
#define GsNReg			0x27
#define CWGsPReg		0x28
#define ModGsPReg		0x29
#define	TModeReg		0x2A
#define TPrescalerReg	0x2B
#define TReloadRegH		0x2C
#define TReloadRegL		0x2D
#define TCounterValRegH	0x2E
#define TCounterValRegL	0x2F

uint8_t SPI_Transceive(uint8_t data);
void USART_Transmit(char data);
void USART_SendString(char *data);

class MFRC522{
public:
	MFRC522();
	void	begin();
	int		WUPA_TAG(uint8_t *ATQA);
	void	ANTICOLL_TAG();
	void	HALT_TAG();
private:
	uint8_t	readFromRegister(uint8_t regAddr);
	void	writeToRegister(uint8_t regAddr, uint8_t val);
	void	setRegBit(uint8_t regAddr, uint8_t bitLocation);
	void	clearRegBit(uint8_t regAddr, uint8_t bitLocation);
	int		sendToTag(uint8_t *data, int dlen, uint8_t *recvData,int *rlen);
	int		calculateCRC(uint8_t *data, int dlen , uint8_t *result);
};

#endif /* MFRC522_H_ */