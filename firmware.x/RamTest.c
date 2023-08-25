#include <xc.h>
#include "p33CK256MP505.h"
#include "RamTest.h"
#include "errorloop.h"
#include "Uart.h"
#include <stdint.h>

const uint16_t constTestPattern[32] = {0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
                                       0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000,
									   0xFFFE,0xFFFD,0xFFFB,0xFFF7,0xFFEF,0xFFDF,0xFFBF,0xFF7F,
                                       0xFEFF,0xFDFF,0xFBFF,0xF7FF,0xEFFF,0xDFFF,0xBFFF,0x7FFF};
const uint16_t constInitPattern[32] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
                                       0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
                                       0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
									   0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};

//###########################test################################
//Den Variablen, die beim Ramtest verwendet werden, 
//wrden mit __attribute__((adress(0x....))) definierte Adressen zugeteilt,
//die außerhalb des Ramtests sind.
/*
uint16_t RamZelleBackup2 = 0;
uint16_t RamBackup2 = 0;
uint16_t idxPattern2 = 0;
uint16_t RamZelleBackupINV2 = 0;
uint16_t RamBackupINV2 = 0;
uint16_t *ptrRamZelle2;
uint16_t *ptrRam2;
*/

uint16_t __attribute__((address(0x1050))) RamZelleBackup = 0;
uint16_t __attribute__((address(0x1052))) RamBackup = 0;
uint16_t __attribute__((address(0x1054))) idxPattern = 0;
uint16_t __attribute__((address(0x1056))) RamZelleBackupINV = 0;
uint16_t __attribute__((address(0x1058))) RamBackupINV = 0;
uint16_t __attribute__((address(0x105A))) *ptrRamZelle;
uint16_t __attribute__((address(0x105C))) *ptrRam;
uint16_t __attribute__((address(0x105E))) dummy;
/*
//###################test####################################
uint16_t __attribute__((address(0x2000))) RamZelleBackup = 0;
uint16_t __attribute__((address(0x2002))) RamBackup = 0;
uint16_t __attribute__((address(0x2004))) idxPattern = 0;
uint16_t __attribute__((address(0x2006))) RamZelleBackupINV = 0;
uint16_t __attribute__((address(0x2008))) RamBackupINV = 0;
uint16_t __attribute__((address(0x200A))) *ptrRamZelle;
uint16_t __attribute__((address(0x200C))) *ptrRam;
uint16_t __attribute__((address(0x200E))) dummy;
*/
//------------------------------------------------------------------------------------------------------------------------------------------
//	StartUpRamTest() wird 1x nach Reset aufgerufen, prüft den kompletten verwendeten Ram.
//	Ein umlaufendes Bit wird gegen 0x00 und 0xFF in 16 Schritten geprüft.
//	Adressierungsfehler werden nicht erkannt.
//------------------------------------------------------------------------------------------------------------------------------------------

void StartUpRamTest(void)	
{	
	for(ptrRamZelle = (uint16_t*)kBeginRamTest; ptrRamZelle < (uint16_t*)kEndRamTest; ptrRamZelle ++)
	{	
		RamZelleBackup =  *ptrRamZelle;
		//hier das inverierte word ablegen
		RamZelleBackupINV =  ~(*ptrRamZelle);
		for(idxPattern = 0; idxPattern <32; idxPattern ++)
		{	*ptrRamZelle = constTestPattern[idxPattern];
			if(*ptrRamZelle != constTestPattern[idxPattern])
			{	ErrorLoop(0xEF);
            
            }
		}
		//wenn backup und inveriertes backup plausibel, dann zurückschreiben
		if(RamZelleBackup == (~RamZelleBackupINV))
		{	*ptrRamZelle = RamZelleBackup; 
		}
		else	ErrorLoop(0xEF);
	}  
     
}
//-----Ende StartUpRamTest()---------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------------
//	RamTest() wird alle 10ms aufgerufen, ein Aufruf dauert 3µS.
//	Ram von 0x1010 bis 0x1D00 = 1552d 16Bit-Integer
//	Anzahl der Aufrufe bis Speichertest komplett: 1552 x 1552byte x 32pattern x10ms, Dauer 214 Std. = 9 Tage
//	Ein umlaufendes Bit wird gegen 0x0000 und 0xFFFF in 32 Schritten geprüft.
//	Adressierungsfehler werden auch erkannt.
//------------------------------------------------------------------------------------------------------------------------------------------
void RamTest(void)
{	__builtin_disi(50);
   	INTCON2bits.GIE = 0; // IR's gesperrt
	cnt10msRoutinen ++;				// SW-RQ 607
	if(ptrRamZelle < (uint16_t*)kEndRamTest)	// äußere Schleife, Überlauf von 0x1000 bis < 0x3000
	{	
		if(ptrRam != ptrRamZelle)		// innere Schleife
		{	
			if(idxPattern < 32)	//	for(idxPattern=0; idxPattern <32; idxPattern ++)	// Bit Schleife
			{	
				RamZelleBackup =  *ptrRamZelle;
				//hier das inverierte byte ablegen
				RamZelleBackupINV =  ~(*ptrRamZelle);
				RamBackup =  *ptrRam;
				//hier das inverierte byte ablegen
				RamBackupINV =  ~(*ptrRam);
				*ptrRam = constInitPattern[idxPattern];
				*ptrRamZelle = constTestPattern[idxPattern];                
				if(*ptrRam != constInitPattern[idxPattern])	
				{	ErrorLoop(0xEE);
				}
				if(*ptrRamZelle != constTestPattern[idxPattern])
				{	ErrorLoop(0xEE);
				}
	
				//wenn backup und inveriertes backup plausibel, dann zurückschreiben
				if(RamBackup == (~RamBackupINV)	)
			      	{	*ptrRam = RamBackup;	   }
				else	ErrorLoop(0xEE);
				if(RamZelleBackup == (~RamZelleBackupINV))
				{	*ptrRamZelle = RamZelleBackup;   }
				else	ErrorLoop(0xEE);

				idxPattern ++;
			}
			else	// idxPattern == 32
			{	idxPattern = 0; 
				if(ptrRam < (uint16_t*)(kEndRamTest-2))
				{	ptrRam ++;
				}
				else
				{	ptrRam = (uint16_t*)kBeginRamTest;
				}
			}
		}
		else	// ptrRam == ptrRamZelle, ptrRam lief 1x durch
		{	ptrRamZelle ++;	
			if(ptrRamZelle < (uint16_t*)(kEndRamTest-2))
			{	ptrRam = ptrRamZelle;
                ptrRam ++;
			}
			else
			{	ptrRam = (uint16_t*)kBeginRamTest;	
			}	
		}	
	}		
	else
	{	ptrRamZelle = (uint16_t*)kBeginRamTest;	
		ptrRam = (uint16_t*)(kBeginRamTest+2);
	}
    __builtin_disi(0);
	INTCON2bits.GIE = 1; // IR's freigegeben
    
} //----Ende RamTest()----------------------------------------------------------

