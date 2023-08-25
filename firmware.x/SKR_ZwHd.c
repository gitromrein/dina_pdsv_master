#include <xc.h>
#include "p33CK256MP505.h"
#include "SKR_ZwHd.h"
#include "Uart.h"
#include "Frequenz.h"
#include <stdint.h>

uint8_t cntDelSkrAusw = _konstDelSkrAusw;
uint8_t sswSKR1 = 0, sswSKR2 = 0, sswSKR3 = 0, sswSKR4 = 0;
uint8_t cntQdelay1 = 0, cntQdelay2 = 0, cntQdelay3 = 0, cntQdelay4 = 0;
uint8_t cntTOSkr1 = 0, cntTOSkr2 = 0, cntTOSkr3 = 0, cntTOSkr4 = 0;
uint8_t _flQpos1 = 0, _flQpos2 = 0, _flQpos3 = 0, _flQpos4 = 0;
uint8_t _flQ1 = 0, _flQ2 = 0, _flQ3 = 0, _flQ4 = 0;
uint8_t _flSKR1 = 0, _flSKR2 = 0, _flSKR3 = 0, _flSKR4 = 0;
uint8_t _flSKR1Q = 0, _flSKR2Q = 0, _flSKR3Q = 0, _flSKR4Q = 0;
uint8_t _flSkrErr1 = 0, _flSkrErr2 = 0, _flSkrErr3 = 0, _flSkrErr4 = 0;
uint8_t _flSkrQSErr1 = 0, _flSkrQSErr2 = 0, _flSkrQSErr3 = 0, _flSkrQSErr4 = 0;
uint8_t SkrErrGrundst = 0;
uint8_t cntQuerschlSkr1a = 0, cntQuerschlSkr1b = 0, cntQuerschlSkr2a = 0, cntQuerschlSkr2b = 0;
uint8_t cntQuerschlSkr3a = 0, cntQuerschlSkr3b = 0, cntQuerschlSkr4a = 0, cntQuerschlSkr4b = 0;
//uint8_t CanTX2alt = 0;

//-----------------------------------------------------------------------------------------------------------------------------------------
//	SkrAuswerten(), wird alle 10ms aufgerufen,
//-----------------------------------------------------------------------------------------------------------------------------------------
void SkrAuswerten(void)
{	uint8_t antivalenz = 0, eingaenge = 0, InputSKR = 0;
	cnt10msRoutinen ++;
	
	if(((CanRX[0] & 0x80) != 0) || (flParamKomplett == 0))
	{	//SKR Reset
		cntDelSkrAusw = _konstDelSkrAusw;
		sswSKR1 = 0;
		sswSKR2 = 0;
		sswSKR3 = 0;
		sswSKR4 = 0;
		cntQdelay1 = 0;
		cntQdelay2 = 0;
		cntQdelay3 = 0;
		cntQdelay4 = 0;
		cntTOSkr1 = 0;
		cntTOSkr2 = 0;
		cntTOSkr3 = 0;
		cntTOSkr4 = 0;
		_flQpos1 = 0; _flQpos2 = 0; _flQpos3 = 0; _flQpos4 = 0;
		_flQ1 = 0; _flQ2 = 0; _flQ3 = 0; _flQ4 = 0;
		_flSKR1 = 0; _flSKR2 = 0; _flSKR3 = 0; _flSKR4 = 0;
        _flSKR1Q = 0; _flSKR2Q = 0; _flSKR3Q = 0; _flSKR4Q = 0;
		TX.bf.SKR1 = 0;		//SerTX[0] SKRQ1-4 gelöscht.
        TX.bf.SKR2 = 0;
	}
	else	//kein Error und ParamKomplett
	{	if(cntDelSkrAusw != 0) cntDelSkrAusw --;
		else
		{	antivalenz = 0;
            if((PS.Par.SkrAntival & 0x01) != 0) antivalenz |= 0x02;
            if((PS.Par.SkrAntival & 0x02) != 0) antivalenz |= 0x08;
            if((PS.Par.SkrAntival & 0x04) != 0) antivalenz |= 0x20;
            if((PS.Par.SkrAntival & 0x08) != 0) antivalenz |= 0x80;       
			//Entprellte Eing. SKR übergeben.
			//Antivalente Eing. berücksichtigen. XOR mit SerTX[1] (selectedInDebounced)
			InputSKR = ((antivalenz ^TX.Bytes[1]) & 0x0F);    //SerTX[1]
			//-----SKR1	Quittierung----------------------------------------------------------------------------
			if(cntQdelay1 != 0)	cntQdelay1 --;
			else				 _flQ1 = 0;
			if((CanRX[1] & 0x01) == 0x01)
			{		//Can-Q == 1
				if(_flQDelay1 == 0) cntQdelay1 = _konstQdelayWert;	//1=0ms, 0=500ms
				if(_flQPegel1 == 0)
				{	//quittieren bei high Pegel
					_flQ1 = 1;
				}
				else //quittieren bei fallender Flanke
				{	_flQpos1 = 1;
				}
			}
			else	//Can-Q == 0
			{	if(_flQpos1 == 1)
				{	//negative Quittier-Flanke
					_flQ1 = 1;
					_flQpos1 = 0;
				}
			}
			//-----------------------------------------------------------------
			if(_flAutoMan1 == 0) _flQ1 = 1;	//automatisch quittieren
			//-----------------------------------------------------------------
			//bei gesetztem SKR Quittierflags löschen
			if(_flSKR1Q	 == 1)
			{	_flQpos1 = 0;
				_flQ1 = 0;
				cntQdelay1 = 0;
				//wenn cnt hier gelöscht wird, dann ist Nachprellen nicht erlaubt
				//cntTOSkr1 = 0;
			}
			//-----SKR2	Quittierung----------------------------------------------------------------------------
			if(cntQdelay2 != 0)	cntQdelay2 --;
			else				 _flQ2 = 0;
			if((CanRX[1] & 0x02) == 0x02)
			{		//Can-Q == 1
				if(_flQDelay2 == 0) cntQdelay2 = _konstQdelayWert;	//1=0ms, 0=500ms
				if(_flQPegel2 == 0)
				{	//quittieren bei high Pegel
					_flQ2 = 1;
				}
				else //quittieren bei fallender Flanke
				{	_flQpos2 = 1;
				}
			}
			else	//Can-Q == 0
			{	if(_flQpos2 == 1)
				{	//negative Quittier-Flanke
					_flQ2 = 1;
					_flQpos2 = 0;
				}
			}
			//-----------------------------------------------------------------
			if(_flAutoMan2 == 0) _flQ2 = 1;	//automatisch quittieren
			//-----------------------------------------------------------------
			//bei gesetztem SKR Quittierflags löschen
			if(_flSKR2Q	 == 1)
			{	_flQpos2 = 0;
				_flQ2 = 0;
				cntQdelay2 = 0;
				//wenn cnt hier gelöscht wird, dann ist Nachprellen nicht erlaubt
				//cntTOSkr2 = 0;
			}
			//-----SKR3	Quittierung----------------------------------------------------------------------------
			if(cntQdelay3 != 0)	cntQdelay3 --;
			else				 _flQ3 = 0;
			if((CanRX[1] & 0x04) == 0x04)
			{		//Can-Q == 1
				if(_flQDelay3 == 0) cntQdelay3 = _konstQdelayWert;	//1=0ms, 0=500ms
				if(_flQPegel3 == 0)
				{	//quittieren bei high Pegel
					_flQ3 = 1;
				}
				else //quittieren bei fallender Flanke
				{	_flQpos3 = 1;
				}
			}
			else	//Can-Q == 0
			{	if(_flQpos3 == 1)
				{	//negative Quittier-Flanke
					_flQ3 = 1;
					_flQpos3 = 0;
				}
			}
			//-----------------------------------------------------------------
			if(_flAutoMan3 == 0) _flQ3 = 1;	//automatisch quittieren
			//-----------------------------------------------------------------
			//bei gesetztem SKR Quittierflags löschen
			if(_flSKR3Q	 == 1)
			{	_flQpos3 = 0;
				_flQ3 = 0;
				cntQdelay3 = 0;
				//wenn cnt hier gelöscht wird, dann ist Nachprellen nicht erlaubt
				//cntTOSkr3 = 0;
			}
			//-----SKR4	Quittierung----------------------------------------------------------------------------
			if(cntQdelay4 != 0)	cntQdelay4 --;
			else				 _flQ4 = 0;
			if((CanRX[1] & 0x08) == 0x08)
			{		//Can-Q == 1
				if(_flQDelay4 == 0) cntQdelay4 = _konstQdelayWert;	//1=0ms, 0=500ms
				if(_flQPegel4 == 0)
				{	//quittieren bei high Pegel
					_flQ4 = 1;
				}
				else //quittieren bei fallender Flanke
				{	_flQpos4 = 1;
				}
			}
			else	//Can-Q == 0
			{	if(_flQpos4 == 1)
				{	//negative Quittier-Flanke
					_flQ4 = 1;
					_flQpos4 = 0;
				}
			}
			//-----------------------------------------------------------------
			if(_flAutoMan4 == 0) _flQ4 = 1;	//automatisch quittieren
			//-----------------------------------------------------------------
			//bei gesetztem SKR Quittierflags löschen
			if(_flSKR4Q	 == 1)
			{	_flQpos4 = 0;
				_flQ4 = 0;
				cntQdelay4 = 0;
				//wenn cnt hier gelöscht wird, dann ist Nachprellen nicht erlaubt
				//cntTOSkr4 = 0;
			}
        
			//-----------------------------------------------------------------------------------------------------------
			//-----SKR1 Statemachine--------------------------------------------------------------------------
			//Eingänge verschieben und maskieren
			eingaenge = InputSKR;
			//eingaenge = eingaenge >> 2;
			eingaenge &= 0x03;
			switch(sswSKR1)
			{	case 0:	//--------------------------------------------------------------
				//Schritt 0, Zustand nach SLOKoff Wechsel von 1 auf 0
				if(_flPowerOnGrSt1 != 0)
				{	//Grundstellung 0/0 wird nicht verlangt
					switch(eingaenge)
					{	case 0b11:
						_flSKR1 = 1;
						//flQ1 = 1	//Quittierung nicht nach PowerOn setzen
						sswSKR1 = 5;
						break;
						default: //0b00, 0b01, 0b10
						sswSKR1 = 1;
						break;
					}
				}
				else //_flPowerOnGrSt1 == 0
				{	//Grundstellung 0/0 wird verlangt
					//wenn nicht 0/0, dann ErrGrundstellung
					switch(eingaenge)
					{	case 0b00:
						sswSKR1 = 1;
						break;
						default: //01, 10, 11
						SkrErrGrundst |= 0x01;
						//sswSKR1 erst incr. wenn in Grundstellung
						break;
					}
				}
				break;
				case 1:	//--------------------------------------------------------------
				//Schritt 1, _SKR aus, Abfrage Eingänge 00
				switch(eingaenge)
				{	case 0b00:
					sswSKR1 = 2;
					break;
					case 0b11:
					if(_flSkrQSErr1 == 0)
					{	//nach Querschluss würde auch Einkanalerror gesetzt werden
						//Eingänge sind 11, waren aber nicht vorher 00
						_flSkrErr1 = 1;
					}
					break;
					default: //01, 10, 11
					//nicht tun
					break;
				}
				break;
				case 2:	//--------------------------------------------------------------
				//Schritt 2, Grundstellung, _SKR aus, Abfrage Eingänge 11,  10 oder 01
				switch(eingaenge)
				{	case 0b11:
					 cntTOSkr1 = _konstTOSkr;
					_flSKR1 = 1;
					sswSKR1 = 4;
					break;
					case 0b00:
					//nichts tun
					break;
					default: //01, 10
					//cntTOSkr1 = _konstTOSkr; //deaktiviert
					_flSKR1 = 0;
					sswSKR1 = 3;
					break;
				}
				break;
				case 3:	//--------------------------------------------------------------
				//Schritt 3, Zustand 10 oder 01
				//bei 10 oder 01 soll der timeout noch nicht ablaufen
				switch(eingaenge)
				{	case 0b00:
					_flSKR1 = 0;
					sswSKR1 = 1;
					break;
					case 0b11:
					_flSKR1 = 1;
					cntTOSkr1 = _konstTOSkr;
					sswSKR1 = 4;
					break;
					default:
					//nichts tun
					break;
				}
				break;
				case 4:	//--------------------------------------------------------------
				//Schritt 4, Zustand 1/1, _SKR = 1, timeout läuft ab
				//einkanaliges Prellen führt noch nicht zum Abschalten
				if(cntTOSkr1 != 0)
				{	cntTOSkr1 --;
					switch(eingaenge)
					{	case 0b00:
						_flSKR1 = 0;
						sswSKR1 = 1;
						break;
						default:	//01, 10, 11
						//mindestens 1 Eingang auf 1, abwarten
						break;
					}
				}
				else	//cnt TOSkr1 == 0
				{	switch(eingaenge)
					{	case 0b11:
						_flSKR1 = 1;
						sswSKR1 = 5;
						break;
						default:	//00, 01, 10
						_flSKR1 = 0;
						sswSKR1 = 1;
						break;
					}
				}
				break;
				case 5:	//--------------------------------------------------------------
				//Schritt 5, Zustand 1/1, _SKR = 1, ungleich 1/1 führt zum Abschalten
				switch(eingaenge)
				{	case 0b11:
					_flSKR1 = 1;
					//ok, abwarten
					break;
					default:
					_flSKR1 = 0;
					sswSKR1 = 1;
					break;
				}
				break;
			}
			//-----------------------------------------------------------------
			if(_flSKR1 == 0) _flSKR1Q = 0;
			if((_flSKR1 == 1) && (_flQ1 == 1)) _flSKR1Q = 1;
			//-----------------------------------------------------------------------------------------------------------
			//-----SKR2 Statemachine--------------------------------------------------------------------------
			//Eingänge verschieben und maskieren
			eingaenge = InputSKR;
			eingaenge = eingaenge >> 2;
			eingaenge &= 0x03;
			switch(sswSKR2)
			{	case 0:	//--------------------------------------------------------------
				//Schritt 0, Zustand nach SLOKoff Wechsel von 1 auf 0
				if(_flPowerOnGrSt2 != 0)
				{	//Grundstellung 0/0 wird nicht verlangt
					switch(eingaenge)
					{	case 0b11:
						_flSKR2 = 1;
						//flQ1 = 1	//Quittierung nicht nach PowerOn setzen
						sswSKR2 = 5;
						break;
						default: //0b00, 0b01, 0b10
						sswSKR2 = 1;
						break;
					}
				}
				else //_flPowerOnGrSt2 == 0
				{	//Grundstellung 0/0 wird verlangt
					//wenn nicht 0/0, dann ErrGrundstellung
					switch(eingaenge)
					{	case 0b00:
						sswSKR2 = 1;
						break;
						default: //01, 10, 11
						SkrErrGrundst |= 0x02;
						//sswSKR2 erst incr. wenn in Grundstellung
						break;
					}
				}
				break;
				case 1:	//--------------------------------------------------------------
				//Schritt 1, _SKR aus, Abfrage Eingänge 00
				switch(eingaenge)
				{	case 0b00:
					sswSKR2 = 2;
					break;
					case 0b11:
					if(_flSkrQSErr2 == 0)
					{	//nach Querschluss würde auch Einkanalerror gesetzt werden
						//Eingänge sind 11, waren aber nicht vorher 00
						_flSkrErr2 = 1;
					}
					break;
					default: //01, 10, 11
					//nicht tun
					break;
				}
				break;
				case 2:	//--------------------------------------------------------------
				//Schritt 2, Grundstellung, _SKR aus, Abfrage Eingänge 11,  10 oder 01
				switch(eingaenge)
				{	case 0b11:
					 cntTOSkr2 = _konstTOSkr;
					_flSKR2 = 1;
					sswSKR2 = 4;
					break;
					case 0b00:
					//nichts tun
					break;
					default: //01, 10
					//cntTOSkr2 = _konstTOSkr; //deaktiviert
					_flSKR2 = 0;
					sswSKR2 = 3;
					break;
				}
				break;
				case 3:	//--------------------------------------------------------------
				//Schritt 3, Zustand 10 oder 01
				//bei 10 oder 01 soll der timeout noch nicht ablaufen
				switch(eingaenge)
				{	case 0b00:
					_flSKR2 = 0;
					sswSKR2 = 1;
					break;
					case 0b11:
					_flSKR2 = 1;
					cntTOSkr2 = _konstTOSkr;
					sswSKR2 = 4;
					break;
					default:
					//nichts tun
					break;
				}
				break;
				case 4:	//--------------------------------------------------------------
				//Schritt 4, Zustand 1/1, _SKR = 1, timeout läuft ab
				//einkanaliges Prellen führt noch nicht zum Abschalten
				if(cntTOSkr2 != 0)
				{	cntTOSkr2 --;
					switch(eingaenge)
					{	case 0b00:
						_flSKR2 = 0;
						sswSKR2 = 1;
						break;
						default:	//01, 10, 11
						//mindestens 1 Eingang auf 1, abwarten
						break;
					}
				}
				else	//cnt TOSkr2 == 0
				{	switch(eingaenge)
					{	case 0b11:
						_flSKR2 = 1;
						sswSKR2 = 5;
						break;
						default:	//00, 01, 10
						_flSKR2 = 0;
						sswSKR2 = 1;
						break;
					}
				}
				break;
				case 5:	//--------------------------------------------------------------
				//Schritt 5, Zustand 1/1, _SKR = 1, ungleich 1/1 führt zum Abschalten
				switch(eingaenge)
				{	case 0b11:
					_flSKR2 = 1;
					//ok, abwarten
					break;
					default:
					_flSKR2 = 0;
					sswSKR2 = 1;
					break;
				}
				break;
			}
			//-----------------------------------------------------------------
			if(_flSKR2 == 0) _flSKR2Q = 0;
			if((_flSKR2 == 1) && (_flQ2 == 1)) _flSKR2Q = 1;
			//-----------------------------------------------------------------------------------------------------------
			
            /*
            //-----SKR3 Statemachine--------------------------------------------------------------------------
			//Eingänge verschieben und maskieren
			eingaenge = InputSKR;
			eingaenge = eingaenge >> 4;
			eingaenge &= 0x03;
			switch(sswSKR3)
			{	case 0:	//--------------------------------------------------------------
				//Schritt 0, Zustand nach SLOKoff Wechsel von 1 auf 0
				if(_flPowerOnGrSt3 != 0)
				{	//Grundstellung 0/0 wird nicht verlangt
					switch(eingaenge)
					{	case 0b11:
						_flSKR3 = 1;
						//flQ1 = 1	//Quittierung nicht nach PowerOn setzen
						sswSKR3 = 5;
						break;
						default: //0b00, 0b01, 0b10
						sswSKR3 = 1;
						break;
					}
				}
				else //_flPowerOnGrSt3 == 0
				{	//Grundstellung 0/0 wird verlangt
					//wenn nicht 0/0, dann ErrGrundstellung
					switch(eingaenge)
					{	case 0b00:
						sswSKR3 = 1;
						break;
						default: //01, 10, 11
						SkrErrGrundst |= 0x04;
						//sswSKR3 erst incr. wenn in Grundstellung
						break;
					}
				}
				break;
				case 1:	//--------------------------------------------------------------
				//Schritt 1, _SKR aus, Abfrage Eingänge 00
				switch(eingaenge)
				{	case 0b00:
					sswSKR3 = 2;
					break;
					case 0b11:
					if(_flSkrQSErr3 == 0)
					{	//nach Querschluss würde auch Einkanalerror gesetzt werden
						//Eingänge sind 11, waren aber nicht vorher 00
						_flSkrErr3 = 1;
					}
					break;
					default: //01, 10, 11
					//nicht tun
					break;
				}
				break;
				case 2:	//--------------------------------------------------------------
				//Schritt 2, Grundstellung, _SKR aus, Abfrage Eingänge 11,  10 oder 01
				switch(eingaenge)
				{	case 0b11:
					 cntTOSkr3 = _konstTOSkr;
					_flSKR3 = 1;
					sswSKR3 = 4;
					break;
					case 0b00:
					//nichts tun
					break;
					default: //01, 10
					//cntTOSkr3 = _konstTOSkr; //deaktiviert
					_flSKR3 = 0;
					sswSKR3 = 3;
					break;
				}
				break;
				case 3:	//--------------------------------------------------------------
				//Schritt 3, Zustand 10 oder 01
				//bei 10 oder 01 soll der timeout noch nicht ablaufen
				switch(eingaenge)
				{	case 0b00:
					_flSKR3 = 0;
					sswSKR3 = 1;
					break;
					case 0b11:
					_flSKR3 = 1;
					cntTOSkr3 = _konstTOSkr;
					sswSKR3 = 4;
					break;
					default:
					//nichts tun
					break;
				}
				break;
				case 4:	//--------------------------------------------------------------
				//Schritt 4, Zustand 1/1, _SKR = 1, timeout läuft ab
				//einkanaliges Prellen führt noch nicht zum Abschalten
				if(cntTOSkr3 != 0)
				{	cntTOSkr3 --;
					switch(eingaenge)
					{	case 0b00:
						_flSKR3 = 0;
						sswSKR3 = 1;
						break;
						default:	//01, 10, 11
						//mindestens 1 Eingang auf 1, abwarten
						break;
					}
				}
				else	//cnt TOSkr3 == 0
				{	switch(eingaenge)
					{	case 0b11:
						_flSKR3 = 1;
						sswSKR3 = 5;
						break;
						default:	//00, 01, 10
						_flSKR3 = 0;
						sswSKR3 = 1;
						break;
					}
				}
				break;
				case 5:	//--------------------------------------------------------------
				//Schritt 5, Zustand 1/1, _SKR = 1, ungleich 1/1 führt zum Abschalten
				switch(eingaenge)
				{	case 0b11:
					_flSKR3 = 1;
					//ok, abwarten
					break;
					default:
					_flSKR3 = 0;
					sswSKR3 = 1;
					break;
				}
				break;
			}
			//-----------------------------------------------------------------
			if(_flSKR3 == 0) _flSKR3Q = 0;
			if((_flSKR3 == 1) && (_flQ3 == 1)) _flSKR3Q = 1;
			//-----------------------------------------------------------------------------------------------------------
			//-----SKR4 Statemachine--------------------------------------------------------------------------
			//Eingänge verschieben und maskieren
			eingaenge = InputSKR;
			eingaenge = eingaenge >> 6;
			eingaenge &= 0x03;
			switch(sswSKR4)
			{	case 0:	//--------------------------------------------------------------
				//Schritt 0, Zustand nach SLOKoff Wechsel von 1 auf 0
				if(_flPowerOnGrSt4 != 0)
				{	//Grundstellung 0/0 wird nicht verlangt
					switch(eingaenge)
					{	case 0b11:
						_flSKR4 = 1;
						//flQ1 = 1	//Quittierung nicht nach PowerOn setzen
						sswSKR4 = 5;
						break;
						default: //0b00, 0b01, 0b10
						sswSKR4 = 1;
						break;
					}
				}
				else //_flPowerOnGrSt4 == 0
				{	//Grundstellung 0/0 wird verlangt
					//wenn nicht 0/0, dann ErrGrundstellung
					switch(eingaenge)
					{	case 0b00:
						sswSKR4 = 1;
						break;
						default: //01, 10, 11
						SkrErrGrundst |= 0x08;
						//sswSKR4 erst incr. wenn in Grundstellung
						break;
					}
				}
				break;
				case 1:	//--------------------------------------------------------------
				//Schritt 1, _SKR aus, Abfrage Eingänge 00
				switch(eingaenge)
				{	case 0b00:
					sswSKR4 = 2;
					break;
					case 0b11:
					if(_flSkrQSErr4 == 0)
					{	//nach Querschluss würde auch Einkanalerror gesetzt werden
						//Eingänge sind 11, waren aber nicht vorher 00
						_flSkrErr4 = 1;
					}
					break;
					default: //01, 10, 11
					//nicht tun
					break;
				}
				break;
				case 2:	//--------------------------------------------------------------
				//Schritt 2, Grundstellung, _SKR aus, Abfrage Eingänge 11,  10 oder 01
				switch(eingaenge)
				{	case 0b11:
					 cntTOSkr4 = _konstTOSkr;
					_flSKR4 = 1;
					sswSKR4 = 4;
					break;
					case 0b00:
					//nichts tun
					break;
					default: //01, 10
					//cntTOSkr4 = _konstTOSkr; //deaktiviert
					_flSKR4 = 0;
					sswSKR4 = 3;
					break;
				}
				break;
				case 3:	//--------------------------------------------------------------
				//Schritt 3, Zustand 10 oder 01
				//bei 10 oder 01 soll der timeout noch nicht ablaufen
				switch(eingaenge)
				{	case 0b00:
					_flSKR4 = 0;
					sswSKR4 = 1;
					break;
					case 0b11:
					_flSKR4 = 1;
					cntTOSkr4 = _konstTOSkr;
					sswSKR4 = 4;
					break;
					default:
					//nichts tun
					break;
				}
				break;
				case 4:	//--------------------------------------------------------------
				//Schritt 4, Zustand 1/1, _SKR = 1, timeout läuft ab
				//einkanaliges Prellen führt noch nicht zum Abschalten
				if(cntTOSkr4 != 0)
				{	cntTOSkr4 --;
					switch(eingaenge)
					{	case 0b00:
						_flSKR4 = 0;
						sswSKR4 = 1;
						break;
						default:	//01, 10, 11
						//mindestens 1 Eingang auf 1, abwarten
						break;
					}
				}
				else	//cnt TOSkr4 == 0
				{	switch(eingaenge)
					{	case 0b11:
						_flSKR4 = 1;
						sswSKR4 = 5;
						break;
						default:	//00, 01, 10
						_flSKR4 = 0;
						sswSKR4 = 1;
						break;
					}
				}
				break;
				case 5:	//--------------------------------------------------------------
				//Schritt 5, Zustand 1/1, _SKR = 1, ungleich 1/1 führt zum Abschalten
				switch(eingaenge)
				{	case 0b11:
					_flSKR4 = 1;
					//ok, abwarten
					break;
					default:
					_flSKR4 = 0;
					sswSKR4 = 1;
					break;
				}
				break;
			}
			//-----------------------------------------------------------------
			if(_flSKR4 == 0) _flSKR4Q = 0;
			if((_flSKR4 == 1) && (_flQ4 == 1)) _flSKR4Q = 1;
			//-----------------------------------------------------------------------------------------------------------
			//alle 4 SKR abgearbeitet. Ergebnis auf IPK schreiben
            */
            //beide SKR wurden abgearbeitet, Ergebnis auf IPK schreiben
			//SerTX[0] &= 0xF0;
            if(_flSkrAktiv1 != 0)
            {   if(_flSKR1Q == 1) TX.bf.SKR1 = 1;
                else              TX.bf.SKR1 = 0;
            }
            else                  TX.bf.SKR1 = 0;
            if(_flSkrAktiv2 != 0)
            {   if(_flSKR2Q == 1) TX.bf.SKR2 = 1;          //SerTX[0] |= 0x02;
                else              TX.bf.SKR2 = 0;
            }
            else                  TX.bf.SKR2 = 0;
            /*
            if(_flSkrAktiv3 != 0)
            {   if(_flSKR3Q == 1) TX.bf.SKR3 = 1;
                else              TX.bf.SKR3 = 0;
            }
            else                  TX.bf.SKR3 = 0;
            if(_flSkrAktiv4 != 0)
            {   if(_flSKR4Q == 1) TX.bf.SKR4 = 1;          //SerTX[0] |= 0x02;
                else              TX.bf.SKR4 = 0;
            }
            else                  TX.bf.SKR4 = 0;
            */
			//SKR Bits0-3 auf CAN Buffer schreiben, Bits 4-7 sind nicht betroffen
			CanTX[2] &= 0x0F;
			//CanTX[2] |= (TX.Bytes[3] & RX.Bytes[3] & 0x30); //###test#####################
            CanTX[2] |= (TX.Bytes[3] & 0x30);
		} //cntDelSkrAusw == 0
	} //kein Error und ParamKomplett
} //------Ende SkrAuswerten------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------
//	Zweihandschaltung(), wird alle 10ms aufgerufen
//---------------------------------------------------------------------------------------------------------------------------
void ZwHdAuswerten(void)
{	uint8_t eingaenge;
    static uint8_t ZHstep = 0, cntZHTimeOut = 0;
	cnt10msRoutinen ++;

    if(((CanRX[0] & 0x80) != 0)
    || (flParamKomplett == 0)
    || (_flZweihandAktiv == 0))
    {   
		TX.bf.ZH = 0;        //ZH bit0 löschen
        CanTX[2] &= 0xFE;    //ZH bit0 löschen
		ZHstep = 0;			 //zurück zur Grundstellung
		cntZHTimeOut = 0;      
    }
    else        
	{	eingaenge = TX.Bytes[1] & 0x0F;		//selected Input
		if(ZHstep > 2) ZHstep = 0;
		switch(ZHstep)
		{	case 0:
				if(eingaenge == 0x05)
				{	TX.bf.ZH = 0;          // ZH bit0 löschen
					ZHstep = 1;
				}
				else
				{	//auf Grundstellung 0x05 warten
					TX.bf.ZH = 0;          // ZH bit0 löschen
				}
				break;
			case 1:
				if(eingaenge == 0x05)
				{	TX.bf.ZH = 0;          // ZH bit0 löschen, warten
				}
				else if(eingaenge == 0x0A)
				{	//Zweihandschalter wurde betätigt
					TX.bf.ZH = 1;	//ZH bit0 setzen
					cntZHTimeOut = 0;
					ZHstep = 2;
				}
				else
				{	//Grundstellung 0x05 wurde verlassen, TimeOut läuft ab
					cntZHTimeOut ++;
					if(cntZHTimeOut > 51)
					{	ZHstep = 0;
						cntZHTimeOut = 0;
						TX.bf.ZH = 0;	//ZH bit0 löschen
					}
				}
				break;
			case 2:
				if(eingaenge == 0x0A)
				{	; //ok warten
				}
				else
				{	// != 0x0A
					TX.bf.ZH = 0;          // ZH bit0 löschen
					ZHstep = 0;			//zurück zur Grundstellung
					cntZHTimeOut = 0;
				}
				break;
		}
        //if((TX.bf.ZH == 1)&&(RX.bf.ZH == 1)) CanTX[2] |= 0x01;   //####test###########ZH bit0 setzen
        if(TX.bf.ZH == 1) CanTX[2] |= 0x01;     //ZH bit0 setzen   //###test##########################
		else              CanTX[2] &= 0xFE;     //ZH bit0 löschen
	}
}
//-------Ende Zweihandschaltung()-------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------
//QSchlErkSKR() wird alle 1ms aufgerufen von debounceInput4ms().
//--------------------------------------------------------------------------------------------------------------------------
void QSchlErkSKR(uint8_t Eingaenge)
{   
    uint8_t error = 0;

    //Querschlusserkennung SKR1 //------------------------------------------------------------------
    error = 0;
    if((_flQSErkenn1 != 0) && (sswSKR1 == 5))
    {   //Querschlusserkennung erforderlich, SKR on Zustand
        if((Eingaenge & 0x03) == 0) //---------------------00--------------------------------
        {   if((PS.Par.SkrAntival & 0x01) != 0)
            {   //bei SKR antivalent ist low/low kein Abschaltgrund
                cntQuerschlSkr1a = 0;
                cntQuerschlSkr1b = 0;              
            }
            //Zustand 00, über cnt verzögert abschalten
            //nur alle 100ms incrementieren
            if(fl100ms == 1)  
            {   if(cntQuerschlSkr1b < konstQuerschluss)  cntQuerschlSkr1b ++;
                if(cntQuerschlSkr1b >= konstQuerschluss) error = 1;               
            }
        }
        else
        {   //Eingänge sind nicht 00
            if((Eingaenge & 0x01) == 0) //--------------------x0---------------------------------
            {   cntQuerschlSkr1a = 0;            
            }
            else  //E1 == 1 ----------------------------------------x1---------------------------------
            {   //nur alle 100ms incrementieren
                if(fl100ms == 1)  
                {  if(cntQuerschlSkr1a < konstQuerschluss)  cntQuerschlSkr1a ++;
                    if(cntQuerschlSkr1a >= konstQuerschluss) error = 1;   
                }           
            }
            if((Eingaenge & 0x02) == 0) //----------------------0x-------------------------------
            {   cntQuerschlSkr1b = 0;          
            }
            else//E2 == 1 --------------------------------------------1x------------------------------
            {   //nur alle 100ms incrementieren
                if(fl100ms == 1)  
                {   if(cntQuerschlSkr1b < konstQuerschluss)  cntQuerschlSkr1b ++;
                    if(cntQuerschlSkr1b >= konstQuerschluss) error = 1;               
                }         
            }
        }
        if(error != 0)
        {   //SKR abschalten
            sswSKR1 = 1;
            cntQdelay1 = 0;
            cntTOSkr1 = 0;
            _flSKR1 = 0;
            _flSKR1Q = 0;
            _flQpos1 = 0;
            _flQ1 = 0;
            _flSkrQSErr1 = 1;    
        }       
    }
    else    //Querschlusserkennung nicht erforderlich oder SKR im off Zustand
    {   cntQuerschlSkr1a = 0;
        cntQuerschlSkr1b = 0;
    }
    
    //Querschlusserkennung SKR2  //------------------------------------------------------------------
    error = 0;
    if((_flQSErkenn2 != 0) && (sswSKR2 == 5))
    {   //Querschlusserkennung erforderlich, SKR on Zustand
        if((Eingaenge & 0x0C) == 0) //------------------00-----------------------------------
        {   if((PS.Par.SkrAntival & 0x02) != 0)
            {   //bei SKR antivalent ist low/low kein Abschaltgrund
                cntQuerschlSkr2a = 0;
                cntQuerschlSkr2b = 0;              
            }
            //Zustand 00, über cnt verzögert abschalten
            //nur alle 100ms incrementieren
            if(fl100ms == 1)  
            {   if(cntQuerschlSkr2b < konstQuerschluss)  cntQuerschlSkr2b ++;
                if(cntQuerschlSkr2b >= konstQuerschluss) error = 1;               
            }
        }
        else
        {   //Eingänge sind nicht 00
            if((Eingaenge & 0x04) == 0) //------------------x0-----------------------------------
            {   cntQuerschlSkr2a = 0;            
            }
            else  //E1 == 1 --------------------------------------x1-----------------------------------
            {   //nur alle 100ms incrementieren
                if(fl100ms == 1)  
                {  if(cntQuerschlSkr2a < konstQuerschluss)  cntQuerschlSkr2a ++;
                    if(cntQuerschlSkr2a >= konstQuerschluss) error = 1;   
                }           
            }
            if((Eingaenge & 0x08) == 0) //------------------0x-----------------------------------
            {   cntQuerschlSkr2b = 0;          
            }
            else//E2 == 1 ----------------------------------------01----------------------------------
            {   //nur alle 100ms incrementieren
                if(fl100ms == 1)  
                {   if(cntQuerschlSkr2b < konstQuerschluss)  cntQuerschlSkr2b ++;
                    if(cntQuerschlSkr2b >= konstQuerschluss) error = 1;               
                }         
            }
        }
        if(error != 0)
        {   //SKR abschalten
            sswSKR2 = 1;
            cntQdelay2 = 0;
            cntTOSkr2 = 0;
            _flSKR2 = 0;
            _flSKR2Q = 0;
            _flQpos2 = 0;
            _flQ2 = 0;
            _flSkrQSErr2 = 1;  
        }       
    }
    else    //Querschlusserkennung nicht erforderlich oder SKR im off Zustand
    {   cntQuerschlSkr2a = 0;
        cntQuerschlSkr2b = 0;
    }
    /*
    //Querschlusserkennung SKR3  //------------------------------------------------------------------
    error = 0;
    if((_flQSErkenn3 != 0) && (sswSKR3 == 5))
    {   //Querschlusserkennung erforderlich, SKR on Zustand
        if((Eingaenge & 0x30) == 0) //------------------00-----------------------------------
        {   if((PS.Par.SkrAntival & 0x04) != 0)
            {   //bei SKR antivalent ist low/low kein Abschaltgrund
                cntQuerschlSkr3a = 0;
                cntQuerschlSkr3b = 0;              
            }
            //Zustand 00, über cnt verzögert abschalten
            //nur alle 100ms incrementieren
            if(fl100ms == 1)  
            {   if(cntQuerschlSkr3b < konstQuerschluss)  cntQuerschlSkr3b ++;
                if(cntQuerschlSkr3b >= konstQuerschluss) error = 1;               
            }
        }
        else
        {   //Eingänge sind nicht 00
            if((Eingaenge & 0x10) == 0) //------------------x0-----------------------------------
            {   cntQuerschlSkr3a = 0;            
            }
            else  //E1 == 1 --------------------------------------x1-----------------------------------
            {   //nur alle 100ms incrementieren
                if(fl100ms == 1)  
                {  if(cntQuerschlSkr3a < konstQuerschluss)  cntQuerschlSkr3a ++;
                    if(cntQuerschlSkr3a >= konstQuerschluss) error = 1;   
                }           
            }
            if((Eingaenge & 0x20) == 0) //------------------0x-----------------------------------
            {   cntQuerschlSkr3b = 0;          
            }
            else//E2 == 1 ----------------------------------------01----------------------------------
            {   //nur alle 100ms incrementieren
                if(fl100ms == 1)  
                {   if(cntQuerschlSkr3b < konstQuerschluss)  cntQuerschlSkr3b ++;
                    if(cntQuerschlSkr3b >= konstQuerschluss) error = 1;               
                }         
            }
        }
        if(error != 0)
        {   //SKR abschalten
            sswSKR3 = 1;
            cntQdelay3 = 0;
            cntTOSkr3 = 0;
            _flSKR3 = 0;
            _flSKR3Q = 0;
            _flQpos3 = 0;
            _flQ3 = 0;
            _flSkrQSErr3 = 1;           
        }       
    }
    else    //Querschlusserkennung nicht erforderlich oder SKR im off Zustand
    {   cntQuerschlSkr3a = 0;
        cntQuerschlSkr3b = 0;
    }
    
    
    //Querschlusserkennung SKR4  //------------------------------------------------------------------
    error = 0;
    if((_flQSErkenn4 != 0) && (sswSKR4 == 5))
    {   //Querschlusserkennung erforderlich, SKR on Zustand
        if((Eingaenge & 0xC0) == 0) //------------------00-----------------------------------
        {   if((PS.Par.SkrAntival & 0x08) != 0)
            {   //bei SKR antivalent ist low/low kein Abschaltgrund
                cntQuerschlSkr4a = 0;
                cntQuerschlSkr4b = 0;              
            }
            //Zustand 00, über cnt verzögert abschalten
            //nur alle 100ms incrementieren
            if(fl100ms == 1)  
            {   if(cntQuerschlSkr4b < konstQuerschluss)  cntQuerschlSkr4b ++;
                if(cntQuerschlSkr4b >= konstQuerschluss) error = 1;               
            }
        }
        else
        {   //Eingänge sind nicht 00
            if((Eingaenge & 0x40) == 0) //------------------x0-----------------------------------
            {   cntQuerschlSkr4a = 0;            
            }
            else  //E1 == 1 --------------------------------------x1-----------------------------------
            {   //nur alle 100ms incrementieren
                if(fl100ms == 1)  
                {  if(cntQuerschlSkr4a < konstQuerschluss)  cntQuerschlSkr4a ++;
                    if(cntQuerschlSkr4a >= konstQuerschluss) error = 1;   
                }           
            }
            if((Eingaenge & 0x80) == 0) //------------------0x-----------------------------------
            {   cntQuerschlSkr4b = 0;          
            }
            else//E2 == 1 ----------------------------------------01----------------------------------
            {   //nur alle 100ms incrementieren
                if(fl100ms == 1)  
                {   if(cntQuerschlSkr4b < konstQuerschluss)  cntQuerschlSkr4b ++;
                    if(cntQuerschlSkr4b >= konstQuerschluss) error = 1;               
                }         
            }
        }
        if(error != 0)
        {   //SKR abschalten
            sswSKR4 = 1;
            cntQdelay4 = 0;
            cntTOSkr4 = 0;
            _flSKR4 = 0;
            _flSKR4Q = 0;
            _flQpos4 = 0;
            _flQ4 = 0;
            _flSkrQSErr4 = 1;           
        }       
    }
    else    //Querschlusserkennung nicht erforderlich oder SKR im off Zustand
    {   cntQuerschlSkr4a = 0;
        cntQuerschlSkr4b = 0;
    }
    */
}