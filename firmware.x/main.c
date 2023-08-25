// main.c
// Lothar Bauknecht
// 01.02.2020

// Um eine neue Version anzulegen folgendermaßen vorgehen:
// Projekt schließen.
// Kompletter Projektordner kopieren und auf neue Version umbenennen.
// Z.B: C:\PC-Vario\dsv2\pic33\dsv-2-v0002-mc806\trunk\dsv-2-v0002-mc806 
// nach C:\PC-Vario\dsv2\pic33\dsv-2-v0003-mc806\trunk\dsv-2-v0003-mc806
// Das Projekt von dem neuen Pfad öffnen und das Projekt ebenso umbenennen
// mit rechte Maustaste auf das "Projekt", Menüpunkt Rename anklicken.
// Löschen der alten Dateien in
// ...dist\default\production\dbiv-v1.03.production.elf, -.hex, -.map
// Projekt-Properties / Building / Apply anklicken.
// Danach "Alles speichern", dann kompilieren.

// Hinweis RomTest:
// Compiler Einstellung:
// xc16-ld, Fill Flash Memory, Fill unused Flash, mit Constante 0xFFFFFF ###test###unbedingt notwendig????###
// Pro Adresse werden 2x 16Bit ausgelesen, deswegen AdrRom +=2.
// Rom 16bit Adresse: 0x0000 - 0x7FFF
// RomCopy 16bit Adr: 0x8000 - 0x8FFF
// Mit Elnec Block im 8bit Modus kopieren:
// Block Start        0x0000 - 0xFFFF  
// Destination       0x10000 -0x1FFFF



//------------------------------------------------------------------------------
//Historie: PDSV
//------------------------------------------------------------------------------
//PDSV-V1.00    04.03.2020
//PDSV-V1.10    IPK erweitert

//------------------------------------------------------------------------------

// FSEC
#pragma config BWRP = OFF               // Boot Segment Write-Protect bit (Boot Segment may be written)
#pragma config BSS = DISABLED           // Boot Segment Code-Protect Level bits (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = DISABLED           // General Segment Code-Protect Level bits (No Protection (other than GWRP))
#pragma config CWRP = OFF               // Configuration Segment Write-Protect bit (Configuration Segment may be written)
#pragma config CSS = DISABLED           // Configuration Segment Code-Protect Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = OFF            // Alternate Interrupt Vector Table bit (Disabled AIVT)

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Flash Page Address Limit bits (Enter Hexadecimal value)

// FSIGN

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF       // OSC2 Pin Function bit (OSC2 is clock output)###test###auf OFF ändern
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is enabled/disabled)
#pragma config PLLKEN = OFF              // PLL Lock Status Control (PLL lock signal will be used to disable PLL clock output if lock is lost)
#pragma config XTCFG = G3     //G1      // XT Config (8-16 MHz crystals)
#pragma config XTBST = ENABLE           // XT Boost (Boost the kick-start)


// FWDT
#pragma config RWDTPS = PS1024          // Run Mode Watchdog Timer Post Scaler select bits (1:2147483648)
#pragma config RCLKSEL = LPRC           // Watchdog Timer Clock Select bits (Always use LPRC)
#pragma config WINDIS = ON              // Watchdog Timer Window Enable bit (Watchdog Timer operates in Non-Window mode)
#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
#pragma config SWDTPS = PS1024          // Sleep Mode Watchdog Timer Post Scaler select bits (1:2147483648)
#pragma config FWDTEN = ON_SW           // Watchdog Timer Enable bit (WDT controlled via SW, use WDTCON.ON bit)

// FPOR
#pragma config BISTDIS = DISABLED       // Memory BIST Feature Disable (mBIST on reset feature disabled)

// FICD
#pragma config ICS = PGD3               // ICD Communication Channel Select bits (Communicate on PGC3 and PGD3)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
#pragma config NOBTSWP = DISABLED       // BOOTSWP instruction disable bit (BOOTSWP instruction is disabled)

// FDMTIVTL
#pragma config DMTIVTL = 0xFFFF         // Dead Man Timer Interval low word (Enter Hexadecimal value)

// FDMTIVTH
#pragma config DMTIVTH = 0xFFFF         // Dead Man Timer Interval high word (Enter Hexadecimal value)

// FDMTCNTL
#pragma config DMTCNTL = 0xFFFF         // Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)

// FDMTCNTH
#pragma config DMTCNTH = 0xFFFF         // Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)

// FDMT
#pragma config DMTDIS = OFF             // Dead Man Timer Disable bit (Dead Man Timer is Disabled and can be enabled by software)

// FDEVOPT
#pragma config ALTI2C1 = OFF            // Alternate I2C1 Pin bit (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 Pin bit (I2C2 mapped to SDA2/SCL2 pins)
#pragma config ALTI2C3 = OFF            // Alternate I2C3 Pin bit (I2C3 mapped to SDA3/SCL3 pins)
#pragma config SMBEN = SMBUS            // SM Bus Enable (SMBus input threshold is enabled)
#pragma config SPI2PIN = PPS            // SPI2 Pin Select bit (SPI2 uses I/O remap (PPS) pins)

// FALTREG
#pragma config CTXT1 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits (Not Assigned)
#pragma config CTXT2 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits (Not Assigned)
#pragma config CTXT3 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits (Not Assigned)
//#pragma config CTXT4 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits (Not Assigned)

// FBTSEQ
#pragma config BSEQ = 0xFFF             // Relative value defining which partition will be active after device Reset; the partition containing a lower boot number will be active (Enter Hexadecimal value)
#pragma config IBSEQ = 0xFFF            // The one's complement of BSEQ; must be calculated by the user and written during device programming. (Enter Hexadecimal value)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "p33CK256MP505.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "main.h"
#include "timing.h"
#include "Can.h"
#include "LedAnzeige.h"
#include "Uart.h"
#include "Eingaenge.h"
#include "SKR_ZwHd.h"
//#include "Transistoren.h"
#include "Frequenz.h"
#include "FrequenzVergleich.h"
#include "GetErrors.h"
#include "Auswertung.h"
#include "ADC.h"
#include "Diagnose.h"
#include "PortTest.h"
#include "RamTest.h"

//#define debugging     //In debug session define aktivieren,
                        //da der Debugger den Speicher benötigt
#ifndef debugging
uint8_t __attribute__((address(0x1000)))PlatzhalterDebugging[80];
#endif

uint8_t Typenschild[16] =
        { Hardware,FW1,FW2,FW3,FW4,ArtNr1,ArtNr2,ArtNr3,ArtNr4, 0,0,0,0,0,0,0 };

uint8_t cnt1msRoutinen = 0, cnt4msRoutinen = 0, cnt10msRoutinen = 0, cnt100msRoutinen = 0;
uint8_t flLedAlternativ = 0, flParamKomplett = 0, flErrParCanCRC = 0; 
//uint8_t ix1DNCO = 0, ix2DNCO = 0;
uint16_t Steckplatz = 0, cntLedAlternativ = 0, cntTiming = 0, cntTimingMain = 0, cntTimingC = 0, cntTimingMainC = 0;

//Am Ende von main(): TX.Bytes[] --> Buffer SerTXB[].
uint8_t SerTXB[ProtokollUart] = {0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0xAA};
//Am Ende von main(): Buffer SerRXB[] --> RX.Bytes[]. 
uint8_t SerRXB[ProtokollUart] = {   0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0};
//In SerRxTx(): Buffer SerTXB[] --> SerTXU[] --> Uart senden.
uint8_t SerTXU[ProtokollUart] = {0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0xAA};
//In SerRxTx(): Uart empfangen --> SerRXU[] --> Buffer SerRXB[].
uint8_t SerRXU[ProtokollUart] = {   0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0};

uint8_t CanTX[8] = {Kennung,0,0,0,0,0,0,0};
uint8_t CanRX[8] = {0,0,0,0,0,0,0,0};
//Struktur Bitfeld mit 16 Bits
Bitfeld FlagsError1 =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//unionCanBuf CanTransferBuf2, CanTransferBuf3, CanTransferBuf4;
//volatile unionCanBuf IRCanTransferBuf;

volatile uint8_t IR_fl1ms = 0;
volatile uint8_t Param[64];
volatile uint8_t DNCO[256];
uint8_t DncoCopy[256];
volatile uint8_t ParamBiss[20] = {0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0};   //######test##########
//volatile uint8_t IR_CanRx = 0;
volatile uint16_t __attribute__((aligned(4), address(0x1060) )) CanBuffer[128]; 
//volatile uint16_t __attribute__((aligned(4), address(0x2010) )) CanBuffer[128]; 


 /* uint8_t ParamTest[128] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x01, 0x32, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; */
 
/*  volatile uint8_t Param[64]; = { 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00,     0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     //0-7,   //8-15                                
                                 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     //16-23, //24-31                                 
                                 0x00, 0x64, 0x01, 0xF4, 0x02, 0xEE, 0x03, 0xE8,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     //32-39, //40-47
                                 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };   //48-55, //56-63
                              */
                              /*  00, 00, 00, 00, 03, 00, 03, 00,                         01, 00, 00, 00, 00, 00, 00, 00,
                                32, 00, 00, 00, 00, 00, 00, 86,                         00, 00, 00, 00, 00, 00, 00, 00,
                                00, 64, 01, F4, 02, EE, 03, E8,                         00, 00, 00, 00, 00, 00, 00, 00,
                                00, 00, 81, 00, 00, 00, 00, 00,                         00, 00, 00, 00, 00, 00, 00, 00
                              */ 
 

int main(void)
{
    init();
    while(1)           
    {   
        __builtin_clrwdt();         //Watchdog Timeout 64ms
        if(IR_fl1ms == 0xFF)        timing();          
        if(CanRxFifo2 == 1)         getCanBytes();
        if(fl1ms == 1)              UartRxTx();
        if(fl05ms == 1)             analogFrqDetection();
        if(fl1ms == 1)              getFrequenz();
        if(fl1ms == 1)              FrequenzVergleich();    //Von hier je nach modi zu DZ-, Fenster- oder Bereich-ÜW
        if(fl1ms == 1)              Auswertung(); 
        if(fl1ms == 1)              stm_EMK_DrBrErk();      //von hier wird DrBrErkennung() und EMK aufgerufen
        //if(fl1ms == 1)              checkDiffVerst(); 
        if(fl10ms == 1)             ADCauswerten();
        if(fl1ms == 1)              LedsSPI();
        if(fl1ms == 1)              debounceInput4ms();
        if(fl4ms == 1)              debounceInput16ms();
        if(fl4ms == 1)              selectInput();       
        if(fl10ms == 1)             SkrAuswerten();
        if(fl10ms == 1)             ZwHdAuswerten();  
        if(fl100ms == 1)            PortTest();      
        if(fl10ms == 1)             GetErrors();     
        if(fl300ms == 1)            Diagnose();
        if(fl10ms == 1)             RamTest();
        if(fl10ms == 1)             RomTest();              //###test###aktivieren
        if(fl100ms == 1)            LedsAnzeigen();
        if(fl1ms == 1)              triggerCanTX();
        if(fl1ms == 1)              IpkDatenPuffern();
        
        if(fl1ms == 1)   { if(cnt1msRoutinen != 8)   ;}     //###test###ErrorLoop(0xF5); }
        if(fl4ms == 1)   { if(cnt4msRoutinen != 4)   ;}     //###test###ErrorLoop(0xF4); }
        if(fl10ms == 1)  { if(cnt10msRoutinen != 6)  ;}     //###test###ErrorLoop(0xF3); }
        if(fl100ms == 1) { if(cnt100msRoutinen != 3) ;}     //###test###ErrorLoop(0xF2); }
        //Programmlaufzeit Kontrolle
        if(cntTiming > 0) cntTiming --;
        cntTimingMain ++;
        if(cntTimingMain > 0x300)
        {	;  //###test###ErrorLoop(0xF1);
        }
    
        Reset_Cnt_Flags();
    }
    return(0);
}

