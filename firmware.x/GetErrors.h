#ifndef GETERRORS_H
#define GETERRORS_H

//#include <xc.h>  
#include <stdint.h>
#include "Uart.h"
//#include "Transistoren.h"

#define kFrqToleranz    18  //18 Schritte auf der Tabelle sind 25%
#define mitKritischeErr   1
#define ohneKritischeErr  0

void GetErrors(void);
void getQuittierError(void);
void ResetErrors(uint8_t KritischeErr); 
void getPositionErr(void);
void getFrequenzErr(void);
void getInUngleichErr(void);
void getEmkErr(void);
void getCanTimeOutErr(void);
void FW_Vergleich(void);
void IpkFrameVergleich(void);
uint8_t getSoftErr(void);

extern uint8_t _flSkrErr1, _flSkrErr2;
extern uint8_t _flSkrQSErr1, _flSkrQSErr2;
extern uint8_t SkrErrGrundst;
extern uint8_t cntAusw1, cntAusw2;
extern uint8_t flParamKomplett;
extern uint8_t Muting;
extern volatile uint8_t Param[];
extern uint8_t CanRX[], CanTX[];
extern uint32_t PositionNeu1;
extern uint8_t fl100ms, cnt10msRoutinen;
extern uint8_t stmLatchDZ1, flLatchTX, flLatchRX;
extern str_ipk LatchRX, LatchTX;
extern uint16_t Latch_IstDzIX_1;
extern uint16_t Latch_SollDzIX_1; 
extern uint32_t Latch_Position1;
extern uint8_t Latch_Betriebsarten1;
extern uint8_t Latch_DzFlags1;
extern uint8_t Latch_flPosUW1;
extern uint8_t Latch_flEinkanalErr1;
extern uint8_t Latch_flFrqIxErr1;
extern uint8_t LatchCH2ErrA, LatchCH2ErrB;
extern uint32_t LR_PosAlt1;
extern uint8_t flPositionsUW1;
extern uint8_t cntCanTimeOut;
extern uint8_t flSerTOErr, cntSerTOErr, flErr3V3Test, cntErr3V3Test;
extern uint8_t cntErr1V65Test, flErr1V65Test, cntErr12VTest, flErr12VTest;
extern uint8_t cntPortTestErr, flPortsErr, flDrahtBruchErr;
extern uint8_t cnt1500Hz, fl1500Hz;
extern uint8_t cntErrRichtungsWechsel, flErrRichtungsWechsel;
extern uint8_t cntErrGleichePhase, flErrGleichePhase;
extern uint8_t cntAdcFrqErr, flAdcFrqErr;
extern uint8_t cntDiffErr, flDiffErr;
extern uint8_t cntDrBrSpLowErr, cntDrBrSpHighErr, flDrBrSpannung;
extern uint8_t cntErrTriggerADC, flErrTriggerADC;

#endif	/* GETERRORS_H */

