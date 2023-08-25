#include <xc.h>
#include "p33CK256MP505.h"
#include "RomTest.h"
#include "errorloop.h"
#include <stdint.h>

//uint16_t OffsetRom = 0;
uint16_t AdrRom = 0;
uint16_t AdrRomCopy = kAdrRomCopy;

//------------------------------------------------------------------------------------------------------------------------------------------
//	RomTest() wird alle 10ms aufgerufen.
//	65KByte Rom werden mit einer Kopie verglichen.
//	Bei einem Fehler wird die Endlosschleife ErrorLoop() aufgerufen.
//------------------------------------------------------------------------------------------------------------------------------------------
void RomTest(void)
{	//TBLPAG = 0: Rom Adress Bereich 0x00000 bis 0x0FFFF. Hier liegt das Programm
    //TBLPAG = 1: Rom Adress Bereich 0x10000 bis 0x1FFFF. Hier liegt die Kopie für den RomTest
    //OffsetRom von 0x0000 bis 0xFFFE, dann wieder 0x0000
    uint16_t RomLow, RomHigh, RomCopyLow, RomCopyHigh;
    
    cnt10msRoutinen++;
    
    //__builtin_disi(50);
    //INTCON2bits.GIE = 0; // IR's gesperrt ###test#### Notwendig? Nicht notwendig ############
    TBLPAG = 0;
    RomLow = __builtin_tblrdl(AdrRom);
    RomHigh = __builtin_tblrdh(AdrRom);
    RomCopyLow = __builtin_tblrdl(AdrRomCopy);
    RomCopyHigh = __builtin_tblrdh(AdrRomCopy);
    if((RomLow != RomCopyLow)||(RomHigh != RomCopyHigh))
    {   ;   //###test###ErrorLoop(0xED);  ##########################################
    }
    else
    {   //Nächste Rom Adresse in 2er Schritten
        AdrRom += 2;
        AdrRomCopy += 2;
        if(AdrRom > 0x7FFE)
        {   AdrRom = 0;
            AdrRomCopy = kAdrRomCopy;
        }
    }
    //__builtin_disi(0);
    //INTCON2bits.GIE = 1; // IR's freigeben
}
//----Ende RomTest()---------------------------------------------------------------------------------------------------------------
