#include <xc.h>
#include "p33CK256MP505.h"
#include "PortTest.h"
#include "Eingaenge.h"
#include <stdint.h>

uint8_t pterror = 0, flPortsErr = 0, cntPortTestErr = 0;

void PortTest(void)
{   uint8_t read1 = 0, read2 = 0;
    static uint8_t stmPortTest = 0;	
	cnt100msRoutinen ++;
	
	read1 = PortsEinlesen();
	switch(stmPortTest)
	{	case 0:	
			if((read1 & 0x01) != 0)
			{	//Pin==1
                LATCbits.LATC13 = 0;
				TRISCbits.TRISC13 = 0;	//Pin wird auf 0 gezogen
				wait();
				read2 = PortsEinlesen();
				if(((read1 ^ read2) & 0xFE) != 0) pterror = 1;
				TRISCbits.TRISC13 = 1;
			}
			break;
		case 1:
			if((read1 & 0x02) != 0)
			{	//Pin==1
				LATCbits.LATC12 = 0;
				TRISCbits.TRISC12 = 0;	//Pin wird auf 0 gezogen
				wait();
				read2 = PortsEinlesen();
				if(((read1 ^ read2) & 0xFD) != 0) pterror = 1;
				TRISCbits.TRISC12 = 1;
			}
			break;
		case 2:
			if((read1 & 0x04) != 0)
			{	//Pin==1
				LATBbits.LATB14 = 0;
				TRISBbits.TRISB14 = 0;	//Pin wird auf 0 gezogen
				wait();
				read2 = PortsEinlesen();
				if(((read1 ^ read2) & 0xFB) != 0) pterror = 1;
				TRISBbits.TRISB14 = 1;
			}
			break;
		case 3:	
			if((read1 & 0x08) != 0)
			{	//Pin==1
				LATBbits.LATB15 = 0;
				TRISBbits.TRISB15 = 0;	//Pin wird auf 0 gezogen
				wait();
				read2 = PortsEinlesen();
				if(((read1 ^ read2) & 0xF7) != 0) pterror = 1;
				TRISBbits.TRISB15 = 1;
			}
			break;
		/* case 4:
			if((read1 & 0x10) != 0)
			{	//Pin==1
				LATEbits.LATE4 = 0;
				TRISEbits.TRISE4 = 0;	//Pin wird auf 0 gezogen
				wait();
				read2 = PortsEinlesen();
				if(((read1 ^ read2) & 0xEF) != 0) pterror = 1;
				TRISEbits.TRISE4 = 1;
			}
			break;
		case 5:
			if((read1 & 0x20) != 0)
			{	//Pin==1
				LATEbits.LATE5 = 0;
				TRISEbits.TRISE5 = 0; 	//Pin wird auf 0 gezogen
				wait();
				read2 = PortsEinlesen();
				if(((read1 ^ read2) & 0xDF) != 0) pterror = 1;
				TRISEbits.TRISE5 = 1;
			}
			break;
		case 6:
			if((read1 & 0x40) != 0)
			{	//Pin==1
				LATEbits.LATE6 = 0;
				TRISEbits.TRISE6 = 0; 	//Pin wird auf 0 gezogen
				wait();
				read2 = PortsEinlesen();
				if(((read1 ^ read2) & 0xBF) != 0) pterror = 1;
				TRISEbits.TRISE6 = 1;
			}
			break;
		case 7:
			if((read1 & 0x80) != 0)
			{	//Pin==1
				LATEbits.LATE7 = 0;
				TRISEbits.TRISE7 = 0; 	//Pin wird auf 0 gezogen
				wait();
				read2 = PortsEinlesen();
				if(((read1 ^ read2) & 0x7F) != 0) pterror = 1;
				TRISEbits.TRISE7 = 1;
			}
			break;	*/	
	}
	if(stmPortTest == 3)
	{	//ein Durchlauf ist komplett, error?
		if(pterror == 0)							// wenn kein error
		{	cntPortTestErr = 0;
		}
		else
		{	//pterror == 1
			cntPortTestErr ++; 	
			pterror = 0;
		}		
		if(cntPortTestErr > 10)                     // Auswertung
		{	flPortsErr = 1;                         // error flag setzen
			cntPortTestErr = 11;					// verhindert Überlauf
		}
	}
	stmPortTest ++;
	if(stmPortTest > 3) stmPortTest = 0;
    
    if(flParamKomplett == 0)
    {   cntPortTestErr = 0;
    }
}   //Ende PortTest() ----------------------------------------------------------


void wait(void)
{   uint8_t i;
    for(i=0;i<20;i++)
    {   __builtin_nop();
    }
}