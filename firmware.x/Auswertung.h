#ifndef AUSWERTUNG_H
#define AUSWERTUNG_H

//#include <xc.h> 
#include "main.h"
#include <stdint.h>
#include "Frequenz.h"
#include "Uart.h"

//enum { _RelaisOff, _LowXms, _WaitForHigh, _High200ms, _WaitForLow, _WaitForRelOn, _WaitFor1Hz, _WaitForLimit1,  _WaitForRelOff, _WaitMinRelOff, _WaitForRelOnWindow, _WaitForRelOn05Hz};
   enum { _Off = 0, _LowXms, _WaitForHigh, _High200ms, _WaitForLow, _WaitForQuittMS, _WaitForLim0_0, _WaitForLimit1,  _WaitForOff, _WaitMinOff, _WaitForOnWindow, _WaitForOnLim0};
//state		0		1			2			3           4			    5				6               7				8			9			10					11

//###test### #define _QuittMS    ((TX.bf.Quitt == 1)&&(RX.bf.Quitt == 1))
#define _QuittMS    TX.bf.Quitt
#define _MinAbschaltzeit 50 //50 = 0,5s
#define _WartenQuitt    100 //100 = 1s
#define kTimeLow         10 //100ms Lowpegel
#define kTimeHigh        10 //100ms Highpegel bei manueller Quittierung  

void QuittierFunktion(uint8_t dz_mode);
//###test###void getQuittierError(void);
void QuittReset(void);
void Auswertung(void);
void DrehzahlAuswertung(uint8_t dz_mode);
void EmkVerzoegerung(uint8_t emk_mode);

extern uint8_t flParamKomplett;
extern uint8_t CanTX[8]; 
extern str_ipk RX, TX;
extern uint8_t cnt1msRoutinen;
extern ParamStruct PS;
extern uint8_t stmQuitt;
extern uint8_t flQuitt;
extern uint16_t cntQuitt;
extern uint16_t cntQuittErr;
extern uint8_t flQuittErr;
extern uint16_t cntZeitLimit;
extern uint16_t IstFrqX10;
extern uint8_t Muting;

#endif	/* AUSWERTUNG_H */

