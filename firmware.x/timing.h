#ifndef TIMING_H
#define	TIMING_H

#include <stdint.h>
#include"main.h"
void timing(void);
void Reset_Cnt_Flags(void);

extern uint16_t cntLedAlternativ;
extern uint8_t flLedAlternativ;
//extern Bitfeld FlagsTime1;
//extern Bitfeld Flags2;
extern uint8_t cnt1msRoutinen, cnt4msRoutinen, cnt10msRoutinen, cnt100msRoutinen;   
extern volatile uint8_t IR_fl1ms;
extern uint16_t cntTimingMain, cntTiming;

#endif	/* TIMING_H */

