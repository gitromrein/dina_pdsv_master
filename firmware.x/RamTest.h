#ifndef RAMTEST_H
#define	RAMTEST_H

#include <stdint.h>
#include "Uart.h"

void StartUpRamTest(void);
void RamTest(void);
void ErrorLoop(uint8_t Ursache);

#define kBeginRamTest	0x1160
#define kEndRamTest     0x1800

extern uint8_t cnt10msRoutinen;
extern str_ipk TX;

#endif	/* RAMTEST_H_H */

