#ifndef FREQUENZ_H
#define	FREQUENZ_H


#include <stdint.h>
#include "p33CK256MP505.h"
#include "Uart.h"

enum { FrqBereich_0 = 0, FrqBereich_2_4, FrqBereich_6 };
enum { Hyst_375mV = 0, Hyst_630mV, Hyst_1V33, Hyst_1V88 };
     //Hyst_1           Hyst_2      Hyst_3      Hyst_4

void getFrequenz(void);
void initFreqMessung(void);

#define SizePS 132 //Size of ParamStruct
typedef union
{   struct
    {   uint16_t arrFrqLimits[4][4];  //[Limits  0-3][Validierung 0-3]
        uint16_t arrFrqHyst[4][4];    //[Hyster  0-3][Validierung 0-3]
        uint16_t arrFrqTimeout[4][4]; //[Timeout 0-3][Validierung 0-3]
        uint16_t FrqSchwelle;
        uint16_t Anlaufzeit;
        //100 Byte
        uint16_t EMKEinschaltverz;         
        uint16_t EMKEmpfkeit;
        uint16_t EMKEinschaltschw;
        uint16_t EMKAbschaltschw;
        uint8_t Abschaltverz;
        uint8_t Validierstufen;
        //110 Byte
        uint8_t Hysteresemode;       
        uint8_t XfachMessung;
        uint8_t Entprellung;
        uint8_t SkrQSErk;
        uint8_t SkrAutoMan;
        uint8_t SkrQDelay;
        uint8_t SkrAntival;
        uint8_t SkrPowerOn;
        uint8_t SkrPegel;
        uint8_t SkrAktiv;
        //120 Byte
        uint8_t CanRefresh;
        uint8_t ControlBits; //b0 KapKopl,b1 DZ/Pos,b2 frei,b3 sicher Stopp, 
                             //b4 Mute Sensorik,b5 LR invert.,b6 LR & Stillst,b7 LR deaktiviert       
        uint8_t PhasenUW;
        uint8_t DrehzahlMode;
        uint8_t EmkMode;
        uint8_t DNCO;        //b0 aktiv,b4 0=Limit0-3 per DNCO, 1=Limit3 per DNCO
        uint16_t FrqVglSchw; //Frequenz Vergeich Schwelle. 2=2Hz, 3=3Hz. Auf 50Hz begrenzen
        uint8_t ProzentAbschVerz;
        uint8_t dummy1;
        //130 Byte
        uint8_t dummy2;
        uint8_t dummy3;     
        //zus. 132 Byte
    } Par;
    uint8_t Bytes[SizePS];
} ParamStruct;

#define kMaxMesswert    5000000 //entspricht 0,25Hz bei 1,25Mhz Zählfrequenz
#define k1HzPeriode     1250000
#define k2HzPeriode		625000	//Messwert bei 2Hz
#define k5HzPeriode		250000	//Messwert bei 5Hz
#define k10HzPeriode	125000	//Messwert bei 10Hz
#define k25HzPeriode	50000	//Messwert bei 25Hz
#define k75HzPeriode	16666	//Messwert bei 75Hz
#define k100HzPeriode	12500	//Messwert bei 100Hz
#define k130HzPeriode	9615	//Messwert bei 130Hz
#define k150HzPeriode	8333	//Messwert bei 150Hz
#define k200HzPeriode	6250	//Messwert bei 200Hz
#define k1500HzPeriode	833		//Messwert bei 1500Hz

#define kTOMessung		400     //entspricht 4s
//#define k10Sek			1000	//entspricht 10s bei 10ms decr.

#define _KapKopplung    LATBbits.LATB12 //###test###

#define FunktionsModus (Param[11] & 0x0F)
  
extern volatile uint32_t Periode_IR;
extern volatile uint8_t flNewCapture_IR;
extern volatile uint8_t Richtung_IR;
extern uint32_t Periode;
extern uint8_t flNewCapture;
extern uint8_t Richtung;
extern uint8_t CanTX[];
extern uint8_t cntDrahtBruchErr;
extern uint8_t fl10ms;
extern str_ipk RX, TX;
extern uint8_t RichtungStatus;
extern uint8_t flNeueFlanke;


#endif	/* FREQUENZ_H */

