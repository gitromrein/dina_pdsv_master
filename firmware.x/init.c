#include <xc.h>
#include "p33CK256MP505.h"
#include "init.h"
#include "CCPconfig.h"
#include "getSteckplatz.h"
//#include "ErrorLoop.h"
#include "errorloop.h"
#include "LedAnzeige.h"
#include "Uart.h"
#include "Can.h"
#include "SpiConfig.h"
#include "timing.h"
#include "Eingaenge.h"
#include "Frequenz.h"
#include "ADC.h"
#include "RamTest.h"
#include "RomTest.h"
#include "CRC16.h"
#include <stdint.h>

void init(void)
{
    uint16_t i = 0;
    
    INTCON2bits.GIE = 0;        //Interrupts disabled
    
    //RCON speichert Ursache eines Resets
    //bit15 TRAPR: Trap Reset Flag
    //bit14 IOPUWR: Illegal Opcode
    //bit13 --
    //bit12 --
    //bit11 --
    //bit10 --
    //bit9 CM: Configuration Mismatch
    //bit8 VREGS: Voltage Regulator Standby During Sleep
    //bit7 EXTR: External Reset (MCLR)
    //bit6 SWR: Software RESET
    //bit5 Reserved:
    //bit4 WDTO: Watchdog Timer Time-out
    //bit3 SLEEP: Wake-up from Sleep
    //bit2 IDLE: Wake-up from Idle
    //bit1 BOR: Brown-out Reset
    //bit0 POR: Power-on Reset
/*    
    if(RCONbits.BOR == 1)    ErrorLoop(0xFF); 
    if(RCONbits.IDLE == 1)   ErrorLoop(0xFE);
    if(RCONbits.SLEEP == 1)  ErrorLoop(0xFD);
    if(RCONbits.WDTO == 1)   ErrorLoop(0xFC);
    if(RCONbits.SWR == 1)    ErrorLoop(0xFB);
    if(RCONbits.EXTR == 1)   ErrorLoop(0xFA);
    if(RCONbits.VREGS == 1)  ErrorLoop(0xF9);
    if(RCONbits.CM == 1)     ErrorLoop(0xF8);
    if(RCONbits.IOPUWR == 1) ErrorLoop(0xF7);
    if(RCONbits.TRAPR == 1)  ErrorLoop(0xF6);
*/    

    //40 MIPS with 10MHz crystal
    CLKDIVbits.FRCDIV = 0;
    CLKDIVbits.PLLPRE = 1;          //N1 = 1, 10MHz
    PLLFBDbits.PLLFBDIV = 64;       //M = 65, VCO = 640MHz
    PLLDIVbits.POST1DIV = 2;        //N2 = 2, 320MHz
    PLLDIVbits.POST2DIV = 2;        //N3 = 2, 160MHz, Fosc = 80MHz, Fosc/2 = Fcy = 40MHz (40 MIPS)               
    
    //Erratasheet: Der Teiler von FVCO ist nicht zuverlässig. Als CAN Taktquelle 
    CANCLKCONbits.CANCLKSEL = 2;    //2 = FPLLO = 160
    CANCLKCONbits.CANCLKDIV = 1;    //1 = Teiler 2. CAN CLK = 160MHz/(1+1) = 80MHz (mögliche Frequenzen sind: 80, 40, 20, 10 MHz)
    CANCLKCONbits.CANCLKEN = 1;     //enable
    
    __builtin_write_OSCCONH((uint8_t)(0x03));
    __builtin_write_OSCCONL((uint8_t)(0x01));
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    while (OSCCONbits.LOCK != 1);
    
    //Rererenzclock als 1,25MHz Quelle für CCP1
    REFOCONLbits.ROEN = 1;
    REFOCONLbits.ROSEL = 1;     //Fp = 40MHz
    REFOCONHbits.RODIV = 16;    //Devider: 2*16=32. 40MHz/32=1,25MHz. (16 entspricht 1,25MHz. 8 entspricht 2,5MHz)
    REFOCONLbits.ROSWEN = 1;    //Devider change request
    while(REFOCONLbits.ROSWEN == 1) {;} //0 = Clock divider change has completed
    
    REFOCONLbits.ROOUT = 0;     //Out auf Pin, 1 = enabled für Testzwecke
    //REFCLK auf RB2, RP34, Pin25 schalten:
    //RPOR1bits.RP34R = 14;
    //TRISBbits.TRISB2 = 0;
    //REFCLK auf RD13, RP77, Pin6 schalten:
    //RPOR22bits.RP77R = 14;    
    //TRISDbits.TRISD13 = 0;
    
    //Peripheral Module Disable Resgister. 1 = deaktivieren
    PMD1 = 0xF7D4;  //ADC, CAN, SPI1, UART1, Timer1 sind jeweils 0 = aktiv
    PMD2 = 0xFFFE;  //CCP1 = 0 = aktiv
    PMD3 = 0xFF7F;  //CRC  = 0 = aktiv
    PMD4 = 0xFFF7;  //RefClk
    //PMD5 ist bei dem Baustein nicht vorhanden
    PMD6 = 0xFFFF;
    PMD7 = 0xFFFF;
    PMD8 = 0xFFFF;
    
    ANSELA = 0x1C;   //AN3, AN4, AN9
    ANSELB = 0;
    ANSELC = 0x47;   //AN12, AN13, AN14, AN17
    ANSELD = 0;

    /*Portbelegung:
    RA0 = frei              OUT
    RA1 = frei              OUT
    RA2 = AN9 EMK Spannung  IN analog
    RA3 = AN3 L3 Integr.Sp. IN analog
    RA4 = AN4 1,65V         IN analog
    
    RB0 = OSC1              IN
    RB1 = OSC0              IN
    RB2 = frei              OUT
    RB3 = CAN TX            OUT
    RB4 = CAN RX            IN
    RB5 = PGD3              OUT
    RB6 = PGC3              OUT
    RB7 = SDI1              IN
    
    RB8 = SDO1              OUT
    RB9 = SCK1              OUT
    RB10 = Hysterese1       OUT
    RB11 = Hysterese2       OUT
    RB12 = C-Kopplung       OUT //entfällt###test###
    RB13 = EMK-Verstärkung  OUT
    RB14 = IN3              IN
    RB15 = IN4              IN
    
    RC0 = AN12 = Erw.DrBr   IN analog   Erweiterte Drahtbrucherkennung, Differenzsp. gleichgerichtet
    RC1 = AN13 = SchSp.DrBr IN analog   Schaltspannung 6,8V Drahtbruchspannung 2,9Hz
    RC2 = AN14 = 12V-Test   IN analog
    RC3 = frei              OUT  
    RC4 = SPI1-CS           OUT
    RC5 = frei              OUT
    RC6 = AN17 = 3V3-Test   IN analog
    RC7 = Adresse-L         IN
    RC8 = frei              OUT
    RC9 = SerTX             OUT
    RC10 = PulsL1, QD-A     IN
    RC11 = PulsL3,QD-B,CCP1 IN
    RC12 = IN2              IN
    RC13 = IN1              IN
    
    RD1 = Drahtbruch        OUT
    RD8 = SerRX             IN
    RD10 = Adresse-R        OUT
    RD13 = frei             OUT
    */
    ptrRamZelle = (uint16_t *)kBeginRamTest;
	ptrRam = (uint16_t *)(kBeginRamTest+2);
	idxPattern = 0;
    StartUpRamTest();
	ptrRamZelle = (uint16_t *)kBeginRamTest;
	ptrRam = (uint16_t *)(kBeginRamTest+2);
	idxPattern = 0;
    
    AdrRom = 0;
    AdrRomCopy = kAdrRomCopy; 
    for(i=0; i < 0x8000; i +=2)
	{	RomTest();
	}
    AdrRom = 0;
    AdrRomCopy = kAdrRomCopy; 
    
    LATA = 0;
    LATB = 0x308;   //CANTX, SDO1, SCK1 jeweils 1 um Einschaltglitches zu vermeiden
    LATC = 0x210;   //SPI1-CS, SerTX jeweils 1 um Einschaltglitches zu vermeiden
    LATD = 0x02;    //LATD1 ist high, die DrBrSpannung ist low durch Transitor Invertierung
   
    //Ports als IN=1 / OUT=0 konfigurieren           
    TRISA = 0b11100; 
    TRISB = 0b1100000010010011; 
    TRISC = 0b11110011000110;
    TRISD = 0b00000100000000;
    
    ADC_Config();

    //Peripheral Pin Select
    //SPI1: SDI=Pin35, SDO=Pin36, SCK=Pin37, CS=Pin38=RC4
    RPINR20bits.SDI1R = 39;     //SDI1 RP39
    RPOR4bits.RP40R = 5;        //SDO1 RP40
    RPOR4bits.RP41R = 6;        //SCK1 RP41
    RPOR10bits.RP52R = 7;       //SS1  RP52

    //UART1, TX=Pin29, RX=Pin30
    RPINR18bits.U1RXR = 72;     //SerRX RP72
    RPOR12bits.RP57R = 1;       //SerTX RP57

    //CAN1, TX=Pin26, RX=Pin27
    RPINR26bits.CAN1RXR = 36;   //CAN RX RP36
    RPOR1bits.RP35R = 21;       //CAN TX RP35
    
    //Quadraturdecoder IN, CCP In
    RPINR14bits.QEIA1R = 58;    //QEI-A RP58, RC10
    RPINR14bits.QEIB1R = 59;    //QEI-B RP59, RC11, intern parallel mit ICM1
    RPINR3bits.ICM1R = 59;      //ICM1(CCP1) RP59, RC11, intern parallel mit QEI-B
    
    for(i = 0; i < 64; i++)
    {   Param[i] = 0;
    }
    for(i = 0; i < 256; i++)
    {   DNCO[i] = 0; 
        DncoCopy[i] = 0;
    }
    cntLedAlternativ = 0;
    
     //Timer1 konfig., Prescaler=0. Periode=1ms bei 40MHz
    T1CON = 0;
    TMR1 = 0;
    PR1 = 5000;                //5000 bei 5MHz entspricht 1ms
    IPC0bits.T1IP = 6;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 0;
    T1CONbits.TECS = 1;        //TMR1 clock Fcy = Fp
    T1CONbits.TCKPS = 1;       //1: Prescaler = 8 = 40MHz/8 = 5MHz
    T1CONbits.TON = 0;
    
    //########test############
/*    for(i=0; i<64;i++)
    {
        PS.Bytes[i] = ParamTest[i];
    }
*/
/*    writeParamStruct();
    while(1)
    {   
        getCRC16(&PS, 128);
    }
    //########test############
*/
    
    Steckplatz = getSteckplatz();
    SteckplatzToLedBuffer(Steckplatz);
    UartConfig();
    
    //Timer1 konfig., Prescaler=8. Periode=1ms bei 40MHz
    T1CON = 0;
    TMR1 = 0;
    PR1 = 5000;                //5000 entspricht 1ms  
    IPC0bits.T1IP = 6;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 0;
    T1CONbits.TECS = 1;        //TMR1 clock Fcy = Fp
    T1CONbits.TCKPS = 1;       //1: Prescaler = 8 = 40MHz/8 = 5MHz
    T1CONbits.TON = 1;
    
    SteckplatzAustauschen(Steckplatz);
    
    T1CONbits.TON = 0;
    
    CanConfig();
    SpiConfig(); 
    
    //Parameter Struktur init
    for(i = 0; i < SizePS; i++)
    {   PS.Bytes[i] = 0;
    }
    //IPK Struktur init
    for(i = 0; i < ProtokollUart; i++)
    {   LatchRX.Bytes[i] = 0;
    }
    for(i = 0; i < ProtokollUart; i++)
    {   LatchTX.Bytes[i] = 0;
    }
    for(i = 0; i < ProtokollUart; i++)
    {   RX.Bytes[i] = 0;
    }
    for(i = 0; i < ProtokollUart; i++)
    {   TX.Bytes[i] = 0;
    }
    TX.Bytes[IPK_FW] =  FW_Version;
    TX.Bytes[IPK_End] = ProtokollEnde;
    
    //RCONbits.SWDTEN = 0;       //Watchdog disabled
    CORCONbits.VAR = 0;          //!!!!!Achtung: CORCONbits.VAR = 1 verursacht Adddress Error!!!!!
    INTCON1 = 0;
    INTCON1bits.NSTDIS = 1;      //1: Interrupt nesting disabled. 0 = enabled.
    INTCON2 = 0;
    INTCON3 = 0;
    INTCON4 = 0;
   
    T1CON = 0;
    TMR1 = 0;
    PR1 = 2500;                //2500 entspricht 0,5ms  
    IPC0bits.T1IP = 6;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TECS = 1;        //TMR1 clock Fcy = Fp
    T1CONbits.TCKPS = 1;       //1: Prescaler = 8 = 40MHz/8 = 5MHz
    T1CONbits.TON = 1;
    
    initFreqMessung();
    CCPconfig();
    
    //Lock Peripheral Remapping Configuration Registers
    //###test###__builtin_write_RPCON(0x0800);
    
    cntTimingMain = 0;
    cntTiming = 0;
    
    //###test###WDTCONLbits.ON = 1;         //startet den WatchdogTimer. Die Prescaler sind in der Configuration festgelegt.
    
    INTCON2bits.GIE = 1;         //Interrupts enabled       
}

