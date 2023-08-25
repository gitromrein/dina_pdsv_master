#ifndef SKR_ZWHD_H
#define	SKR_ZWHD_H

#include <stdint.h>
#include "Uart.h"
#include "Frequenz.h"

#define _konstDelSkrAusw  20		// entspricht 200ms Delay SKR Auswertung
#define _konstQdelayWert  50
#define	_konstTOSkr	      50
#define konstQuerschluss  31		//31 entspricht 3,1Sek

#define _flQSErkenn1    (PS.Par.SkrQSErk & 0x01)
#define _flQSErkenn2    (PS.Par.SkrQSErk & 0x02) 
#define _flQSErkenn3    (PS.Par.SkrQSErk & 0x04) 
#define _flQSErkenn4    (PS.Par.SkrQSErk & 0x08)

#define _flAutoMan1     (PS.Par.SkrAutoMan & 0x01)
#define _flAutoMan2     (PS.Par.SkrAutoMan & 0x02)
#define _flAutoMan3     (PS.Par.SkrAutoMan & 0x04)
#define _flAutoMan4     (PS.Par.SkrAutoMan & 0x08)

#define _flQDelay1      (PS.Par.SkrQDelay & 0x01)
#define _flQDelay2      (PS.Par.SkrQDelay & 0x02)
#define _flQDelay3      (PS.Par.SkrQDelay & 0x04)
#define _flQDelay4      (PS.Par.SkrQDelay & 0x08)

#define _flPowerOnGrSt1 (PS.Par.SkrPowerOn & 0x01)
#define _flPowerOnGrSt2 (PS.Par.SkrPowerOn & 0x02)
#define _flPowerOnGrSt3 (PS.Par.SkrPowerOn & 0x04)
#define _flPowerOnGrSt4 (PS.Par.SkrPowerOn & 0x08)

#define _flQPegel1      (PS.Par.SkrPegel & 0x01)
#define _flQPegel2      (PS.Par.SkrPegel & 0x02)
#define _flQPegel3      (PS.Par.SkrPegel & 0x04)
#define _flQPegel4      (PS.Par.SkrPegel & 0x08)

#define _flSkrAktiv1    (PS.Par.SkrAktiv & 0x01)
#define _flSkrAktiv2    (PS.Par.SkrAktiv & 0x02)
#define _flZweihandAktiv (PS.Par.SkrAktiv & 0x04)
//#define frei          (PS.Par.SkrAktiv & 0x08)

/*
//In writeParamStruct() werden die Parameter übernommen:
PS.Par.SkrQSErk = Param[2];
PS.Par.SkrAutoMan = Param[3];
PS.Par.SkrQDelay = Param[4];
PS.Par.SkrAntival = Param[5];
PS.Par.SkrPowerOn = Param[6];
PS.Par.SkrPegel = Param[7];
PS.Par.SkrAktiv = Param[8];
*/

void SkrAuswerten(void);
void QSchlErkSKR(uint8_t Eingaenge);
void ZwHdAuswerten(void);

extern uint8_t CanRX[8], CanTX[8], flParamKomplett, triggerCanTX0; //SerTX[], SerRX[],
extern volatile uint8_t Param[64];
extern uint8_t fl100ms, cnt10msRoutinen;
extern str_ipk RX, TX;
extern ParamStruct PS;


#endif	/* SKR_ZWHD_H */

