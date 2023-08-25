#ifndef         DIAGNOSE_H
#define	DIAGNOSE_H

#include <stdint.h>
#include <xc.h> // include processor files - each processor file is guarded.
#include "Uart.h"

void Diagnose(void);
void DiagnoseTest(void);
void LatchDZ(void);
//void DiagnoseTest(void);    //#######test########
//extern uint16_t ParamCRC16; //#######test########

#define kLatch1 1 //1 für Master
#define kLatch2 2 //2 für Slave

extern uint8_t Typenschild[];
extern str_ipk RX, TX, LatchRX, LatchTX;

#endif	/* XC_HEADER_TEMPLATE_H */




