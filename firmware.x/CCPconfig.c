#include <xc.h>
#include "p33CK256MP505.h"
#include "CCPconfig.h"
#include "Frequenz.h"
#include <stdint.h>

volatile uint32_t CaptureNeu_IR = kMaxMesswert, CaptureAlt_IR = kMaxMesswert;  
volatile uint32_t Periode_IR = kMaxMesswert;
volatile uint8_t flNewCapture_IR = 0;
volatile uint8_t Richtung_IR = 0;
uint32_t Periode = kMaxMesswert;
uint8_t flNewCapture = 0;
uint8_t Richtung = 0;

void CCPconfig(void)
{       
    Periode_IR = kMaxMesswert;
    Periode = kMaxMesswert;
    CaptureNeu_IR = kMaxMesswert;
    CaptureAlt_IR = kMaxMesswert;
    flNewCapture_IR = 0;
    flNewCapture = 0;
    //CCP1 Pin wird in init() konfiguriert.
    CCP1CON1Lbits.TMRSYNC = 0;
    //Erratasheet: When FOSC is selected as the clock source using
    //the CLKSEL[2:0] bits (CCPxCON1L[10:8]), unexpected operation may occur).
    CCP1CON1Lbits.CLKSEL = 1;   //REFCLK 1,25MHz 
    CCP1CON1Lbits.TMRPS = 0;    //Prescaler 1
    CCP1CON1Lbits.T32 = 1;      //32Bit Modus
    CCP1CON1Lbits.CCSEL = 1;    //Capture Input modus
    CCP1CON1Lbits.MOD = 1;      //1=Capture every rising edge, 2=every falling edge
    CCP1CON2Hbits.ICS = 0;      //InputCapture Pin
    IPC1bits.CCP1IP = 6;        //IR priority
    IFS0bits.CCP1IF = 0;        //IR Flag
    IEC0bits.CCP1IE = 1;        //IR enabled
    CCP1CON1Lbits.CCPON = 1;    //CCP1 ist in Betrieb
    //CCP1STATLbits.ICOV        //ICOV==1: Overflow
    //CCP1STATLbits.ICBNE       //ICBNE==1: 32Bit 2-fach Buffer not empty
    //CCP1BUFH / CCP1BUFL       //32Bit 2-fach Buffer, zuerst L auslesen, dann H
    //If the module is disabled, and then re-enabled in Input Capture
    //mode later, the FIFO buffer contents will be undefined, and a read will yield indeterminate results.
    //The FIFO Pointer is adjusted whenever the most significant word of the buffer result is read by
    //the CPU. This allows the results of a 32-bit Input Capture to be read by the 16-bit CPU.
    
    //Fehlerbehandlung Overflow: In timing() wird CCP1STATLbits.ICOV gelöscht,
    //damit durch einen Overflow die CCP-IRs nicht blockiert werden.
}
