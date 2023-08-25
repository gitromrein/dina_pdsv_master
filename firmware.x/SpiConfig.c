#include <xc.h>
#include "p33CK256MP505.h"
#include "SpiConfig.h"
#include <stdint.h>

/*
volatile uint8_t BissFrame1ISR[kMaxFrameLength] = {0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0};
volatile uint8_t BissFrame2ISR[kMaxFrameLength] = {0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0};
volatile uint8_t InfoBiss1ISR = 0, InfoBiss2ISR = 0;
volatile uint8_t StatusBiss1ISR = 0, StatusBiss2ISR = 0;
volatile uint8_t cntBiss1TOISR = 0, cntBiss2TOISR = 0;
uint8_t BissFrame1[kMaxFrameLength] = {0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0};
uint8_t BissFrame2[kMaxFrameLength] = {0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0};
uint8_t InfoBiss1 = 0, InfoBiss2 = 0;
uint8_t StatusBiss1 = 0, StatusBiss2 = 0;
uint8_t cntBiss1TO = 0, cntBiss2TO = 0;
uint32_t PositionBiss1 = 0, PositionBiss2 = 0;
uint32_t PosCPW1 = 0, PosSPW1 = 0, PosCPW2 = 0, PosSPW2 = 0;
uint8_t LC1 = 0, LC2 = 0;



//#define ReadStatus      0x9C    //reads 8 Bits, parts from DeviceStatus und DeviceError Registers
//#define ReadFrameData   0xA6    //reads INFO byte, CPW...SPW...FrameNumberCD
//#define ReadRegister    0x8A
//#define WriteRegister   0xD2
*/

void SpiConfig(void) //---------------------------------------------------------
{
    uint8_t dummy;
      
    //LED Ansteuerung
    //RC4 = SPI1-CS, muss als Portpin vom Programm gesteuert werden.
    LATCbits.LATC4 = 1;
    TRISCbits.TRISC4 = 1;          //CS1 verhindert Glitsch bei einschalten
    TRISBbits.TRISB8 = 1;          //SDO1
    SPI1BRGL = 39;           //49       //40MHz/2/50 = 0,5MHz
    SPI1CON1Lbits.MSTEN = 1;        //Master mode
    SPI1CON1Lbits.CKP = 0;          //CKP=0 + CKE=1 ist ok
    SPI1CON1Lbits.SSEN = 0;
    SPI1CON1Lbits.CKE = 1;          //CKP=0 + CKE=1 ist ok
    SPI1CON1Lbits.MODE16 = 0;       //8bit modus
    SPI1CON1Lbits.MODE32 = 0;       //8bit modu
    SPI1CON1Lbits.DISSDO = 0;       //SDO Pin enabled
    SPI1CON1Lbits.DISSCK = 0;       //SCK Pin enabled
    SPI1CON1Lbits.DISSDI = 1;       //SDI Pin disabled
    SPI1CON1Lbits.SPIFE = 0;
    SPI1CON1Lbits.ENHBUF = 0;
    
    SPI1CON1Hbits.MSSEN = 1;        //SS1 Pin controlled by Master
    SPI1CON2L = 0;
    
    SPI1CON1Lbits.SPIEN = 1;        //SPI1 enabled
    //dummy = SPI1BUFH;
    dummy = SPI1BUFL;
    SPI1STATLbits.FRMERR = 0;
    SPI1STATLbits.SPIROV = 0;
    
    TRISCbits.TRISC4 = 0;          //SS1 Ausgang
    //LATCbits.LATC4 = 0;          //SS1 Ausgang wird durch SPI Modul gesteuert
    TRISBbits.TRISB8 = 0;          //SDO1 Ausgang
    
    
} //----------------------------------------------------------------------------
