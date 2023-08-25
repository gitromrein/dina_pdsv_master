#ifndef     FREQUENZVERGLEICH_H
#define     FREQUENZVERGLEICH_H

//#include <xc.h> 
#include <stdint.h>
#include "Frequenz.h"

void FrequenzVergleich(void);
void keinFrqVergl(void);
void DzFrqVergl(void);
void FensterFrqVergl(void);
void BereichFrqVergl(void);

//extern uint8_t SerTX[];
//extern uint8_t SerRX[];
//extern str_ipk RX, TX;
extern uint8_t CanTX[];
extern uint8_t CanRX[];
extern uint8_t NeuerMesswert1, NeuerMesswert2;//###test###
extern uint8_t fl10ms, fl100ms;
//extern uint16_t cntTimeOut1, cntTimeOut2;
extern volatile uint8_t Param[];
//extern uint8_t ix1DNCO, ix2DNCO;
extern volatile uint8_t DNCO[];
extern uint8_t cnt1msRoutinen;

extern uint16_t cntTimeOut0;           //für Limit0
extern uint16_t cntTimeOut1;			//für Limit1
extern uint16_t cntTimeOut2;			//für Limit2
extern uint16_t cntTimeOut3;			//für Limit3
extern uint8_t cntTimeOut250ms; 
extern uint8_t cntTimeOut1S;
extern uint16_t cntTimeOut3S;
extern uint16_t cntTimeOutMessung;		//für max. Periodendauer, unterer Grenzwert der Frequenz (0,3Hz).
extern uint16_t latchTimeOut1;         //Latchfunktion: Bei Timeout1 wird aus diesem Wert die Periode zurückgerechnet
extern uint8_t cntAbschaltverz1, cntAbschaltverz2;
extern uint8_t cnt1500Hz;	
extern uint8_t flValidCapture;
extern uint8_t NeueMessung;
extern uint8_t BereichIstFrqAlt, BereichIstFrq;
extern uint8_t stmSTHyst;	//kleinste Hysterese
extern uint8_t StatusSTHyst;			//Status der Hystereseansteuerung
extern uint8_t Betriebsarten, flPositionsUW1; 
extern uint8_t StatusLimit0, StatusLimit1, StatusLimit2, StatusLimit3;
extern uint16_t FrqLimit0, FrqLimit1, FrqLimit2, FrqLimit3; 
extern uint16_t FrqLimHyst0, FrqLimHyst1, FrqLimHyst2, FrqLimHyst3; 
extern uint16_t IstFrqX10;
extern uint8_t Validierung;
extern ParamStruct PS;
extern uint8_t stmQuitt;
extern uint8_t flQuitt;
extern uint16_t cntQuitt;

#endif	/* FREQUENZVERGLEICH_H */

