#ifndef ADC_H
#define	ADC_H

#include <stdint.h>
//#include "Transistoren.h"   //#######test#########
#include "FrequenzVergleich.h"
#include "Uart.h"

//Der ADC arbeitet mit 10Bit Auflösung (0V-3,3V entspr. 0-1023)
#define     k8Bit0V1          7
#define     k8Bit0V2         15   
#define     k8Bit0V5         79
#define     k8Bit3V0        232
#define     k8Bit3V1        240
#define     k3V1            961
#define     k0V2             62
#define     k3V3TestMax     563     //563 = 1023/2*1,1 = 1,81V
#define     k3V3TestMin     459     //459 = 1023/2*0,9 = 1,48V
#define     k1V65TestMax    563     //563 = 1023/2*1,1 = 1,81V
#define     k1V65TestMin    459     //459 = 1023/2*0,9 = 1,48V
#define     kDrBrSpLow       77     // 77 = 1023/3,3V*0,25V.  0,25V -> 1V        bei Spannungst. 3/1
#define     kDrBrSpHighMin  446     //446  = 1023/3,3V*1,44V. 1,44V=6,8V/4*0,85  bei Spannungst. 3/1
#define     kDrBrSpHighMax  604     //604  = 1023/3,3V*1,95V. 1,95V=6,8V/4*1,15  bei Spannungst. 3/1
//Betreff 12V Versorgung/Test: 
//Alte Bestückung 10K/3K3,      Änderung 10K/2K2
// 12V = 2,997V = 929.          12V = 2,164V = 661
// 15V = 3,722V = 1023/Fehler   15V = 2,704V = 838 
// 10V = 2,481V = 769           10V = 1,803V = 559
// Max: 13,3V = 3,3V = 1023     Max: 18,3V = 3,3V = 1023
//Bestückung 10K/2K2:
#define     k12VTestMax     824     //824 = 1023/3,3V*2,65V. Spannungsteiler: 14,7V/12,2K *2,2K = 2,65V
#define     k12VTestMin     559     //559 = 1023/3,3V*1,803V. Spannungsteiler: 10V/12,2K *2,2K = 1,803V
//Bestückung alt 10K/3K3:
//#define     k12VTestMax     1023     //1023 = 1023/3,3V*3,300V. Spannungsteiler: 15V/13,3K *3,3K = 3,300V Fehler
//#define     k12VTestMin     769      // 769 = 1023/3,3V*2,481V. Spannungsteiler: 10V/13,3K *3,3K = 2,481V


#define     kErrAnalogSignalDet 30		//30 bei +=2 : 15 x 200ms = 3s. analogFrqDetection().

void analogFrqDetection(void);
//void DrahtbruchErkennung(void);
void DrahtbruchErkennung(uint8_t Pegel);
//void getEmkSpannung(void);
void getEmkSpannung(uint8_t stateCnt, uint8_t Pause);
void stm_EMK_DrBrErk(void);
void ADCauswerten(void);
void checkDiffVerst(void);
void checkDrBrSpannung(uint8_t Pegel);
void ADC_Config(void);

#endif	/* ADC_H */

