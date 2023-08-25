#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#include <stdint.h>
#include "main.h"
#include "Can.h"
#include "Frequenz.h"
//#include "Transistoren.h"
#include "SpiConfig.h"
#include "CCPconfig.h"

//###test####################notwendig???####################################
void __attribute__((interrupt,no_auto_psv)) _DMA0Interrupt(void);
void __attribute__((interrupt,no_auto_psv)) _DMA1Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _C1Interrupt(void);  //CAN
void __attribute__((interrupt,no_auto_psv,shadow)) _U1RXInterrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _T1Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _INT0Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _CCT2Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _CCT3Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _CCT4Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _CCP1Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _CCP2Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _CCP3Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _CCP4Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _ADCAN0Interrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _SPI1RXInterrupt (void);
void __attribute__((interrupt,no_auto_psv,shadow)) _SPI1TXInterrupt(void);
void __attribute__((interrupt,no_auto_psv,shadow)) _INT0Interrupt(void);
/*Traps*/
void __attribute__((interrupt,no_auto_psv)) _OscillatorFail(void);
void __attribute__((interrupt,no_auto_psv)) _AddressError(void);
void __attribute__((interrupt,no_auto_psv)) _StackError(void);
void __attribute__((interrupt,no_auto_psv)) _MathError(void);
//void __attribute__((interrupt,no_auto_psv)) _DMACError(void);

extern volatile uint16_t __attribute__((aligned(4), address(0x1060) ))CanBuffer[];
//extern volatile uint16_t __attribute__((aligned(4), address(0x2010) ))CanBuffer[];
extern volatile CANFD_RX_MSGOBJ* ptrCanRxMsg_IR;

//extern volatile unionCanBuf IRCanTransferBuf;
extern volatile uint8_t IR_fl1ms;
extern volatile uint8_t Param[64];
extern volatile uint8_t DNCO[256];
//extern volatile uint8_t IR_CanRx;
//extern volatile uint32_t PositionNeu1_ISR, PositionNeu2_ISR, posHold_ISR;
//extern volatile uint32_t CaptNeu1_ISR, CaptAlt1_ISR, CaptDiff1_ISR;
//extern volatile uint32_t CaptNeu2_ISR, CaptAlt2_ISR, CaptDiff2_ISR;
//extern volatile uint8_t CaptState1_ISR, CaptState2_ISR;
//extern volatile uint16_t cntCCP1_ISR, cntCCP2_ISR;
//extern volatile uint8_t CCP_UW1_Off, CCP_UW2_Off;
extern volatile uint32_t Periode_IR, CaptureNeu_IR, CaptureAlt_IR;
extern volatile uint8_t flNewCapture_IR;
extern volatile uint8_t Richtung_IR;
extern volatile uint16_t ADC1V65_IR, ADC3V3_IR, ADC12V_IR, ADC_DrBrSp_IR;
//extern uint16_t          ADC1V65,    ADC3V3,    ADC12V,    ADC_DrBrSp;
extern volatile uint16_t EMK_IR, EMK_avg_IR;
//extern uint16_t          EMK_avg;
extern volatile uint16_t  EMK_Peak_IR; 
//extern uint16_t           EMK_Peak;
extern volatile uint16_t PhaseV_1_IR,PhaseV_2_IR,PhaseV_3_IR,PhaseV_4_IR;
extern volatile uint16_t  buffV_1_IR, buffV_2_IR, buffV_3_IR, buffV_4_IR;
//extern uint16_t          PhaseV_1,   PhaseV_2,   PhaseV_3,   PhaseV_4;
extern volatile uint16_t DiffVerst_IR;
extern uint8_t cntCanTimeOut;
volatile extern uint8_t cntAdcIrTO;
extern volatile uint8_t ADCgetriggert_IR;

#endif	/* INTERRUPTS_H */

