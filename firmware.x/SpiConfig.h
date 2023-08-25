#ifndef SPI_H
#define	SPI_H

#include <stdint.h>
void SpiConfig(void);

/*
//BiSS-iC-MCW Kommunikation
uint8_t StatusLesenBiss1(void);
uint8_t RegisterLesenBiss1(uint8_t adresse);
void DeveiceErrorLoeschenBiss1(void);
uint8_t StatusLesenBiss2(void);
uint8_t RegisterLesenBiss2(uint8_t adresse);
void DeveiceErrorLoeschenBiss2(void);
uint32_t PosBiss1Auslesen(void);
uint32_t PosBiss2Auslesen(void);

//SPI Kommandos zur BiSS-iC-MCW Kommunikation
#define ReadStatus      0x9C    //reads 8 Bits, parts from DeviceStatus und DeviceError Registers
#define ReadFrameData   0xA6    //reads INFO byte, CPW...SPW...FrameNumberCD
#define ReadRegister    0x8A
#define WriteRegister   0xD2

//BiSS-iC-MCW Register Adressen
#define ChipRevision    0x00
#define DeviceStatus    0x01
#define DeviceError     0x02
#define FrameInformation 0x03
#define FrameNumberCD_L  0x04
#define FrameNumberCD_H  0x05
#define FrameError      0x06
#define FrameCounter_L  0x07
#define FrameCounter_H  0x08

//Masken
#define M_Available  0x80
#define M_Detect     0x40
#define M_Wait       0x30
#define M_PLL        0x08
#define M_SPI        0x04
#define M_Frame      0x02
#define M_Reset      0x01

//Konstante
#define kBissTimeOut    50
#define kMaxFrameLength 20
*/

//extern uint8_t SerTX[];
extern uint8_t flParamKomplett;

#endif	/* SPI_H */

