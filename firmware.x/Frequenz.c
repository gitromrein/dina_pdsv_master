#include <xc.h>
#include "p33CK256MP505.h"
#include "Frequenz.h"
#include "Uart.h"
#include "CCPconfig.h"
#include <stdint.h>
  	
uint16_t cntTimeOut0 = 0;           //für Limit0
uint16_t cntTimeOut1 = 25;			//für Limit1
uint16_t cntTimeOut2 = 25;			//für Limit2
uint16_t cntTimeOut3 = 25;			//für Limit3
uint8_t cntTimeOut250ms = 0; 
uint8_t cntTimeOut1S = 0;
uint16_t cntTimeOut3S = 25;
uint16_t cntTimeOutMessung = 0;		//für max. Periodendauer, unterer Grenzwert der Frequenz (0,3Hz).
uint16_t latchTimeOut1 = 0;         //Latchfunktion: Bei Timeout1 wird aus diesem Wert die Periode zurückgerechnet
uint8_t cntAbschaltverz1 = 0, cntAbschaltverz2 = 0;
uint8_t cnt1500Hz = 0, fl1500Hz = 0;	
uint8_t flValidCapture = 0;
uint8_t NeueMessung = 0;
uint8_t BereichIstFrqAlt = 6, BereichIstFrq = 6;
uint8_t stmSTHyst = FrqBereich_6;	//kleinste Hysterese
uint8_t StatusSTHyst = Hyst_1V88;			//Status der Hystereseansteuerung
uint8_t Betriebsarten = 0, flPositionsUW1 = 0;
//uint8_t BA_St = 0, BA_F1 = 0, BA_F2 = 0, BA_F3 = 0, BA_MT = 0; 
uint8_t StatusLimit0 = 0, StatusLimit1 = 0, StatusLimit2 = 0, StatusLimit3 = 0;
uint16_t FrqLimit0 = 0, FrqLimit1 = 0, FrqLimit2 = 0, FrqLimit3 = 0; 
uint16_t FrqLimHyst0 = 0, FrqLimHyst1 = 0, FrqLimHyst2 = 0, FrqLimHyst3 = 0; 
uint16_t IstFrqX10 = 0;
uint8_t Validierung = 0;
ParamStruct PS, Param_copy;
uint8_t stmQuitt = 0;
uint8_t flQuitt = 0;
uint16_t cntQuitt = 0;
uint16_t cntZeitLimit = 0;
uint8_t RichtungStatus = 0, cntErrRichtungsWechsel = 0, flErrRichtungsWechsel = 0;
uint8_t flNeueFlanke = 0;

//------------------------------------------------------------------------------
//getFrequenz() wird jede 1ms aufgerufen
//Die 10fache Frequenz =  1.250.000 x 10 / Periode
//4 Promille mehr: 12.500.000 x 1,004 = 12.550.000
//Was ist besser? Die Periode in 10xFrequenz umzurechnen, 
//oder die VergleichsIstFrqX10 in Vergleichs-Periode umzurechnen?
//Bei FrqX100: 0,500Hz -> 49     0,502Hz -> 50
//Bei FrqX10:  0,500Hz -> 4      0,502Hz -> 5    
//Fazit: Gleiche Genauigkeit, aber die letzte Stelle erscheint nicht.
void getFrequenz(void)
{   uint16_t notiz = 0;

	cnt1msRoutinen ++;
   
	__builtin_disi(50);
   	INTCON2bits.GIE = 0; // IR's gesperrt
	Periode = Periode_IR;
    Richtung = Richtung_IR;
	flNewCapture = flNewCapture_IR;	
	flNewCapture_IR = 0;
	__builtin_disi(0);
	INTCON2bits.GIE = 1; // IR's freigegeben
    
    //Validierung = (TX.bf.Val & RX.bf.Val);    //###test###
    Validierung = (TX.bf.Val);

    /*
	if(fl10ms == 1)
	{	;
		//Countdown Abschaltverzögerung###test############################################
		//if(cntAbschaltverz1 > 0) cntAbschaltverz1 --;		//Abschaltverzögerung läuft ab
		//if(cntAbschaltverz2 > 0) cntAbschaltverz2 --;		//Abschaltverzögerung läuft ab
	}
    */
		
	if(flNewCapture == 1)
	{	cntTimeOut0 = 0;
		//latchTimeOut1 0;			//für Latchfunktion in Auswertung()
		cntTimeOut1 = 0;
        cntTimeOut2 = 0;
        cntTimeOut3 = 0;
		cntTimeOut1S = 100;                     //entspricht 1S
		TX.bf.TO1s = 0;                         //IPK 1S TimeOut Flag löschen
		cntTimeOutMessung = kTOMessung;         //entspricht 4Sek	
		cntTimeOut250ms = 25;					//entspricht 250ms
		cntTimeOut3S = 330;                     //entspricht 3,3Sek
        flNeueFlanke = 1;
	}
	else
	{	if(fl10ms == 1)
		{
			if(cntTimeOut0 < 0xFFFF) cntTimeOut0 ++;
			if(cntTimeOut1 < 0xFFFF) cntTimeOut1 ++;
            if(cntTimeOut2 < 0xFFFF) cntTimeOut2 ++;
            if(cntTimeOut3 < 0xFFFF) cntTimeOut3 ++;
			if(cntTimeOut1S > 0) cntTimeOut1S --;		//bei abgelaufenem TimeOut wird der Frq-Vergleich gesperrt.
			else 
            {   TX.bf.TO1s = 1;                        //IPK 1S TimeOut Flag
                RichtungStatus = 0xFF;
                TX.bf.Richtung = 1;
            }
			if(cntTimeOutMessung > 0) cntTimeOutMessung --;
			else
			{	Periode = kMaxMesswert;                 //entspricht 0,25Hz
				flValidCapture = 0;
			}
			if(cntTimeOut250ms > 0) cntTimeOut250ms --;
			if(cntTimeOut3S > 0) cntTimeOut3S --;
		}
	}

	if(flValidCapture == 1)
	{	if(flNewCapture == 1) 
		{	flNewCapture = 0;

            //Richtungserkennung------------------------------------------------
			if(Periode > k1HzPeriode)
			{	//<1Hz: Entspricht Stillstand
				RichtungStatus = 0xFF;
			}
			else if(Periode < k100HzPeriode)
			{	//>100Hz: Richtung nicht mehr auswerten, da die Signalflanken
                //eventuell zu eng beieinander liegen und falsch interpretiert werden.
				;
			}
			else	// > 1Hz und < 100Hz
			{	if((PS.Par.ControlBits & 0x20) == 0)    //Default Richtung
                {   if(Richtung == _Rechts) RichtungStatus = ((RichtungStatus << 1)+ 1);
                    else				    RichtungStatus = ((RichtungStatus << 1)+ 0);
                }
                else                                    //Invertierte Richtung
                {   if(Richtung == _Links)  RichtungStatus = ((RichtungStatus << 1)+ 1);
                    else				    RichtungStatus = ((RichtungStatus << 1)+ 0);               
                }
			}
            if((RichtungStatus & 0x03) == 0)          TX.bf.Richtung = 0;
            else if((RichtungStatus & 0x03) == 0x03)  TX.bf.Richtung = 1;
            
			//Fehlererkennung bei ständigem Richtungswechsel, wenn z.B. eine Phase wegbricht
            if((flParamKomplett == 1)
            && ((PS.Par.ControlBits & 0x80) == 0))  //Richtungserkennung aktiviert
            {   if((Periode < k5HzPeriode)&&(Periode > k100HzPeriode))
                {	//>10Hz, < 100Hz
                    if(((RichtungStatus & 0x03) == 0x01)||((RichtungStatus & 0x03) == 0x02))
                    {	//Die letzten 2 Bits sind ungleich, also Richtungswechsel
                        cntErrRichtungsWechsel += 4;
                        if(cntErrRichtungsWechsel > 19) flErrRichtungsWechsel = 1;
                    }
                    else	//Die letzten 2 Bits sind gleich (0x00 oder 0x03), also kein Richtungswechsel
                    {	if(cntErrRichtungsWechsel > 0) cntErrRichtungsWechsel --;
                    }
                }
                else	//<10Hz oder >100Hz soll der cntErr dekrementiert werden
                {	if(cntErrRichtungsWechsel > 0) cntErrRichtungsWechsel --;
                }
            }
            else //Parameter nicht komplett oder Richtungserkennung deaktiviert
            {   cntErrRichtungsWechsel = 0;
                flErrRichtungsWechsel = 0;
            }
            //Richtungserkennung Ende-------------------------------------------
        
			//Bei 50 Messungen >1500Hz Error
			if(Periode < k1500HzPeriode) 
			{	cnt1500Hz ++;
				if(cnt1500Hz > 30) fl1500Hz = 1;
			}
			else cnt1500Hz = 0;
        			
			//Bereich der Istfrequnz ermitteln
			BereichIstFrqAlt = BereichIstFrq;		//Der Bereich der letzen Messung in Alt speichern, für 2fach Messung
			if(Periode < k100HzPeriode)			//>100Hz
			{	if(Periode < k200HzPeriode) 	 BereichIstFrq = 6;	//>200Hz
				else if(Periode < k150HzPeriode) BereichIstFrq = 5;	//>150Hz bis 200Hz (Umschalt-Hysterese)
				else 						     BereichIstFrq = 4;	//>100Hz bis 150Hz
			}
			else								//<=100Hz
			{	if(Periode < k75HzPeriode)	     BereichIstFrq = 3;		//>75Hz bis 100Hz
				else if(Periode < k25HzPeriode)  BereichIstFrq = 2;		//Bereich 2: >25Hz bis 75Hz
				else if(Periode < k10HzPeriode)  BereichIstFrq = 1;		//Bereich 1: >10Hz bis 25Hz (Umschalt-Hysterese)
				else						     BereichIstFrq = 0;		//0Hz bis 10Hz	
			}
            //Bei jedem gültigen Capture wird NeueMessung gesetzt, und in FreqenzVergleich() gelöscht
            NeueMessung = 1;
            
            //Bei einer Periode < 193 (6510Hz) würde der X10 Wert die 16Bit sprengen, also begrenzen! 
            if(Periode < 193) IstFrqX10 = 0xFFFF; 
            else( IstFrqX10 = ((uint16_t)(12550000 / Periode)));    //###test###12.500.000 x 1,004 = 12.550.000: 4/1000 mehr.

            TX.Bytes[IPK_FreqH] = IstFrqX10 >> 8;
            TX.Bytes[IPK_FreqL] = IstFrqX10;
		}		
	}
	else if(flValidCapture == 0)	
	{	//warten auf erste Flanke nach PowerOn, der Periodenwert ist noch ungültig
		if(flNewCapture == 1) 
		{	flValidCapture = 1;
			flNewCapture = 0;
		}	
	}	

	//Timeout betreff SchmittTrigger Hystereseumschaltung und Kapazitiver Kopplung
	if(cntTimeOut1S == 0)		//Nach 1s seit der letzten Signalflanke: 
	{	BereichIstFrq = 0;
		BereichIstFrqAlt = 0;
		cnt1500Hz = 0;
	}
	//SchmittTrigger Hysterese Umschaltung per Analogschalter
	//2-fach Messung durch Abfrage BereichIstFrq und BereichIstFrqAlt ,
	//damit nicht versehentlich die Signale durch zu große Hysterese blockiert werden.
	//1,87V-Hysterese: Hyst1=RB10=0. Hyst2=RB11=1. Entspricht LATB |= 0x0800
	//1,32V-Hysterese: Hyst1=RB10=1. Hyst2=RB11=1. Entspricht LATB |= 0x0C00
	//625mV-Hysterese: Hyst1=RB10=0. Hyst2=RB11=0. Entspricht LATB |= 0x0000
	//375mV-Hysterese: Hyst1=RB10=1. Hyst2=RB11=0. Entspricht LATB |= 0x0400
    
	if(fl10ms == 1)
	{	//Rücklesen der PortB Ansteuerung LATB
        notiz = LATB;
        notiz &= 0xF3FF;    //RB10,11 in notiz löschen 

        switch(stmSTHyst)
		{	
            case FrqBereich_0: //---------------------------------------------------
			if(PS.Par.Hysteresemode == 0)
			{	//1,88V: Hyst1=0  Hyst2=1   //Hyst1 = 0x0400
				notiz |= 0x0800;            //Hyst2 = 0x0800    
				LATB = notiz;
				TX.bf.Hyst1 = 0;
                TX.bf.Hyst2 = 1;
				StatusSTHyst = Hyst_1V88;
			}
			else if(PS.Par.Hysteresemode == 1)
			{	//1,33V: Hyst1=1 Hyst2=1
				notiz |= 0x0C00;		  
				LATB = notiz;
				TX.bf.Hyst1 = 1;
                TX.bf.Hyst2 = 1; 
				StatusSTHyst = Hyst_1V33;
			}
			else if(PS.Par.Hysteresemode == 2)
			{	//630mV: Hyst1=0 Hyst2=0
				LATB = notiz;       //wurde oben genullt
				TX.bf.Hyst1 = 0;
                TX.bf.Hyst2 = 0;
				StatusSTHyst = Hyst_630mV;
			}
			else //Modus == 3
			{	//375mV: Hyst1=1 Hyst2=0
				notiz |= 0x0400;	
				LATB = notiz;
				TX.bf.Hyst1 = 1;
                TX.bf.Hyst2 = 0;	
				StatusSTHyst = Hyst_375mV;		
			}
			//In anderen FrqBereich springen?
			if((BereichIstFrq > 1)&&(BereichIstFrqAlt > 1))stmSTHyst = FrqBereich_2_4;
			else	; 								//im Bereich bleiben
			break;
			case FrqBereich_2_4: //------------------------------------------------------------------------------
			if(PS.Par.Hysteresemode == 3)
			{	//375mV: Hyst1=1  Hyst2=0				
				notiz |= 0x0400; 
				LATB = notiz;
				TX.bf.Hyst1 = 1;
                TX.bf.Hyst2 = 0;
				StatusSTHyst = Hyst_375mV;
			}
			else	//Modus 0,1,2 sind gleich
			{	//630mV: Hyst1=0  Hyst2=0
				LATB = notiz;       //wurde oben genullt
				TX.bf.Hyst1 = 0;
                TX.bf.Hyst2 = 0;
				StatusSTHyst = Hyst_630mV;
			}
			//In anderen FrqBereich springen?
			if((BereichIstFrq == 0)&&(BereichIstFrqAlt == 0)) 	stmSTHyst = FrqBereich_0;
			else if((BereichIstFrq == 6)&&(BereichIstFrqAlt == 6)) stmSTHyst = FrqBereich_6;
			else	; 									//im Bereich bleiben
			break;
			case FrqBereich_6:	//-----------------------------------------------------------------------------
			//Modus 0,1,2,3 sind gleich
			{	//375mV: Hyst1=1  Hyst2=0
				notiz |= 0x0400;	  
				LATB = notiz;
				TX.bf.Hyst1 = 1;
                TX.bf.Hyst2 = 0; 
				StatusSTHyst = Hyst_375mV;
			}
			if((BereichIstFrq < 5)&&(BereichIstFrqAlt < 5)) stmSTHyst = FrqBereich_2_4;
			else ;								//im Bereich bleiben
			break;		
		}

		_KapKopplung = 0;							//Analogsch. OFF = KapKoppl ON       
        
        /* Die Berechnung der Frequenz verschoben nach "validCapture + newCapture"
        //Bei einer Periode < 193 (6510Hz) würde der X10 Wert die 16Bit sprengen, also begrenzen! 
        if(Periode < 193) IstFrqX10 = 0xFFFF; 
		else( IstFrqX10 = ((uint16_t)(12550000 / Periode)));    //###test###12.500.000 x 1,004 = 12.550.000: 4/1000 mehr.
             
        TX.Bytes[IPK_FreqH] = IstFrqX10 >> 8;
        TX.Bytes[IPK_FreqL] = IstFrqX10;
        */
	}
} //--------Ende FrequenzAuswerten()----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------
void initFreqMessung(void)
{	//SerTX[3] &= 0xC7;			//RelBit, Limit1Bit und Limit2Bit genullt.	
    TX.Bytes[4] &= 0xF0;    //Limit1-4 gelöscht
    TX.Bytes[5] &= 0xF0;    //Q1-4 gelöscht
//	hysterese1 = 0;			//entsprich Limit1Bit = 0, Istfreq. im roten Bereich
//	hysterese2 = 0;			//entsprich Limit2Bit = 0, Istfreq. im roten Bereich
//	StatusLimit1 = 0xFF;			//entsprich Limit1Bit = 0
//	StatusLimit2 = 0;			//entsprich Limit2Bit = 0
        //cntTimeOut1S = 25;		//25=250ms
        //SerTX[3] |= 0x02;			//1s-Timeout Bit. Bei 1 kein FrqIX Vergleich	
        //IstIndex, NewCapture, ValidCapture. IstIndex wird auch während der Parametrierung ermittelt.
        //cntTimeOut250ms = 25;
        //cntTimeOut3S = 25;		//25=250ms. Ev. >0, damit nicht sofort Quittiert wird/Anlaufüberbrückung		
//	LM = 0;
	TX.bf.Val = 0;			//Validierungsbit-0,1 löschen
	cntTimeOut1 = 0;
	cntTimeOut2 = 25;			//25=250ms, damit das Relais nach kurzer Zeit einschalten kann
        //cntTimeOutMessung = 0;	//Bei 0 wird Periode auf maximalen Wert (0,1Hz) gesetzt	
//	cntAbschaltverz1 = 0;
//	cntAbschaltverz2 = 0;
//	cntEinschaltverz = 0;
	cntDrahtBruchErr = 0;
//	eventuellDB = 0;
        //BereichIstFrq = 6;
        //BereichIstFrqAlt = 6;
        //stmSTHyst = FrqBereich_6;
        //Hyst1=1  Hyst2=0 entsprechend FrqBereich_6
//	_SchTrHyst1 = 1;
//	_SchTrHyst2 = 0;
	TX.bf.Hyst1 = 1;
	TX.bf.Hyst2 = 0; 
    //KapKopplung();
	_KapKopplung = 0;			//Analogsch. OFF = KapKoppl ON						
	//SerTX[1] &= 0xFB;			//KapKopplung Bit
}
//----Ende initFreqMessung()---------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------
//KapKopplung()
//-----------------------------------------------------------------------------------------------------------------------------------------
/* void KapKopplung(void)
{	//Kapazitive Kopplung mit Analogschalter steuern.
	if(KapKopModus == 0)							//Analogsch. OFF = KapKoppl ON
	{	_KapKopplung = 0;							
		 SerTX[1] &= 0xFB;
	}
	else if(KapKopModus == 1)
	{	if((BereichIstFrq < 3)&&(BereichIstFrqAlt < 3))		//0Hz bis 75Hz
		{	_KapKopplung = 1;						//Analogsch. ON = KapKoppl OFF
			SerTX[1] |= 0x04;
		}
		else if((BereichIstFrq > 3)&&(BereichIstFrqAlt > 3))	//>100Hz
		{	_KapKopplung = 0;						//Analogsch. OFF = KapKoppl ON
		 	SerTX[1] &= 0xFB;
		}
	}
	else if(KapKopModus == 2)						//Analogsch. ON = KapKoppl OFF
	{	_KapKopplung = 1;
		SerTX[1] |= 0x04;
	}
} */
//----Ende KapKopplung()---------------------------------------------------------------------------------------------------

    