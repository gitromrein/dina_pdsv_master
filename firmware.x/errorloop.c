#include <xc.h>
#include "p33CK256MP505.h"
#include "errorloop.h"
#include <stdint.h>
//#include "Transistoren.h"


//------------------------------------------------------------------------------------------------------------------------------------------
//	ErrorLoop() ist eine Endlosschleife und wird nach einem schweren Fehler aufgerufen.
//	Alle 100ms sendet die IPK den der Funktion übergebenen Fehlercode.
//	Dadurch kann die Fehlerursache mit einem PC-Terminalprogramm festgestellt werden.
//------------------------------------------------------------------------------------------------------------------------------------------
void ErrorLoop(uint8_t Ursache)
{	uint16_t i; 
    __builtin_disi(50);
    INTCON2bits.GIE = 0; // IR's gesperrt
    
    TRISBbits.TRISB1 = 0;       //Toggle Transistor2 OUT
    // init Serielle Schnittstelle---------------
    
    //UART1, TX=Pin29, RX=Pin30 
    RPINR18bits.U1RXR = 72;     //SerRX RP72
    RPOR12bits.RP57R = 1;       //SerTX RP57
    LATCbits.LATC9 = 1;         //SerTX
    TRISCbits.TRISC9 = 0;
    LATDbits.LATD8 = 0;         //SerRX
    TRISDbits.TRISD8 = 1;
    
    //IEC0bits.U1TXIE = 1;      //IR enable   
    U1MODEHbits.BCLKSEL = 2;    //Fosc = 100MHz
    U1MODEHbits.BCLKMOD = 0;    //0: Legacy Mode. 1: Fractional Baudrate Generation.
    U1MODEbits.BRGH = 0;        //Baudrate: 0=clock/16. 1=clock/4
    U1BRG = 47;                 //130,2kBaud
    U1BRGH = 0;   
    U1MODEbits.MOD = 4;         //9Bit Modus (mit address detect?###test###)
    //U1P1 = 0x55;              //TX Adresse im Address Mode. Notwendig für das 9. Bit
    //Wenn U1P1 beschrieben wird und dann ein Wert in TXBuf geschrieben wird, dann wird U1P1 mit gesetztem 9. Bit gesendet.
    //Demnach könnte in U1P1 statt einer festen Adresse irgendwelche Daten geschrieben werden!!!!!!
    U1P2 = 0xAA;                //RX Adresse im Address Mode. Notwendig für das 9. Bit
    U1P3 = 0x00;                //RX Adress Maske. Bei 0x00 wird jede Adresse akzeptiert
    U1MODEbits.UARTEN = 1;      //enabled
    U1MODEbits.URXEN = 1;       //RX enabled
    U1MODEbits.UTXEN = 1;       //TX enabled
    U1STA = 0;
    IFS0bits.U1TXIF = 0;        //damit nach aktivieren der UART kein Interrupt ausgelöst wird
    
	while(1)
	{	
		while(U1STAHbits.UTXBE == 0) { ; }      //Warten bis Sendepuffer leer ist.
		U1TXREG = (uint16_t)Ursache;
		//if(_Transistor2 == 0) _Transistor2 = 1;	// freier Pin toggelt, für Messzwecke
        //else                  _Transistor2 = 0;
		for(i = 1; i < 0xFFFF;  i ++)                // warteschleife 10ms
		{	 __builtin_clrwdt();
		}
	}
}
//----Ende ErrorLoop()-------------------------------------------------------------------------------------------------------------
