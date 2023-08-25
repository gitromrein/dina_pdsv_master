#ifndef UART_H
#define	UART_H

#include <stdint.h>

#define ProtokollUart   20  //###test###18     //Array SerRX/TX[] 18byte groß
#define ProtokollEnde    0xAA

//IPK Struktur
typedef union
{   struct
    {   //byt0
        unsigned Frametyp:2;
        unsigned _1_2:1;            //frei
        unsigned TO1s:1;
        unsigned Hyst1:1;
        unsigned Hyst2:1;
        unsigned ParamKompl:1;
        unsigned SLOKoff:1;
        //byte1
        unsigned IN1:1;
        unsigned IN2:1;
        unsigned IN3:1;
        unsigned IN4:1;
        unsigned _1_4:1;            //frei
        unsigned _1_5:1;            //frei
        unsigned _1_6:1;            //frei
        unsigned _1_7:1;            //frei
        //byte2
        unsigned F1:1;
        unsigned F2:1;
        unsigned F3:1;
        unsigned F4:1;
        unsigned Val:2;
        unsigned _2_6:1;            //frei
        unsigned _2_7:1;            //frei
        //byte3
        unsigned ZH:1;
        unsigned _3_1:1;            //frei
        unsigned _3_2:1;            //frei
        unsigned _3_3:1;            //frei
        unsigned SKR1:1;
        unsigned SKR2:1;
        unsigned _3_6:1;            //frei
        unsigned _3_7:1;            //frei
        //byte4
        unsigned Limit0:1;
        unsigned Limit1:1;
        unsigned Limit2:1;
        unsigned Limit3:1;
        unsigned Quitt:1;
        unsigned Richtung: 1;
        unsigned Beschleunigung: 1; //reserviert
        unsigned _4_7: 1;           //frei
        //byte5
        unsigned Q0:1;
        unsigned Q1:1;
        unsigned Q2:1;
        unsigned Q3:1;
        unsigned _5_4:1;            //frei
        unsigned _5_5:1;            //frei
        unsigned EMK:1;
        unsigned EMK_Verz:1;
        //byte6-10
        unsigned _6:8;              //Istfrq H 
        unsigned _7:8;              //Istfrq L
        unsigned _8:8;              //EMK Spannung
        unsigned _9:8;              //frei
        unsigned _10:8;             //frei
        //byte11
        unsigned Err3V3:1;
        unsigned Err1V65:1;
        unsigned Err12V:1;
        unsigned ErrCanTO:1;
        unsigned ErrIPK:1;
        unsigned ErrParCanCRC:1;     //CAN Parameter, übertragene CRC16 ist falsch
        unsigned ErrParCRCVgl:1;     //Laufzeitparameter IPK Vergleich Fehler
        unsigned ErrFW:1;
        //byte12
        unsigned ErrDrahtbrPuls:1;  //Fehlerbit wenn keine 6,8V DrBrSpannung
        unsigned ErrParPlaus:1;      //Berechnete Laufzeitparameter sind nicht plausibel
        unsigned SoftErr:1;
        unsigned ErrEMK1Kan:1;
        unsigned ErrPortTest:1;
        unsigned ErrEingTest:1;
        unsigned ErrEingUngleich:1;
        unsigned ErrTimeOutADC:1;
        //byte13
        unsigned ErrDrahtBr:1;
        unsigned ErrExtendedDrBr:1;
        unsigned ErrPhasenlage:1;
        unsigned ErrRichtung:1;
        unsigned Err1500Hz:1;
        unsigned ErrFrqVgl:1;
        unsigned ErrAnalogDigital:1;
        unsigned ErrQuitt:1;
        //byte14
        unsigned ErrSKR1:1;
        unsigned ErrSKR2:1;
        unsigned ErrQuersSKR1:1;
        unsigned ErrQuersSKR2:1;
        unsigned ErrGrundstSKR1:1;
        unsigned ErrGrundstSKR2:1;
        unsigned _14_6:1;           //frei
        unsigned _14_7:1;           //frei
        //byte15-19
        unsigned _15:8;             //DNCO
        unsigned _16:8;             //CRC16 H
        unsigned _17:8;             //CRC16 L
        unsigned _18:8;             //FW Version
        unsigned _19:8;             //Frame End 0xAA
    } bf;
    uint8_t Bytes[ProtokollUart]; //20
} str_ipk;

//IPK Daten Index-Verzeichnis ganzer Byte
#define IPK_FreqH   6
#define IPK_FreqL   7
#define IPK_EMK     8
#define IPK_PosH    9
#define IPK_PosL    10
#define IPK_DNCO    14
#define IPK_frei1   15
#define IPK_frei2   16
#define IPK_ChsPar  17
#define IPK_FW      18
#define IPK_End     19


void UartConfig(void);
void UartRxTx(void);
//void UartRxTx_2(void);  //###test###
void SteckplatzAustauschen(uint16_t SteckplatzNr);
void IpkDatenPuffern(void);
void UartDebug(uint16_t wert1, uint16_t wert2);

//extern uint8_t SerTX[], SerRX[];
extern uint8_t SerRXU[], SerTXU[], SerRXB[], SerTXB[];
//extern uint16_t IpkLatchIstIX1_a,IpkLatchIstIX1_b;
extern uint16_t IstDzIX_verwendet1;
extern uint8_t CanTX[];
extern uint8_t flParamKomplett, cnt1msRoutinen;
extern uint16_t ADC3V3, ADC1V65;
extern uint8_t flLatchTX, flLatchRX;

//extern str_ipk TX;
#endif	/* UART_H */

