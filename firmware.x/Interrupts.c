#include <xc.h>
#include "p33CK256MP505.h"
#include "Interrupts.h"
#include "SpiConfig.h"
#include "errorloop.h"
#include <stdint.h>

/*
MPLAB® XC16 C Compiler User Guide
15.1.2.1 address (addr)
The address attribute specifies an absolute address for the function.
void __attribute__ ((address(0x100))) foo()
{
...
}
Alternatively, you may define the address in the function prototype:
void foo() __attribute__ ((address(0x100)));  
*/

void __attribute__((interrupt,no_auto_psv,shadow))  _C1RXInterrupt(void)
{   //CAN RX Interrupt
    uint8_t i, filter, notiz;
    
    while(C1FIFOSTA3bits.TFNRFNIF == 1)
    {   ptrCanRxMsg_IR = (CANFD_RX_MSGOBJ *)C1FIFOUA3L;     //Aktuelle FIFO3 Adresse
        filter = ptrCanRxMsg_IR -> bF.ctrl.FilterHit;
        switch(filter)
        {   case 1:                     //Filter1, Standard ID+1, Param[32-39]
            for(i=0;i<8;i++)
            {   Param[32+i] = ptrCanRxMsg_IR -> byte[8+i];  
            }
            break;
            case 2:                     //Filter2, Standard ID+2, Param[40-47]
            for(i=0;i<8;i++)
            {   Param[40+i] = ptrCanRxMsg_IR -> byte[8+i];  
            }    
            break; 
            case 3:                     //Filter3, Standard ID+3, Param[48-55]
            for(i=0;i<8;i++)
            {   Param[48+i] = ptrCanRxMsg_IR -> byte[8+i];  
            }   
            break; 
            case 4:                     //Filter4, Standard ID+4, DNCO
            //Byte8 index-DNCO[], byte12-15 = 4 dnco-werte.
            //64 Message x 4 byte = 256 Byte.
            notiz = ptrCanRxMsg_IR -> byte[8];  //DNCO index
            for(i=0; i<4; i++)
            {  DNCO[notiz+i] = ptrCanRxMsg_IR -> byte[12+i];
            }    
            break; 
            //case 5-7:                 //Filter5, Standard ID+5-7, reserviert
            //break; 
        }
        C1FIFOCON3Lbits.UINC = 1;       //FIFO3 Zeiger auf nächste Message
        __builtin_nop();
        __builtin_nop();
        
        cntCanTimeOut = kCanTimeOut;
    }
    IFS1bits.C1RXIF = 0;  
}

void __attribute__((interrupt,no_auto_psv,shadow)) _T1Interrupt(void) //--------------------------
{   //uint8_t AnzahlByte, ErrorMerker, i;  
    IR_fl1ms = 0xFF;
    IFS0bits.T1IF = 0;
} //-----------------------------------------------------------------------------------------------

void __attribute__((interrupt,no_auto_psv,shadow)) _CCP1Interrupt(void)
{   uint32_t captureL = 0, captureH = 0, dummy;
    //Richtungserkennung
    if(PORTCbits.RC10 == 0) Richtung_IR = _Links;
    else			        Richtung_IR = _Rechts;
    //Periodenmessung: Differenz von neuem und alten Wert
    if(CCP1STATLbits.ICBNE == 1)
    {   captureL = CCP1BUFL;
        captureH = CCP1BUFH;
    }
    if(CCP1STATLbits.ICBNE == 1)
    {   dummy = CCP1BUFL;
        dummy = CCP1BUFH;
    }
    CaptureNeu_IR = ((captureH << 16) + captureL);
    Periode_IR = CaptureNeu_IR - CaptureAlt_IR;
    CaptureAlt_IR = CaptureNeu_IR;
    flNewCapture_IR = 1;
    
    IFS0bits.CCP1IF = 0;
}


void __attribute__((interrupt,no_auto_psv,shadow)) _ADCInterrupt(void)
{   //Ch3 = L3-Integrator, Ch4 = 1,65V, Ch9 = EMK-Spannung
    //Ch13 = Temperatur, Ch14 = 12V, Ch17=3,3V-Test
    //Ergebnisse entsprechend der Channels in ADCBUF3,4,9,13,14 und 17
    //Alle 0,5ms wird der ADC Scan getriggert in analogFrqDet().
    //Nach 50µs ist der Scan fertig, der ADCInterrupt wird ausgelöst.
    //Die Werte werden aus den ADCBUF3-17 ausgelesen. Scan im IR getriggert.
    //Nachdem 4 Scans durch sind, wird der Scan im IR nicht getriggert, 
    //sondern wieder in analogFrqDet(), damit werden wieder 4 Scans gestartet.
    //Nach 4 Scans werden 4 Werte nach PhaseV_1-4_IR geschrieben, 
    //zur Übernahme von analogFrqDet().
    //Die Ergebnisse im Buffer im 10Bit Format müssen mit >>2 angepasst werden.
    //Nach 8 Scans werden 8 aufsummierte EMK-Werte durch 8 geteilt und
    //auf EMK_avg_IR geschrieben, zur Übernahme der EMK Auswertung.
    //###test###TODO Erweiterte Drahtbrucherkennung implementieren############
    //###test### evetuell DiffVerst Wert aufsummieren und Durchschnitt bilden    
    volatile static uint8_t stmADC = 1; //durchläuft 1-8
    
    //TRISBbits.TRISB14 = 0;  //###test###
    //LATBbits.LATB14 = 1;    //###test###
 
    switch(stmADC)
    {   case 1: //--------------------------------------------------------------
        buffV_1_IR = ((ADCBUF3 >> 2) & 0x3FF); ADCBUF3 = 0;
        ADC1V65_IR = ((ADCBUF4 >> 2) & 0x3FF); ADCBUF4 = 0;
        EMK_Peak_IR = ((ADCBUF9 >> 2) & 0x3FF); ADCBUF9 = 0;
        EMK_IR = EMK_Peak_IR;
        DiffVerst_IR = ((ADCBUF12 >> 2) & 0x3FF); ADCBUF12 = 0; //Erweiterte Drahtbrucherkennung
        ADC_DrBrSp_IR = ((ADCBUF13 >> 2) & 0x3FF); ADCBUF13 = 0;   //Schaltspannung Drahtbrucherkennung 2,9Hz Signal
        ADC12V_IR =  ((ADCBUF14 >> 2) & 0x3FF); ADCBUF14 = 0;
        ADC3V3_IR =  ((ADCBUF17 >> 2) & 0x3FF); ADCBUF17 = 0;
        stmADC = 2;
        ADCON3Lbits.SWCTRG = 1;   //triggern des Scan von Ch3-Ch17
        break;
        case 2: //--------------------------------------------------------------
        buffV_2_IR = ((ADCBUF3 >> 2) & 0x3FF); ADCBUF3 = 0;
        ADC1V65_IR = ((ADCBUF4 >> 2) & 0x3FF); ADCBUF4 = 0;
        EMK_Peak_IR = ((ADCBUF9 >> 2) & 0x3FF); ADCBUF9 = 0;
        EMK_IR += EMK_Peak_IR;
        DiffVerst_IR = ((ADCBUF12 >> 2) & 0x3FF); ADCBUF12 = 0;
        ADC_DrBrSp_IR = ((ADCBUF13 >> 2) & 0x3FF); ADCBUF13 = 0;
        ADC12V_IR =  ((ADCBUF14 >> 2) & 0x3FF); ADCBUF14 = 0;
        ADC3V3_IR =  ((ADCBUF17 >> 2) & 0x3FF); ADCBUF17 = 0;
        stmADC = 3;
        ADCON3Lbits.SWCTRG = 1;   //triggern des Scan von Ch3-Ch17
        break;
        case 3: //--------------------------------------------------------------
        buffV_3_IR = ((ADCBUF3 >> 2) & 0x3FF); ADCBUF3 = 0;
        ADC1V65_IR = ((ADCBUF4 >> 2) & 0x3FF); ADCBUF4 = 0;
        EMK_Peak_IR = ((ADCBUF9 >> 2) & 0x3FF); ADCBUF9 = 0;
        EMK_IR += EMK_Peak_IR;
        DiffVerst_IR = ((ADCBUF12 >> 2) & 0x3FF); ADCBUF12 = 0;
        ADC_DrBrSp_IR = ((ADCBUF13 >> 2) & 0x3FF); ADCBUF13 = 0;
        ADC12V_IR =  ((ADCBUF14 >> 2) & 0x3FF); ADCBUF14 = 0;
        ADC3V3_IR =  ((ADCBUF17 >> 2) & 0x3FF); ADCBUF17 = 0;
        stmADC = 4; 
        ADCON3Lbits.SWCTRG = 1;   //triggern des Scan von Ch3-Ch17
        break;
        case 4: //--------------------------------------------------------------
        buffV_4_IR = ((ADCBUF3 >> 2) & 0x3FF); ADCBUF3 = 0;
        ADC1V65_IR = ((ADCBUF4 >> 2) & 0x3FF); ADCBUF4 = 0;
        EMK_Peak_IR = ((ADCBUF9 >> 2) & 0x3FF); ADCBUF9 = 0;
        EMK_IR += EMK_Peak_IR;
        DiffVerst_IR = ((ADCBUF12 >> 2) & 0x3FF); ADCBUF12 = 0;
        ADC_DrBrSp_IR = ((ADCBUF13 >> 2) & 0x3FF); ADCBUF13 = 0;
        ADC12V_IR =  ((ADCBUF14 >> 2) & 0x3FF); ADCBUF14 = 0;
        ADC3V3_IR =  ((ADCBUF17 >> 2) & 0x3FF); ADCBUF17 = 0;
        PhaseV_1_IR = buffV_1_IR;
        PhaseV_2_IR = buffV_2_IR;
        PhaseV_3_IR = buffV_3_IR;
        PhaseV_4_IR = buffV_4_IR;
        stmADC = 5;
        ADCgetriggert_IR = 0;      //Handshaking mit analogFrqDetection()
        //Die Werte von case_4 werden von analogFrqDetection() übernommen
        //Scan wird von analogFreqDet() getriggert
        break;
        case 5: //--------------------------------------------------------------
        buffV_1_IR = ((ADCBUF3 >> 2) & 0x3FF); ADCBUF3 = 0;
        ADC1V65_IR = ((ADCBUF4 >> 2) & 0x3FF); ADCBUF4 = 0;
        EMK_Peak_IR = ((ADCBUF9 >> 2) & 0x3FF); ADCBUF9 = 0;
        EMK_IR += EMK_Peak_IR;
        DiffVerst_IR = ((ADCBUF12 >> 2) & 0x3FF); ADCBUF12 = 0;
        ADC_DrBrSp_IR = ((ADCBUF13 >> 2) & 0x3FF); ADCBUF13 = 0;
        ADC12V_IR =  ((ADCBUF14 >> 2) & 0x3FF); ADCBUF14 = 0;
        ADC3V3_IR =  ((ADCBUF17 >> 2) & 0x3FF); ADCBUF17 = 0;
        stmADC = 6;  
        ADCON3Lbits.SWCTRG = 1;   //triggern des Scan von Ch3-Ch17
        break;
        case 6: //--------------------------------------------------------------
        buffV_2_IR = ((ADCBUF3 >> 2) & 0x3FF); ADCBUF3 = 0;
        ADC1V65_IR = ((ADCBUF4 >> 2) & 0x3FF); ADCBUF4 = 0;
        EMK_Peak_IR = ((ADCBUF9 >> 2) & 0x3FF); ADCBUF9 = 0;
        EMK_IR += EMK_Peak_IR;
        DiffVerst_IR = ((ADCBUF12 >> 2) & 0x3FF); ADCBUF12 = 0;
        ADC_DrBrSp_IR = ((ADCBUF13 >> 2) & 0x3FF); ADCBUF13 = 0;
        ADC12V_IR =  ((ADCBUF14 >> 2) & 0x3FF); ADCBUF14 = 0;
        ADC3V3_IR =  ((ADCBUF17 >> 2) & 0x3FF); ADCBUF17 = 0;
        stmADC = 7;
        ADCON3Lbits.SWCTRG = 1;   //triggern des Scan von Ch3-Ch17
        break;
        case 7: //--------------------------------------------------------------
        buffV_3_IR = ((ADCBUF3 >> 2) & 0x3FF); ADCBUF3 = 0;
        ADC1V65_IR = ((ADCBUF4 >> 2) & 0x3FF); ADCBUF4 = 0;
        EMK_Peak_IR = ((ADCBUF9 >> 2) & 0x3FF); ADCBUF9 = 0;
        EMK_IR += EMK_Peak_IR;
        DiffVerst_IR = ((ADCBUF12 >> 2) & 0x3FF); ADCBUF12 = 0;
        ADC_DrBrSp_IR = ((ADCBUF13 >> 2) & 0x3FF); ADCBUF13 = 0;
        ADC12V_IR =  ((ADCBUF14 >> 2) & 0x3FF); ADCBUF14 = 0;
        ADC3V3_IR =  ((ADCBUF17 >> 2) & 0x3FF); ADCBUF17 = 0;
        stmADC = 8;
        ADCON3Lbits.SWCTRG = 1;   //triggern des Scan von Ch3-Ch17
        break;
        case 8: //--------------------------------------------------------------
        buffV_4_IR = ((ADCBUF3 >> 2) & 0x3FF); ADCBUF3 = 0;
        ADC1V65_IR = ((ADCBUF4 >> 2) & 0x3FF); ADCBUF4 = 0;
        EMK_Peak_IR = ((ADCBUF9 >> 2) & 0x3FF); ADCBUF9 = 0;
        EMK_IR += EMK_Peak_IR;
        DiffVerst_IR = ((ADCBUF12 >> 2) & 0x3FF); ADCBUF12 = 0;
        ADC_DrBrSp_IR = ((ADCBUF13 >> 2) & 0x3FF); ADCBUF13 = 0;
        ADC12V_IR =  ((ADCBUF14 >> 2) & 0x3FF); ADCBUF14 = 0;
        ADC3V3_IR =  ((ADCBUF17 >> 2) & 0x3FF); ADCBUF17 = 0;
        PhaseV_1_IR = buffV_1_IR;
        PhaseV_2_IR = buffV_2_IR;
        PhaseV_3_IR = buffV_3_IR;
        PhaseV_4_IR = buffV_4_IR;
        EMK_avg_IR = ((EMK_IR >> 3) & 0x3FF); //Durchschnitt ist EMK_IR/8, auf 10bit begrenzt.
        EMK_IR = 0;
        stmADC = 1;
        ADCgetriggert_IR = 0;      //Handshaking mit analogFrqDetection()
        //Die Werte von case_8 werden von analogFrqDetection() übernommen
        //Scan wird von analogFreqDet() getriggert
        break;
    }
    cntAdcIrTO = 0;
    IFS6bits.ADCAN17IF = 0;     //IF löschen in ISR
    IFS5bits.ADCIF = 0;
    
    //LATBbits.LATB14 = 0;    //###test###
}

/*
//traps***********************************************
void __attribute__((interrupt,no_auto_psv)) _OscillatorFail(void)
{   ErrorLoop(0x21);
}

void __attribute__((interrupt,no_auto_psv)) _AddressError(void)
{   ErrorLoop(0x22);
}

void __attribute__((interrupt,no_auto_psv)) _StackError(void)
{   ErrorLoop(0x23);
}

void __attribute__((interrupt,no_auto_psv)) _MathError(void)
{   ErrorLoop(0x24);
}
*/

void __attribute__((interrupt,no_auto_psv,shadow)) _DefaultInterrupt(void)
{   //DefaultInterrupt wird angesprungen, wenn ein nicht implementierter IR ausgelöst wird
    //VECNUM enthält die Interrupt Vector Nummer (von 0 bis 203 dez. des IRs.
    ErrorLoop(INTTREGbits.VECNUM);
}

