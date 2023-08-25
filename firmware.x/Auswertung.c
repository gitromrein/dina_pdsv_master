#include <xc.h>
#include "p33CK256MP505.h"
#include "Auswertung.h"
#include "FrequenzVergleich.h"
#include "Frequenz.h"
#include "Can.h"
#include "Diagnose.h"
//#include "Transistoren.h"
#include "Uart.h"
#include <stdint.h>

//------------------------------------------------------------------------------
//Auswertung() wird jede 1ms aufgerufen.
//Die Limit0-3 Bits als Ergebnis aus FrequenzVergleich() werden mit der 
//Quittierfunktion verknüpft und zusammen mit den Error-Zuständen ausgewertet.
//Die IPK Master/Slave Bits UND-verknüpft und auf die CAN Schnittstelle gelegt.
//------------------------------------------------------------------------------
void Auswertung(void)
{      
    cnt1msRoutinen ++;
    
    if(flParamKomplett == 1)
    {   //if(PS.Par.DrehzahlMode == 0)        KeineDzAuswertung();  //0
        //else if(PS.Par.DrehzahlMode < 3)    DrehzahlAuswertung(); //1,2
        //else if(PS.Par.DrehzahlMode < 7)    FensterAuswertung();  //3,4,5,6
        //else if(PS.Par.DrehzahlMode == 7)   BereichAuswertung();  //7       
        DrehzahlAuswertung(PS.Par.DrehzahlMode);
        EmkVerzoegerung(PS.Par.EmkMode);
    }
    else
    {   QuittReset();
        TX.Bytes[4] = 0;
        TX.Bytes[5] = 0;
        CanTX[1] &= 0xF0;   //Q0 bis Q3 löschen
        CanTX[4] &= 0xFC;   //Error und EMK löschen
    }
    
    //DZ-ÜW
    //if((((TX.Bytes[12] | RX.Bytes[12]) & 0x09)==0)    //###test###
    //&& (((TX.Bytes[13] | RX.Bytes[13]) & 0x7F)==0))   //###test###
    if((((TX.Bytes[12]) & 0x09)==0)
    && (((TX.Bytes[13]) & 0x7F)==0))
    {   
        //Es liegen keine Errors vor
        //IPK AND verknüpfen und auf CAN schreiben
        //CanTX[1] = (TX.Bytes[5] & RX.Bytes[5] & 0x0F);    //###test###Q0-Q3
        CanTX[1] = (TX.Bytes[5] & 0x0F);                    //###test###Q0-Q3
        //CanTX[1] |= 0x03;   //###############test###########löschen##Stop und DZ gemutet#####
        //if((CanTX[1] & 0x02) == 0) CanTX[1] &= 0xFE;//###############test###########löschen##Stop folgt DZ
        //else                       CanTX[1] |= 0x01;//###############test###########löschen##
    }
    else
    {   //Es liegen für die DZ-ÜW relevante Fehlerbits vor
        if(((PS.Par.ControlBits & 0x10)!=0)&&(Muting==1))
        {   //Muting der Sensorikfehler aktiv
            ;   //nichts tun          
        }
        else
        {   //Muting der Sensorikfehler nicht aktiv
            QuittReset();
            if((PS.Par.ControlBits & 0x08) != 0)
            {   //Sicherer Stillstand
                TX.Bytes[5] &= 0xF0;    //Stillstand und DZ löschen
                CanTX[1] &= 0xF0;
            }
            else
            {   TX.Bytes[5] &= 0xF1;    //Stillstand bleibt stehen
                CanTX[1] &= 0xF1;       //Stillstand bleibt stehen
            }          
        }
    }
    
    //EMK-ÜW
    //if(((TX.Bytes[13] | RX.Bytes[13]) & 0x7F) == 0)       //###test####
    if(((TX.Bytes[13]) & 0x7F) == 0)
    {   //Kein EMK Fehler
        //if((TX.bf.EMK_Verz == 1)&&(RX.bf.EMK_Verz == 1))  //###test####
        if(TX.bf.EMK_Verz == 1) CanTX[4] |= 0x01;   //Verz.EMK
        else                    CanTX[4] &= 0xFE;                   
    }
    else
    {   //EMK Fehler
        CanTX[4] &= 0xFE;                           //Verzögertes EMK
    }
    
    //Fehlerausgang
    //if((((TX.Bytes[12] | RX.Bytes[12]) & 0x09)==0)    //###test###
    //&& ((TX.Bytes[13] | RX.Bytes[13])==0))            //###test###
    if((((TX.Bytes[12]) & 0x09)==0)
    && ((TX.Bytes[13])==0))
    {   CanTX[4] &= 0xFD;                           //Fehlerausgang löschen
    }
    else
    {   CanTX[4] |= 0x02;                           //Fehlerausgang setzen        
    }
    
    LatchDZ();
    
}//--------Ende Auswertung()-----------------------------------------------


//------------------------------------------------------------------------------
//DrehzahlÜW, FensterÜW und BereichsÜW
//------------------------------------------------------------------------------
void DrehzahlAuswertung(uint8_t dz_mode)
{      
    if(dz_mode == 0)
    {   //keine Auswertung
        TX.Bytes[4] &= 0xF0; //LimitBits 0-3
        TX.Bytes[5] &= 0xF0; //Q0-3
    }
    else if(dz_mode < 3)
    {   //Stillstand und DZ-ÜW
        //Q0: Stillstand ohne Quittierung  
        if(TX.bf.Limit0 == 1) TX.bf.Q0 = 1;
        else                  TX.bf.Q0 = 0;
        //Q1: Drehzahl mit Quittierung
        if(fl10ms == 1) QuittierFunktion(dz_mode);
        if((TX.bf.Limit1 == 1)&&(flQuitt==1)) TX.bf.Q1 = 1;
        else                                  TX.bf.Q1 = 0;  
        
        //Richtungserkennung 
        if(TX.bf.Richtung == 0)         TX.bf.Q2 = 0;
        else                            TX.bf.Q2 = 1;        
        if((PS.Par.ControlBits & 0x40)==0x40)
        {   //ODER Verknüpfung mit Stillstand
            if(TX.bf.Q0 == 1) TX.bf.Q2 = 1;    
        }
        if((PS.Par.ControlBits & 0x80)==0x80)
        {   //Richtung deaktiviert
            TX.bf.Q2 = 0;
        }
    }
    else if(dz_mode < 7)
    {   //Stillstand und Fenster-ÜW
        //Q0: Stillstand ohne Quittierung
        if(TX.bf.Limit0 == 1) TX.bf.Q0 = 1;
        else                  TX.bf.Q0 = 0;
        
        if(fl10ms == 1) QuittierFunktion(dz_mode);
 
        switch(dz_mode)
        {   case 3: //Fenster, manuell Quitt, ohne Anlaufüw --------------------
            if((TX.bf.Limit1 == 0)&&(TX.bf.Limit2 == 1)&&(flQuitt==1)) TX.bf.Q1 = 1;
            else                                                       TX.bf.Q1 = 0; 
            break;
            case 4: //Fenster, manuell Quitt, mit Anlaufüw ---------------------
            //Die Bedingungen für Limit1 sind in der QuittierFunktion() berücksichtig
            if((TX.bf.Limit2 == 1)&&(flQuitt==1)) TX.bf.Q1 = 1;
            else                                  TX.bf.Q1 = 0;
            break;
            case 5: //Fenster, auotm. Quitt, ohne Anlaufüw ---------------------
            if((TX.bf.Limit1 == 0)&&(TX.bf.Limit2 == 1)&&(flQuitt==1)) TX.bf.Q1 = 1;
            else                                                       TX.bf.Q1 = 0;     
            break;
            case 6: //Fenster, auotm. Quitt, mit Anlaufüw ----------------------
            //Die Bedingungen für Limit1 sind in der QuittierFunktion() berücksichtig
            if((TX.bf.Limit2 == 1)&&(flQuitt==1)) TX.bf.Q1 = 1;
            else                                  TX.bf.Q1 = 0;    
            break;           
        }
        //Richtungserkennung       
        if(TX.bf.Richtung == 0)         TX.bf.Q2 = 0;
        else                            TX.bf.Q2 = 1;
        if((PS.Par.ControlBits & 0x40)==0x40)
        {   //ODER Verknüpfung mit Stillstand
            if(TX.bf.Q0 == 1) TX.bf.Q2 = 1;    
        }
        if((PS.Par.ControlBits & 0x80)==0x80)
        {   //Richtung deaktiviert
            TX.bf.Q2 = 0;
        }
    }
    else if(dz_mode == 7)
    {   //Bereich-ÜW
        if(TX.bf.Limit0 == 1)
        {   TX.bf.Q0 = 1;
            TX.bf.Q1 = 0;
            TX.bf.Q2 = 0;
            TX.bf.Q3 = 0;
        }
        else if((TX.bf.Limit0 == 0)&&(TX.bf.Limit1 == 1))
        {   TX.bf.Q0 = 0;
            TX.bf.Q1 = 1;
            TX.bf.Q2 = 0;
            TX.bf.Q3 = 0;           
        }
        else if((TX.bf.Limit1 == 0)&&(TX.bf.Limit2 == 1))
        {   TX.bf.Q0 = 0;
            TX.bf.Q1 = 0;
            TX.bf.Q2 = 1;
            TX.bf.Q3 = 0;
        }
        else if((TX.bf.Limit2 == 0)&&(TX.bf.Limit3 == 1))
        {   TX.bf.Q0 = 0;
            TX.bf.Q1 = 0;
            TX.bf.Q2 = 0;
            TX.bf.Q3 = 1;
        }
        else if(TX.bf.Limit3 == 0)
        {   TX.bf.Q0 = 0;
            TX.bf.Q1 = 0;
            TX.bf.Q2 = 0;
            TX.bf.Q3 = 0;
        }       
    }
} //----------------------------------------------------------------------------


//------------------------------------------------------------------------------
void EmkVerzoegerung(uint8_t emk_mode)
{   
    static uint16_t cntEmkVerz = 0;
    
    if(emk_mode == 0)
    {   TX.bf.EMK = 0;
        TX.bf.EMK_Verz = 0;    
    }
    else if(emk_mode == 1)
    {   //if((TX.bf.EMK == 1)&&(RX.bf.EMK == 1))    //###test###
        if(TX.bf.EMK == 1)                          //###test###
        {   if(fl10ms == 1) 
            {   if(cntEmkVerz > 0) cntEmkVerz --;
                if(cntEmkVerz == 0) TX.bf.EMK_Verz = 1; 
            }
        }
        else
        {   TX.bf.EMK_Verz = 0; 
            cntEmkVerz = PS.Par.EMKEinschaltverz;
        }
    }
} //----------------------------------------------------------------------------


//------------------------------------------------------------------------------
//Wird von jeder Modus1-3 Auswertung() aufgerufen, also alle 10ms
//TODO: _WaitForQuittMS überdenken, insbesondere bei Fensterüberwachung###########test########
//------------------------------------------------------------------------------
void QuittierFunktion(uint8_t dz_mode)
{   uint8_t QEingang;
    //Qittiersignal aus CAN Puffer lesen
    if((CanRX[4] & 0x40) != 0) QEingang = 1;
    else                       QEingang = 0;

    if(dz_mode == 0)
    {   //kein DZ-Modul, keine Quittierung
        flQuitt = 0;
        TX.bf.Quitt = 0;
        cntQuitt = 0;
        cntZeitLimit = 0;
        stmQuitt = _Off;
    }
    else if(dz_mode == 1) //---------------------------------------DZ-ÜW manuell-------------------------------------
    {   //###test###getQuittierError();
        
        switch(stmQuitt)
		{	case _Off:						//Step0
			//Warten auf < Limit1, dann kann quittiert werden
            if(TX.bf.Limit1 == 1)
			{	
                if(QEingang != 0)
                {	stmQuitt = _High200ms;//#######test########
                }
                    
                /* //Nach PowerOn oder Dz OFF. Warten auf QittEingang=Low, dann Step1
				//Wenn High > 10Sek, dann Error
				if(QEingang == 0) 
				{	cntQuitt = 0;
					stmQuitt = _LowXms;
				}
				else //High
				{	if(cntQuitt < 1000) cntQuitt ++;
					else //10Sek auf high sind um, Error
					{	cntQuitt = 0;
					}
				} */
			}
			break;
			//--------------------------------------------
			case _LowXms:							//Step1
			//Wenn Low > 100ms, dann stmQuitt=Step2, andernfalls Step0. 
			if(QEingang == 0)
			{	if(cntQuitt < kTimeLow) cntQuitt ++;
				else //100ms Low sind um
				{	cntQuitt = 0;
					stmQuitt = _WaitForHigh;
				}
			}
			else //High
			{	cntQuitt = 0;
				stmQuitt = _Off;
			}
			break;
			//--------------------------------------------
			case  _WaitForHigh:						//Step2
			//Warten auf High, dann Step3
			if(QEingang != 0)
			{	stmQuitt = _High200ms;
			}
			break;
			//--------------------------------------------
			case _High200ms:						//Step3
			//Wenn High > 150ms dann Step4, andernfalls Step2
			if(QEingang != 0)
			{	if(cntQuitt < kTimeHigh) cntQuitt ++;
				else //100ms Low sind um
				{	cntQuitt = 0;
					stmQuitt = _WaitForLow;
				}
			}
			else //Low
			{	cntQuitt = 0;
				stmQuitt = _WaitForHigh;
			}
			break;
			//--------------------------------------------
			case _WaitForLow:						//Step4
			//Warten auf Low, dann Quittierung=1 und Step5. 
            //Wenn High > 10Sek, dann Error und Step0.
			if(QEingang == 0)
			{	cntQuitt = 0;
				flQuitt = 1;
                TX.bf.Quitt = 1;
				stmQuitt =  _WaitForQuittMS;
			}
			else //High
			{	if(cntQuitt < 1000) cntQuitt ++;
				else //10Sek auf high sind um, Error
				{	cntQuitt = 0;
					stmQuitt =  _Off;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForQuittMS:					//Step5
			//Warten auf Quittierung von Master und Slave 
			if(_QuittMS != 0)
			{	cntQuitt = 0;
				stmQuitt =  _WaitForOff;
			}
			else
			{	if(cntQuitt < 1100) cntQuitt ++;
				if((cntQuitt >= 1100)||(TX.bf.Limit1 == 0))
				{	//1s sind um oder Limit1 Bit == 0
					flQuitt = 0;
                    TX.bf.Quitt = 0;
					cntQuitt = 0;
					stmQuitt =  _Off;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForOff:					//Step8
			//Warten auf Relais OFF, dann Quittierung=0 und Step0
			//Abschaltkriterium: Bei > Limit1 Quittierung löschen.
            if(TX.bf.Limit1 == 0)
			{	cntQuitt = 0;
				flQuitt = 0;
                TX.bf.Quitt = 0;
				stmQuitt =  _Off;
			}
			break;
		}
    }
    else if(dz_mode == 2) //-----------------------------------------DZ-ÜW autom-------------------------------------
    {   switch(stmQuitt)
		{	case _Off:                              //Step0
			//Warten auf < Limit1, dann kann quittiert werden	
            if(TX.bf.Limit1 == 1)
			{	//< Limit1
				if(QEingang != 0) 
				{	flQuitt = 1;
                    TX.bf.Quitt = 1;
					stmQuitt = _WaitForQuittMS;
					cntQuitt = 0;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForQuittMS:					//Step5
			//Warten auf Quittierung von Master und Slave
			if(_QuittMS != 0)
			{	cntQuitt = 0;
				stmQuitt =  _WaitForOff;
			}
			else
			{	if(cntQuitt < _WartenQuitt) cntQuitt ++;
				if((cntQuitt >= _WartenQuitt)||(TX.bf.Limit1 == 0))
				{	//1s sind um oder Limit1 Bit == 0
					flQuitt = 0;
                    TX.bf.Quitt = 0;
					cntQuitt = _MinAbschaltzeit;
					stmQuitt =  _WaitMinOff;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForOff:					//Step8
			//Warten auf OFF, dann Quittierung=0 und Step0
			//Abschaltkriterium: Bei > Limit1 Quittierung löschen
			if(TX.bf.Limit1 == 0) 
			{	flQuitt = 0;
                TX.bf.Quitt = 0;
				cntQuitt = _MinAbschaltzeit;
				stmQuitt =  _WaitMinOff;
			}
			break;
			//--------------------------------------------
			case _WaitMinOff:					//Step9
			//Warten bis Mindest-Abschaltzeit von 0,5s abgelaufen ist
			if(cntQuitt > 0) cntQuitt --;
			else  stmQuitt =  _Off;
			break;
		}    
    }
    else if(dz_mode == 3) //--------------------Fenster-ÜW manuel, ohne Anlaufüw-------------------------------------
    {   //###test###getQuittierError();
        
        switch(stmQuitt)
		{	case _Off:						//Step0
			//Warten auf > Limit1 und < Limit2, dann kann quittiert werden
			if((TX.bf.Limit1 == 0)&&(TX.bf.Limit2 == 1))
			{	//> Limit1 und < Limit2               
                if(QEingang != 0)
                {	stmQuitt = _High200ms;//#######test########
                }                   
                /* 
				//Nach PowerOn oder OFF. Warten auf QittEingang=Low, dann Step1
				//Wenn High > 10Sek, dann Error
				if(QEingang == 0) 
				{	cntQuitt = 0;
					stmQuitt = _LowXms;
				}
				else //High
				{	if(cntQuitt < 1000) cntQuitt ++;
					else //10Sek auf high sind um, Error
					{	cntQuitt = 0;
					}
				}   */
			}
			break;
			//--------------------------------------------
			case _LowXms:							//Step1
			//Wenn Low > 100ms, dann stmQuitt=Step2, andernfalls Step0. 
			if(QEingang == 0)
			{	if(cntQuitt < kTimeLow) cntQuitt ++;
				else //100ms Low sind um
				{	cntQuitt = 0;
					stmQuitt = _WaitForHigh;
				}
			}
			else //High
			{	cntQuitt = 0;
				stmQuitt = _Off;
			}
			break;
			//--------------------------------------------
			case  _WaitForHigh:						//Step2
			//Warten auf High, dann Step3
			if(QEingang != 0)
			{	stmQuitt = _High200ms;
			}
			break;
			//--------------------------------------------
			case _High200ms:						//Step3
			//Wenn High > 100ms dann Step4, andernfalls Step2
			if(QEingang != 0)
			{	if(cntQuitt < kTimeHigh) cntQuitt ++;
				else //100ms Low sind um
				{	cntQuitt = 0;
					stmQuitt = _WaitForLow;
				}
			}
			else //Low
			{	cntQuitt = 0;
				stmQuitt = _WaitForHigh;
			}
			break;
			//--------------------------------------------
			case _WaitForLow:						//Step4
			//Warten auf Low, dann Quittierung=1 und Step5. Wenn High > 10Sek, dann Error und Step0.
			if(QEingang == 0)
			{	
                if((TX.bf.Limit1 == 0)&&(TX.bf.Limit2 == 1))
				{	//Fallende Flanke und innerhalb des Fensters(> Limit1, < Limit2), OK
					cntQuitt = 0;
					flQuitt = 1;
                    TX.bf.Quitt = 1;
					cntZeitLimit = 0;   //PS.Par.Anlaufzeit;###test#####
					stmQuitt =  _WaitForQuittMS;
				}
				else	////Fallende Flanke aber außerhalb des Fensters, zurück auf 0
				{	cntQuitt = 0;
					flQuitt = 0;
                    TX.bf.Quitt = 0;
					cntZeitLimit = 0;
					stmQuitt =  _Off;
				}
			}
			else //High
			{	if(cntQuitt < 1000) cntQuitt ++;
				else //10Sek auf high sind um, Error
				{	cntQuitt = 0;
					stmQuitt =  _Off;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForQuittMS:					//Step5
			//Warten auf Quittierung von Master und Slave 
			if(_QuittMS != 0)
			{	cntQuitt = 0;
				stmQuitt =  _WaitForOff;
			}
			else //Quittierungen==0
			{	if(cntQuitt < _WartenQuitt) cntQuitt ++;
				if((cntQuitt >= _WartenQuitt)
                ||(TX.bf.Limit1 == 1)       //< Fenster Unterkante
                ||(TX.bf.Limit2 == 0))      //> Fenster Oberkante ((SerTX[4] & 0x06)!= 0x04))
				{	//1s sind um oder nicht im Fenster
					flQuitt = 0;
                    TX.bf.Quitt = 0;
					cntQuitt = 0;
					stmQuitt =  _Off;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForOff:					//Step8
			//Warten auf OFF, dann Quittierung=0 und Step0
			//Abschaltkriterium: Bei < Limit1 > Limit2 Quittierung löschen.
            if((TX.bf.Limit1 == 1)||(TX.bf.Limit2 == 0))  //< Limmit1 oder > Limit2
			{	cntQuitt = 0;
				flQuitt = 0;
                TX.bf.Quitt = 0;
				stmQuitt =  _Off;
			}
			break;
		}
    }
    else if(dz_mode == 4) //---------------------Fenster-ÜW manuel, mit Anlaufüw-------------------------------------
    {   //###test###getQuittierError();
        
        switch(stmQuitt)
		{	case _Off:						//Step0
			//Warten auf < Limit2, dann kann quittiert werden
            if(TX.bf.Limit2 == 1)
			{   if(QEingang != 0)
                {	stmQuitt = _High200ms;//#######test########
                }                  
                /* 
                //Nach PowerOn oder DzOFF. Warten auf QittEingang=Low, dann Step1
				//Wenn High > 10Sek, dann Error
				if(QEingang == 0) 
				{	cntQuitt = 0;
					stmQuitt = _LowXms;
				}
				else //High
				{	if(cntQuitt < 1000) cntQuitt ++;
					else //10Sek auf high sind um, Error
					{	cntQuitt = 0;
					}
				}   */
			}
			break;
			//--------------------------------------------
			case _LowXms:							//Step1
			//Wenn Low > 100ms, dann stmQuitt=Step2, andernfalls Step0. 
			if(QEingang == 0)
			{	if(cntQuitt < kTimeLow) cntQuitt ++;
				else //100ms Low sind um
				{	cntQuitt = 0;
					stmQuitt = _WaitForHigh;
				}
			}
			else //High
			{	cntQuitt = 0;
				stmQuitt = _Off;
			}
			break;
			//--------------------------------------------
			case  _WaitForHigh:						//Step2
			//Warten auf High, dann Step3
			if(QEingang != 0)
			{	stmQuitt = _High200ms;
			}
			break;
			//--------------------------------------------
			case _High200ms:						//Step3
			//Wenn High > 150ms dann Step4, andernfalls Step2
			if(QEingang != 0)
			{	if(cntQuitt < kTimeHigh) cntQuitt ++;
				else //150ms Low sind um
				{	cntQuitt = 0;
					stmQuitt = _WaitForLow;
				}
			}
			else //Low
			{	cntQuitt = 0;
				stmQuitt = _WaitForHigh;
			}
			break;
			//--------------------------------------------
			case _WaitForLow:						//Step4
			//Warten auf Low, dann Quittierung=1 und Step5. Wenn High > 10Sek, dann Error und Step0.
			if(QEingang == 0)
			{	cntQuitt = 0;
				flQuitt = 1;
                TX.bf.Quitt = 1;
				cntZeitLimit = PS.Par.Anlaufzeit;   //Ab Quittierung läuft das Zeitfenster für die Motorstartphase ab.
				stmQuitt =  _WaitForQuittMS;
			}
			else //High
			{	if(cntQuitt < 1000) cntQuitt ++;
				else //10Sek auf high sind um, Error
				{	cntQuitt = 0;
					stmQuitt =  _Off;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForQuittMS:					//Step5
			//Warten auf Quittierung von Master und Slave 
			if(_QuittMS != 0)
			{	cntQuitt = 0;
				stmQuitt =  _WaitForLimit1;
			}
			else
			{	if(cntQuitt < _WartenQuitt) cntQuitt ++;
				if((cntQuitt >= _WartenQuitt)||(TX.bf.Limit2 == 0))     //((SerTX[4] & 0x04) == 0))
				{	//1s sind um oder Limit2 Bit == 0
					flQuitt = 0;
                    TX.bf.Quitt = 0;
					cntQuitt = 0;
					stmQuitt =  _Off;
				}
			}
			break;
			//--------------------------------------------
			case  _WaitForLimit1:					//Step7
			//Warten auf > Limit1, damit nach Quittieren bei < Limit0 nicht sofort wieder abgeschaltet wird
            if(TX.bf.Limit1 == 0)
			{	// > Limit1
				stmQuitt = _WaitForOff;
				cntZeitLimit = 0;
			}
			else // < Limit1
			{	if(cntZeitLimit > 0) cntZeitLimit --;	//ZeitLimit für Motor-Startphase läuft ab
				if(cntZeitLimit == 0)
				{	//Zeitlimit ist abgelaufen, Quittierung löschen, das Relais wird in Auswertung() abgeschaltet
					cntQuitt = 0;
					flQuitt = 0;
                    TX.bf.Quitt = 0;
					stmQuitt =  _Off;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForOff:					//Step8
			//Warten auf OFF, dann Quittierung=0 und Step0
			//Abschaltkriterium: Bei < Limit1 > Limit2 Quittierung löschen.
            if((TX.bf.Limit1 == 1)||(TX.bf.Limit2 == 0))
			{	//< Limit1 oder > Limit2
                cntQuitt = 0;
				flQuitt = 0;
                TX.bf.Quitt = 0;
				stmQuitt =  _Off;
			}
			break;
		}
    }
    else if(dz_mode == 5) //---------------------Fenster-ÜW autom, ohne Anlaufüw-------------------------------------
    {   //Es wird ausschließlich im Fensterbereich Quittiert
		switch(stmQuitt)
		{	case _Off:						//Step0
            if((TX.bf.Limit1 == 0)&&(TX.bf.Limit2 == 1))
			{	//> Limit1 und < Limit2
				if(QEingang != 0) 
				{	//Quittierung = 1;
                    flQuitt = 1;
                    TX.bf.Quitt = 1;
					stmQuitt = _WaitForQuittMS;
					cntQuitt = 0;
				}
			}
			//Anlaufüberbrückung entfällt.
			break;
			//--------------------------------------------
			case _WaitForQuittMS:					//Step5
			//Warten auf Quittierung von Master und Slave 
			if(_QuittMS != 0)
			{	cntQuitt = 0;
				stmQuitt =  _WaitForOff;
			}
			else //_flRelais==0
			{	if(cntQuitt < _WartenQuitt) cntQuitt ++;
				if((cntQuitt >= _WartenQuitt)   //||((SerTX[4] & 0x06) != 0x04))
                ||(TX.bf.Limit1 == 1)
                ||(TX.bf.Limit2 == 0))
				{	//1s sind um oder außerhalb dem Fenster
					flQuitt = 0;
                    TX.bf.Quitt = 0;
					cntQuitt = _MinAbschaltzeit;
					stmQuitt =  _WaitMinOff;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForOff:					//Step8
			//Warten auf OFF, dann Quittierung=0 und Step0
			//Abschaltkriterium: Bei < Limit0 > Limit1 Quittierung löschen.
            if((TX.bf.Limit1 == 1)||(TX.bf.Limit2 == 0))
			{	flQuitt = 0;
                TX.bf.Quitt = 0;
				cntQuitt = _MinAbschaltzeit;
				stmQuitt =  _WaitMinOff;
			}
			break;
			//--------------------------------------------
			case _WaitMinOff:					//Step9
			//Warten bis Mindest-Abschaltzeit der Relais von 0,5s abgelaufen ist
			if(cntQuitt > 0) cntQuitt --;
			else  stmQuitt =  _Off;
			break;
		}
    }
    else if(dz_mode == 6) //----------------------Fenster-ÜW autom, mit Anlaufüw-------------------------------------
    {   switch(stmQuitt)
		{	case _Off:                                                   //Step0
			cntQuitt = 0;
			if(QEingang != 0) 
			{   
                if((TX.bf.Limit1 == 0)&&(TX.bf.Limit2 == 1))
				{	//> Limit1 und < Limit2
                    flQuitt = 1;
                    TX.bf.Quitt = 1;
					stmQuitt = _WaitForOnWindow; //(Step10)
				}
				//Anlaufüberbrückung: Bedingung Limit0 = Stillstand = ON
                //else if((TX.bf.Limit0 == 1)&&(RX.bf.Limit0 == 1))
                else if(TX.bf.Limit0 == 1)
				{	// Master und Slave Limit0 == 1
					flQuitt = 1;
                    TX.bf.Quitt = 1;
					stmQuitt = _WaitForOnLim0;   //(Step11)
				}
			}
			break;
			//--------------------------------------------
			case _WaitForOnWindow:  //PIC hat schon eingesch. da im Fenster //Step10
			//Warten auf Qitt ON von Master und Slave
			if(_QuittMS != 0)
			{	cntQuitt = 0;
				stmQuitt =  _WaitForOff;
			}
			else
			{	if(cntQuitt < _WartenQuitt) cntQuitt ++;
				if((cntQuitt >= _WartenQuitt)       //||((SerTX[4] & 0x06) != 0x04))
                ||(TX.bf.Limit1 == 1)
                ||(TX.bf.Limit2 == 0))
				{	//1s sind um oder außerhalb dem Fenster
                    flQuitt = 0;
                    TX.bf.Quitt = 0;
					cntQuitt = _MinAbschaltzeit;
					stmQuitt =  _WaitMinOff;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForOnLim0:  //PIC hat schon eingeschaltet bei <1Hz //Step11
			//Warten auf Qitt ON von Master und Slave
			if(_QuittMS != 0)
			{	cntQuitt = 0;
				stmQuitt =  _WaitForLim0_0;
			}
			else
			{	if(cntQuitt < _WartenQuitt) cntQuitt ++;
				if((cntQuitt >= _WartenQuitt)       //||((SerTX[4] & 0x06) != 0x04))
                ||(TX.bf.Limit1 == 1)
                ||(TX.bf.Limit2 == 0))
				{	//1s sind um oder außerhalb dem Fenster
					flQuitt = 0;
                    TX.bf.Quitt = 0;
					cntQuitt = _MinAbschaltzeit;
					stmQuitt =  _WaitMinOff;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForLim0_0:                                         //Step6
			//Unbegrenzt Warten bis zum Antriebsstart
            if(TX.bf.Limit0 == 0)
			{	//PIC > Limit0, also > Stillstand
				cntZeitLimit = PS.Par.Anlaufzeit;	//Ab Quittierung läuft das Zeitfenster für die Motorstartphase ab.
				stmQuitt = _WaitForLimit1;
			}
			break;
			//-------------------------------------------
			case  _WaitForLimit1:                                       //Steip7
			//Antriebsstart Überbrückung
			//Warten auf > Limit1, damit nach Quittieren bei < Limit1 nicht sofort wieder abgeschaltet wird
            if(TX.bf.Limit1 == 0)
			{	// > Limit1
				stmQuitt = _WaitForOff;
				cntZeitLimit = 0;
			}
			else // < Limit1
			{	if(cntZeitLimit > 0) cntZeitLimit --;		//ZeitLimit für Motor-Startphase läuft ab
				if(cntZeitLimit == 0)
				{	//Zeitlimit ist abgelaufen, Quittierung löschen, das Relais wird in Auswertung() abgeschaltet
					flQuitt = 0;
                    TX.bf.Quitt = 0;
					cntQuitt = _MinAbschaltzeit;
					stmQuitt =  _WaitMinOff;
				}
			}
			break;
			//--------------------------------------------
			case _WaitForOff:                                            //Step8
			//Warten auf OFF, dann Quittierung=0 und Step0
			//Abschaltkriterium: Bei < Limit1 oder > Limit2 Quittierung löschen.
            if((TX.bf.Limit1 == 1)||(TX.bf.Limit2 == 0))
			{	flQuitt = 0;
                TX.bf.Quitt = 0;
				cntQuitt = _MinAbschaltzeit;
				stmQuitt =  _WaitMinOff;
			}
			break;
			//--------------------------------------------
			case _WaitMinOff:                                            //Step9
			//Warten bis Mindest-Abschaltzeit der Relais von 0,5s abgelaufen ist
			if(cntQuitt > 0) cntQuitt --;
			else  stmQuitt =  _Off;
			break;
            
           
		}
    }
    else if(dz_mode == 7) //------------------------Bereich-ÜW keine Quittierung-------------------------------------
    {   //bei BereichÜW ist keine Quittierung vorgesehen
        flQuitt = 0;
        TX.bf.Quitt = 0;
        cntQuitt = 0;
        cntZeitLimit = 0;
        stmQuitt = _Off; 
    }
    
    /*
    //Bei Fehler QuittReset() aufrufen##########test####### TX & RX #######
    //if((((TX.Bytes[12] | RX.Bytes[12]) & 0x09)!= 0)   //QuittErr, Dr.Br.PulsErr
    //|| (((TX.Bytes[13] | RX.Bytes[13]) & 0x7F)!= 0))
    if(((TX.Bytes[12] & 0x09)!= 0)  //QuittErr, Dr.Br.PulsErr
    || ((TX.Bytes[13] & 0x01)!= 0)  //Dr.Br.Err, wieder erweitern auf 0x7F###test#####
    || (flParamKomplett == 0))
    {   QuittReset();   
    }
    */

} //------Ende QuittierFunktion()------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void QuittReset(void)
{
    flQuitt = 0;
    TX.bf.Quitt = 0;
    cntQuitt = 0;
    cntZeitLimit = 0;
    stmQuitt = _Off; 
} //----------------------------------------------------------------------------

