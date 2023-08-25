#include <xc.h>
#include "p33CK256MP505.h"
#include "LedAnzeige.h"
#include <stdint.h>
//#include "Transistoren.h"

// edGn1Hz1zu1
// LedGn1HzKurzAn
// LedGn1HzKurzAus
// LedGn2HzKurzAn
// LedGn2HzKurzAus
// _24VKlemme
// _Trans1RK 

const uint8_t tblZiffer0[20] =
	{LedOff,LedGn,LedGn,LedGn,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,
	LedOff, LedGn,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,LedGn,LedGn};
const uint8_t tblZiffer1[20] =
	{LedOff,LedOff,LedOff,LedGn,LedOff,LedOff,LedGn,LedGn,LedOff,LedGn,
	LedOff, LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn};
const uint8_t tblZiffer2[20] =
	{LedOff,LedGn,LedGn,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedOff,
	LedGn, LedOff,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,LedGn,LedGn};
const uint8_t tblZiffer3[20] =
	{LedOff,LedGn,LedGn,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedGn,
	LedGn,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedGn,LedGn,LedGn};
const uint8_t tblZiffer4[20] =
	{LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,
	LedGn,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn};
const uint8_t tblZiffer5[20] =
	{LedOff,LedGn,LedGn,LedGn,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,
	LedGn,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedGn,LedGn,LedGn};
const uint8_t tblZiffer6[20] =
	{LedOff,LedOff,LedGn,LedGn,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,
	LedGn,LedGn,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,LedGn,LedGn};
const uint8_t tblZiffer7[20] =
	{LedOff,LedGn,LedGn,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedOff,
	LedGn,LedOff,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedOff};
const uint8_t tblZiffer8[20] =
	{LedOff,LedGn,LedGn,LedGn,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,
	LedGn,LedGn,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,LedGn,LedGn};
const uint8_t tblZiffer9[20] =
	{LedOff,LedGn,LedGn,LedGn,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,
	LedGn,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedGn,LedGn,LedGn};
const uint8_t tblZifferA[20] =
	{LedOff,LedOff,LedGn,LedOff,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,
	LedGn,LedGn,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn};
const uint8_t tblZifferB[20] =
	{LedOff,LedGn,LedGn,LedOff,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,
	LedGn,LedOff,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,LedGn,LedOff};
const uint8_t tblZifferC[20] =
	{LedOff,LedOff,LedGn,LedGn,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,
	LedOff,LedOff,LedOff,LedGn,LedOff,LedOff,LedOff,LedOff,LedGn,LedGn};
const uint8_t tblZifferD[20] =
	{LedOff,LedGn,LedGn,LedOff,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,
	LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,LedOff,LedGn,LedGn,LedOff};
const uint8_t tblZifferE[20] =
	{LedOff,LedGn,LedGn,LedGn,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,
	LedGn,LedGn,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,LedGn,LedGn};
const uint8_t tblZifferF[20] =
	{LedOff,LedGn,LedGn,LedGn,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,
	LedGn,LedGn,LedOff,LedGn,LedOff,LedOff,LedOff,LedGn,LedOff,LedOff};

uint8_t LedBuffer[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t LedBufferAlt[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void SteckplatzToLedBuffer(uint8_t nummer)
{	uint8_t i;
	const uint8_t *ptrZiffer;
	flLedAlternativ = 1;		//damit holt LedAnsteuern() die LedBytes aus dem altern. buffer
        //Flags2.bit0 = 1;
	cntLedAlternativ = 30;		//30 entspricht 3 Sek.
	ptrZiffer = tblZiffer0;
	switch(nummer)
	{	case 1: ptrZiffer = tblZiffer1; break;
		case 2: ptrZiffer = tblZiffer2; break;
		case 3: ptrZiffer = tblZiffer3; break;
		case 4: ptrZiffer = tblZiffer4; break;
		case 5: ptrZiffer = tblZiffer5; break;
		case 6: ptrZiffer = tblZiffer6; break;
		case 7: ptrZiffer = tblZiffer7; break;
		case 8: ptrZiffer = tblZiffer8; break;
		case 9: ptrZiffer = tblZiffer9; break;
		case 0x0A: ptrZiffer = tblZifferA; break;
		case 0x0B: ptrZiffer = tblZifferB; break;
		case 0x0C: ptrZiffer = tblZifferC; break;
		case 0x0D: ptrZiffer = tblZifferD; break;
		case 0x0E: ptrZiffer = tblZifferE; break;
		case 0x0F: ptrZiffer = tblZifferF; break;
	}
	for(i=0; i<20; i++)
	{	LedBufferAlt[i] = *ptrZiffer;	//ptr zeigt auf Zifferntabelle im Rom
		ptrZiffer ++;
	}
}

void LedsSPI(void)
{
    static uint8_t cntLedBuffer = 0;
	cnt1msRoutinen ++;
	if(cntLedBuffer < 20)
	{	//bei _flLedAlternativ=1 led aus alternativem buffer holen
		if(flLedAlternativ == 1) SPI1BUFL = LedBufferAlt[cntLedBuffer]; // senden
		else					 SPI1BUFL = LedBuffer[cntLedBuffer]; 	// senden
	}
	cntLedBuffer ++;
	if(cntLedBuffer >49) cntLedBuffer = 0;	// von 20 - 99 ist Pause
}

/*-----------------------------------------------------------------------------------------------------------------------------------------
*	LedsAnzeigen(), wird alle 100ms aufgerufen
*	Schreibt die Zustände der Eingänge, SKR, Relais in den LedBuffer[ ].
*-----------------------------------------------------------------------------------------------------------------------------------------*/
void LedsAnzeigen(void)
{   uint8_t i;
    cnt100msRoutinen ++;
    
    //flParamKomplett = 1;   //###test###löschen###
    
    if(flParamKomplett == 0)
    {   for(i=0;i<20;i++)
        {   LedBuffer[i] = LedOff;
        }        
    }
    else
    {
        if(_flErrorPorts == 1)
        {   for(i = 0; i < 8; i++)
            {   LedBuffer[i] = LedGn2HzKurzAn;
            }    
        }
        else
        {	/*Klemme 1-8 aus Can Buffer*/
            if((CanTX[3] & 0x01) == 0 ) LedBuffer[0] = LedOff;
            else                        LedBuffer[0] = LedGn;
            if((CanTX[3] & 0x02) == 0) 	LedBuffer[1] = LedOff;
            else						LedBuffer[1] = LedGn;
            if((CanTX[3] & 0x04) == 0) 	LedBuffer[2] = LedOff;
            else						LedBuffer[2] = LedGn;
            if((CanTX[3] & 0x08) == 0) 	LedBuffer[3] = LedOff;
            else						LedBuffer[3] = LedGn;
            /*
            if((CanTX[3] & 0x10) == 0 ) LedBuffer[4] = LedOff;
            else						LedBuffer[4] = LedGn;
            if((CanTX[3] & 0x20) == 0) 	LedBuffer[5] = LedOff;
            else						LedBuffer[5] = LedGn;
            if((CanTX[3] & 0x40) == 0) 	LedBuffer[6] = LedOff;
            else						LedBuffer[6] = LedGn;
            if((CanTX[3] & 0x80) == 0) 	LedBuffer[7] = LedOff;
            else						LedBuffer[7] = LedGn;
            */
        }

        //Q1
        if((CanTX[1] & 0x01) == 0)  LedBuffer[8] = LedOff;
        else                        LedBuffer[8] = LedGn;
        //Q2
        if((CanTX[1] & 0x02) == 0)  LedBuffer[9] = LedOff;
        else                        LedBuffer[9] = LedGn;
        //Q3
        if((CanTX[1] & 0x04) == 0)  LedBuffer[10] = LedOff;
        else                        LedBuffer[10] = LedGn;
        //Q4
        if((CanTX[1] & 0x08) == 0)  LedBuffer[11] = LedOff;
        else                        LedBuffer[11] = LedGn;
        /*
        if((CanTX[1] & 0x04) == 0)  
        {   if(flHim1 == 1)
            {   LedBuffer[10] = LedGn1Hz1zu1;
            }
            else
            {   LedBuffer[10] = LedOff;       
            }
        }
        else //Can bit on
        {   if((Param[10] & 0x10) == 0)
            {   LedBuffer[10] = LedGn;
            }
            else
            {   //Mute mit deaktivierter Sensorik
                LedBuffer[10] = LedGn1HzKurzAus;
            }
        }
        //Dz2
        if((CanTX[1] & 0x20) == 0)  
        {   if(flHim2 == 1)
            {   LedBuffer[11] = LedGn1Hz1zu1;
            }
            else
            {   LedBuffer[11] = LedOff;       
            }
        }
        else //Can bit on
        {   if((Param[11] & 0x10) == 0)
            {   LedBuffer[11] = LedGn;
            }
            else
            {   //Mute mit deaktivierter Sensorik
                LedBuffer[11] = LedGn1HzKurzAus;
            }
        }
        */
    }
}
/*----------Ende LedsAnzeigen()-------------------------------------------------------------------------------------------------*/
