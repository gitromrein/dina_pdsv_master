#ifndef LEDANZEIGE_H
#define	LEDANZEIGE_H

#include "p33CK256MP505.h"
#include <stdint.h>
#include "main.h"
#include "p33CK256MP505.h"

#define LedGn		0xF0
#define LedRt		0xFF
#define LedOff		0x00
#define LedGn1Hz1zu1	0x30  /*1Hz blinken 1:1. LedGn0011*/
#define LedGn1HzKurzAn	0x10  /*1Hz kurz an. LedGn0001*/
#define LedGn1HzKurzAus	0xE0  /*1Hz kurz aus. LedGn1110*/
#define	LedGn2HzKurzAn	0x50  /*2Hz kurz an. LedGn0101*/
#define LedGn2HzKurzAus	0xA0  /*2Hz kurz aus. LedGn1010*/

void SteckplatzToLedBuffer(uint8_t);
void LedsSPI(void);
void LedsAnzeigen(void);

extern uint16_t cntLedAlternativ;
extern uint8_t flLedAlternativ;
extern uint8_t CanTX[];
//extern uint8_t SerTX[];
//extern uint8_t SerRX[];
//extern uint8 LedBufferAlt[20];
//extern Bitfeld Flags2;
extern Bitfeld FlagsError1;


extern volatile uint8_t Param[];
extern uint8_t flParamKomplett, cnt1msRoutinen, cnt100msRoutinen;
//#define _flLedAlternativ Flags2.bit0

#endif	/* LEDANZEIGE_H */

