#include <xc.h>
#include "p33CK256MP505.h"
#include "ADC.h"
#include "timing.h"
#include <stdint.h>

volatile uint16_t ADC1V65_IR = 0, ADC3V3_IR = 0, ADC12V_IR = 0, ADC_DrBrSp_IR = 0;
uint16_t          ADC1V65 = 0,    ADC3V3 = 0,    ADC12V = 0,    ADC_DrBrSp = 0;
volatile uint16_t EMK_IR = 0, EMK_avg_IR = 0;
uint16_t          EMK_avg = 0;
volatile uint16_t  EMK_Peak_IR = 0; 
uint16_t           EMK_Peak = 0;    
volatile uint16_t PhaseV_1_IR = 0,PhaseV_2_IR = 0,PhaseV_3_IR = 0,PhaseV_4_IR = 0;
volatile uint16_t  buffV_1_IR = 0, buffV_2_IR = 0, buffV_3_IR = 0, buffV_4_IR = 0;
uint16_t          PhaseV_1 = 0,   PhaseV_2 = 0,   PhaseV_3 = 0,   PhaseV_4 = 0;
volatile uint16_t DiffVerst_IR = 0;
uint16_t          DiffVerst = 0;
volatile uint8_t ADCgetriggert_IR = 0;
uint8_t          ADCgetriggert = 0;
uint16_t ADCavg = 0, avg20msADC = 0;
uint16_t EmkDelta = 0;

uint8_t cntErr3V3Test = 0, flErr3V3Test = 0, cntErrEncVers = 0, flErrEncVers = 0; 
uint8_t cntErr1V65Test = 0, flErr1V65Test = 0, cntErr12VTest = 0, flErr12VTest = 0;
uint8_t cntDrahtBruchErr = 0, flDrahtBruchErr = 0;
uint8_t cntDrBrSpLowErr = 0, cntDrBrSpHighErr = 0, flDrBrSpannung = 0;
uint8_t cntAdcFrqErr = 0, flAdcFrqErr = 0, cntEmkPeak = 0;
uint8_t EmkStatus = 0xFF;
uint16_t cntLowPegel = 0, cntHighPegel = 0, cntMotorRuns = 0;
uint8_t cntErrGleichePhase = 0,	flErrGleichePhase = 0;
uint8_t cntDiffErr = 0, flDiffErr = 0;
uint8_t cntErrTriggerADC = 0, flErrTriggerADC = 0;

//Erläuterungen zur DN3PD2- und DN3PD2-Auswertung der analogen ADC Werte.
//DN3PS2 EMK Auswertung zur Stillstandsüberwachung:
//Alle 1ms wird der Durchschnitt aus 16 ADC Messungen (ADC-ISR) und ein Peakwert übernommen. 
//Mit dem Durchschnittswert wird während 50ms Minimum und Maximum und das Delta=Max-Min ermittelt.
//Delta --> Delta-Vergleich --> EmkStatus --> Relais schalten.
//Ist der Peakwert 50x <0,5V oder >4,5V, dann wird das Relais abgeschaltet.
//Die Drahtbrucherkennung arbeitet mit statischen Werten.
//Der ADC Durchschnittswert wird bein PDSV aus 8 anstatt 16 Werten gebildet.

//DN3PD2 analoge Auswertung:
//Die ADC ISR liefert liefert alle 1m 4 ADC-Werte, die während 150µ im Abstand von 50µs eingelesen werden.
//Die analogeFrqDetection() arbeitet mit Kopien dieser 4 ADC-Werte.
//In analogeFrqDetection() wird alle 1ms der Durchschnitt der 4 ADC-Werten gebildet.
//Außerdem wird über 20ms aus dem 1ms-Durchschnitt ein 20ms-Durchschnitt gebildet (avg20msADC-Wert).
//Die DrahtbruchErkennung() arbeitet dynamisch. Mit der Freq. von 2,94Hz wird 5V/0V auf
//die 1M Drahtbruchwiderstände geschaltet. Das Tastverhältnis ist unsymetrisch 260ms(5V) und 80ms(0V).
//Die DrahtbruchErkennung() wertet den avg20msADC-Wert aus.


//------------------------------------------------------------------------------
//analogFrqDetection() wird alle 0,5ms aufgerufen
//Alle 500µs werden während 150µs 4 Messungen im Abstand von 50µs vom ADC durchgeführt.
//Alle 500µs liegen 4 neue ADC Werte vor und werden aus dem Buffer kopiert.
//Der kleinste und der größte Wert wird zur Auswertung verwendet.
//Nach 400x0,5ms=200ms werden die Messungen ausgewertet.
//Nach 200ms werden SpannungSS genullt. 
//------------------------------------------------------------------------------
void analogFrqDetection(void)
{	uint16_t minSample = 0, maxSample = 0;
	static uint16_t minValue = 0x3FF, maxValue = 0, SpannungSS = 0;
	static uint16_t cntSamples = 0;
	static uint8_t cnt20ms = 0;
    static uint16_t sum20msADC = 0; 
    uint16_t portC = 0;
    static uint16_t cntGleichePhase = 0;
	//static unsigned int cntCh2Low = 0;
	static uint32_t GleichePhase = 0, cntCh1Low = 0, cntCh2Low = 0;
	static uint8_t stmPhasenGleichheit = 0;
	uint8_t ProzentGleichheit = 0, Ch1Tastverh = 0, Ch2Tastverh = 0;

	if(fl1ms == 1) cnt1msRoutinen ++;
	//------------------------------L1------------------------------------------
	__builtin_disi(50);
   	INTCON2bits.GIE = 0; // IR's gesperrt
    ADC1V65 = ADC1V65_IR;
    ADC3V3 = ADC3V3_IR;
    ADC12V = ADC12V_IR;
    ADC_DrBrSp = ADC_DrBrSp_IR;
    EMK_Peak = EMK_Peak_IR;         
    EMK_avg = EMK_avg_IR;
    PhaseV_1 = PhaseV_1_IR;
    PhaseV_2 = PhaseV_2_IR;
    PhaseV_3 = PhaseV_3_IR;
    PhaseV_4 = PhaseV_4_IR;
    DiffVerst = DiffVerst_IR;
    ADCgetriggert = ADCgetriggert_IR;
    //Der ADC wird neu getriggert. Beim nächsten analogFrqDetection() Aufruf liegen wieder neue ADC Werte vor.
    //ADCgetriggert wird durch den ADC IR gelöscht, wenn nicht 0, dann ist der ADC scan noch nicht fertig.
    if(ADCgetriggert_IR == 0)
    {   ADCON3Lbits.SWCTRG = 1;   //triggern des Scan
        ADCgetriggert_IR = 1;
    }
    //Pegel für Phasenüberwachung einlesen
    portC = PORTC;
	__builtin_disi(0);
   	INTCON2bits.GIE = 1; // IR's enabled
    //--------------------------------------------------------------------------
    if(ADCgetriggert == 1)
    {   //ADCgetriggert_IR wurde nicht im ADC IR gelöscht, Fehler
        cntErrTriggerADC ++;
        if(cntErrTriggerADC > 20) flErrTriggerADC = 1;
    }
    else
    {   cntErrTriggerADC = 0;       
    }
    
   //Wenn PhasenlageModus == 1 und die ParameterKomplett sind, dann Phasenlage prüfen
	if((PS.Par.PhasenUW != 0)&&(flParamKomplett == 1))
	{	//Auf PhasenGleicheit prüfen--------------------------------------------
		if((Periode < k5HzPeriode)&&(Periode > k130HzPeriode)&&(cntTimeOut1S > 70))
		{	//> 5Hz und < 130Hz, 1s-Timeout == 0
			switch(stmPhasenGleichheit)
			{	case 0: //------------------------------------------------------
				if(flNeueFlanke == 0)
				{	cntGleichePhase = 0;
					GleichePhase = 0;
					cntCh1Low = 0;
					cntCh2Low = 0;
					cntErrGleichePhase = 0;
				}
				else
				{	//flNeueFlanke == 1, Start des Einlesens in "case 1"
					flNeueFlanke = 0;
					cntGleichePhase = 0;
					GleichePhase = 0;
					cntCh1Low = 0;
					cntCh2Low = 0;
					cntErrGleichePhase = 0;
					stmPhasenGleichheit = 1;
				}
				break;
	
				case 1: //------------------------------------------------------------------------------------------
				if(flNeueFlanke == 0)
				{	//Einlesen bis zur nächsten Flanke
					cntGleichePhase ++;
					if(cntGleichePhase > 1000)
					{	cntGleichePhase = 0;
						GleichePhase = 0;
						cntErrGleichePhase = 0;
						stmPhasenGleichheit = 0;
					}
					else
					{	//cntGleichePhase <= 1000. Sind RC10 und RC11 gleich?
						if((portC & 0x800)==0) cntCh2Low ++;	//bei Low wird incr., bei High nicht
						if((portC & 0x400)==0) cntCh1Low ++;
						if(((portC & 0xC00)==0)||((portC & 0xC00)==0xC00)) GleichePhase ++;
					}
				}
				else
				{	//flNeueFlanke == 1
					flNeueFlanke = 0;
					cntGleichePhase ++;
					if((portC & 0x800)==0) cntCh2Low ++;	//bei Low wird incr., bei High nicht
					if((portC & 0x400)==0) cntCh1Low ++;
					if(((portC & 0xC00)==0)||((portC & 0xC00)==0xC00)) GleichePhase ++;
					//Auswertung....
					if(cntGleichePhase > 0)
					{	//Die Berechnung Multiplikation, Division benötigt 57µs. Multiplikation ersetzt durch << 7 52µs.
						ProzentGleichheit = (GleichePhase * 100) / cntGleichePhase;	    //die Berechnung benötigt 57µs
						if((ProzentGleichheit < 50)||(ProzentGleichheit > 79)	
						||(cntCh2Low == 0)||(cntCh2Low == cntGleichePhase))		//alle samples vom 2. Kanal sind Low oder alle High
						{	cntErrGleichePhase += 3;
						}
						else
						{	if(cntErrGleichePhase > 0) cntErrGleichePhase --;
						}
						//Error nach 6x Gleichheit < 50%, > 79%
						if(cntErrGleichePhase > 17)	flErrGleichePhase = 1;
						cntGleichePhase = 0;
						GleichePhase = 0;
						cntCh1Low = 0;
						cntCh2Low = 0;					
					}
					else //cntGleichePhase == 0, keine Division möglich, Fehler, zurück auf case 0.
					{	cntGleichePhase = 0;
						GleichePhase = 0;
						cntCh1Low = 0;
						cntCh2Low = 0;
						cntErrGleichePhase = 0;
						stmPhasenGleichheit = 0;
					}		
				}
				break;
			} //switch(stmPhasenGleichheit) Ende -------------------------------
		}
		else	//außerhalb des Frequenzbereichs 2Hz bis 100Hz, oder 1s-Timeout
		{	//alles zurücksetzen
			GleichePhase = 0;
			cntGleichePhase = 0;
			cntCh1Low = 0;
			cntCh2Low = 0;	
			cntErrGleichePhase = 0;
			ProzentGleichheit = 0;
			Ch1Tastverh = 0;
			Ch2Tastverh = 0;
			flNeueFlanke = 0;
			stmPhasenGleichheit = 0;
			//Einrastend, Fehler nicht löschen. flErrGleichePhase = 0;
		}
	} 
	else
	{	//PhasenlageModus == 0, alles zurücksetzen
		GleichePhase = 0;
		cntGleichePhase = 0;
		cntCh1Low = 0;
		cntCh2Low = 0;	
		cntErrGleichePhase = 0;
		ProzentGleichheit = 0;
		Ch1Tastverh = 0;
		Ch2Tastverh = 0;
		flNeueFlanke = 0;
		stmPhasenGleichheit = 0;
		flErrGleichePhase = 0;
	}	//Auf PhasenGleicheit prüfen Ende--------------------------------------- 

	//min/max der 4 neuen ADCWerte ermitteln
	minSample = PhaseV_1;
	if(PhaseV_2 < minSample) minSample = PhaseV_2;
	if(PhaseV_3 < minSample) minSample = PhaseV_3;
	if(PhaseV_4 < minSample) minSample = PhaseV_4;
	maxSample = PhaseV_1;
	if(PhaseV_2 > maxSample) maxSample = PhaseV_2;
	if(PhaseV_3 > maxSample) maxSample = PhaseV_3;
	if(PhaseV_4 > maxSample) maxSample = PhaseV_4;
	//In minSample steht nun der kleinste Wert, in maxSample der größte Wert

	if(cntSamples < 400) cntSamples ++;
	//minValue/maxValue über längere Zeit ermitteln
	if(minSample < minValue) minValue = minSample;
	if(maxSample > maxValue) maxValue = maxSample;
	
	if(cntSamples >= 400)
	{	 //cntSamples >= 400, jetzt kommt die eigentliche Auswertung der gesammelten Daten
		cntSamples = 0;
		SpannungSS = maxValue - minValue;
		
		if(cntTimeOut1S == 0)
		{	//Signal Timeout, es kommen seit 1s keine Impulse mehr. Analoge Signal Detection?
			switch(StatusSTHyst)
			{	case Hyst_375mV:
				if(SpannungSS > 156) cntAdcFrqErr += 2;		//3,3V/255x39=0,5V. 39=0,5Vx255/3,3V. 155=0,5Vx1023/3,3V. Da müsste der ST schalten
				else	if(cntAdcFrqErr > 0) cntAdcFrqErr --;
				break;

				case Hyst_630mV:
				if(SpannungSS > 246) cntAdcFrqErr += 2;		//3,3V/255x61=0,79V. 61=0,79Vx255/3,3V. 245=0,79x1023/3,3V. Da müsste der ST schalten
				else	if(cntAdcFrqErr > 0) cntAdcFrqErr --;
				break;

				case Hyst_1V33:
				if(SpannungSS > 513) cntAdcFrqErr += 2;		//3,3V/255x128=1,65V.128=1,65Vx255/3,3V. 512=1,65Vx1023/3,3V. Da müsste der ST schalten
				else	if(cntAdcFrqErr > 0) cntAdcFrqErr --;
				break;

				case Hyst_1V88:
				if(SpannungSS > 733) cntAdcFrqErr += 2;		//3,3V/255x182=2,36V. 182=2,36Vx255/3,3V. 732=2,36Vx1023/3,3V. Da müsste der ST schalten
				else	if(cntAdcFrqErr > 0) cntAdcFrqErr --;
				break;
			}
		}
		else if((cntTimeOut1S > 80)&&(IstFrqX10 > 200)&&(IstFrqX10 < 10000))	//Frequenzbereich > 20Hz und < 1000Hz
		{	//Das Signal wird gemessen. cntTimeOut1s > 80: Entspricht > 5Hz bzw. <200ms. 
			switch(StatusSTHyst)
			{	case Hyst_375mV:
				if(SpannungSS < 80) cntAdcFrqErr += 2;		//3,3V/255x20=0,26V. 20=0,26Vx255/3,3V. 81=0,26Vx1023/3,3V. Wäre eine zu kleine Spannung
				else	if(cntAdcFrqErr > 0) cntAdcFrqErr --;
				break;

				case Hyst_630mV:
				if(SpannungSS < 151) cntAdcFrqErr += 2;		//3,3V/255x38=0,49V. 38=0,49Vx255/3,3V. 152=0,49Vx1023/3,3V. Wäre eine zu kleine Spannung
				else	if(cntAdcFrqErr > 0) cntAdcFrqErr --;
				break;

				case Hyst_1V33:
				if(SpannungSS < 325) cntAdcFrqErr += 2;		//3,3V/255x81=1,05V. 81=1,05Vx255/3,3V. 326=1,05Vx1023/3,3V. Wäre eine zu kleine Spannung
				else	if(cntAdcFrqErr > 0) cntAdcFrqErr --;
				break;

				case Hyst_1V88:
				if(SpannungSS < 464) cntAdcFrqErr += 2;		//3,3V/255x116=1,5V. 116=1,5Vx255/3,3V. 465=1,5x1023/3,3V. Wäre eine zu kleine Spannung
				else	if(cntAdcFrqErr > 0) cntAdcFrqErr --;
				break;
			}
		}
		else	//alle anderen Fälle
		{	if(cntAdcFrqErr > 0) cntAdcFrqErr --;
		}
		
		if(cntAdcFrqErr > kErrAnalogSignalDet) flAdcFrqErr = 1;		//Konstante entspricht 3s

		//------------------------------------------------------------------
		//nach Fehlererkennung/Auswertung Werte löschen
		SpannungSS = 0;
		maxValue = 0;
		minValue = 0x3FF;
	}

    //Für die DrahtbruchErkennung():
	//Jede 1ms Durchschnitt aus den aktuellen 4 Werten bilden,
	//und dann den Durchschnitt 20x aufsummieren und durch 20 teilen.
    //Die Werte von PhaseV_1 bis...4 usw. sind auf 10Bit (0x3FF) beschränkt.
    //Siehe _ADCInterrupt(). Ein Überlauf kommt nicht zustande (20 x 1023 = 20460).
	if(fl1ms == 1)
	{	ADCavg = (((uint16_t)PhaseV_1 + (uint16_t)PhaseV_2 + (uint16_t)PhaseV_3 + (uint16_t)PhaseV_4) >> 2);        
        sum20msADC += ADCavg;       
        cnt20ms ++;      
		if(cnt20ms == 20)
		{	avg20msADC = (sum20msADC / 20);
			sum20msADC = 0;
			cnt20ms = 0;
		}    
	}
}
//----Ende analogFrqDetection()-------------------------------------------------


//------------------------------------------------------------------------------
//stmEMK() wird alle 1ms aufgerufen
//Eine Statemachine um die Drahtbrucherkennung und EMK Auswertung zu synchronisieren
//Von hier wird DrahtbruchErkennung() und getEMK() aufgerufen
//------------------------------------------------------------------------------
void stm_EMK_DrBrErk(void)
{   static uint8_t stmEMK = 0;
    static uint16_t stateCnt = 0;
    
    switch(stmEMK)
    {
        case 0: //----------------------------------------------------20ms Block
        stateCnt ++;
        getEmkSpannung(stateCnt, 1);
        if(stateCnt == 15) DrahtbruchErkennung(0);  //Am Ende der DrBrSignal Low Phase, -> Beginn High Phase
        if(stateCnt == 20)
        {   stateCnt = 0;
            stmEMK = 1;
        }
        break;
        case 1: //----------------------------------------------------50ms Block
        stateCnt ++;
        getEmkSpannung(stateCnt, 0); 
        //if((stateCnt==8)||(stateCnt==18)||(stateCnt==28)||(stateCnt==38)||(stateCnt==48))
        //{   checkDrBrSpannung(1);           //Wird nicht benötigt
        //}
        if(stateCnt == 50)
        {   stateCnt = 0;
            stmEMK = 2;
        }
        break;
        case 2: //----------------------------------------------------50ms Block
        stateCnt ++;
        getEmkSpannung(stateCnt, 0);
        if(stateCnt == 50)
        {   stateCnt = 0;
            stmEMK = 3;
        }
        break;
        case 3: //----------------------------------------------------50ms Block
        stateCnt ++;
        getEmkSpannung(stateCnt, 0);
        if(stateCnt == 50)
        {   stateCnt = 0;
            stmEMK = 4;
        }
        break;
        case 4: //----------------------------------------------------50ms Block
        stateCnt ++;
        getEmkSpannung(stateCnt, 0);
        if(stateCnt == 50)
        {   stateCnt = 0;
            stmEMK = 5;
        } 
        break;
        case 5: //----------------------------------------------------50ms Block
        stateCnt ++;
        getEmkSpannung(stateCnt, 0);
        if(stateCnt == 50)
        {   stateCnt = 0;
            stmEMK = 6;
        }
        break;
        case 6: //----------------------------------------------------20ms Block    
        stateCnt ++;
        getEmkSpannung(stateCnt, 1);
        if(stateCnt == 5) DrahtbruchErkennung(1); //Am Ende der DrBrSignal High Phase, -> Beginn Low Phase
        if(stateCnt == 20)
        {   stateCnt = 0;
            stmEMK = 7;            
        }
        break;
        case 7: //----------------------------------------------------50ms Block
        stateCnt ++;
        getEmkSpannung(stateCnt, 0);
        //if((stateCnt==8)||(stateCnt==18)||(stateCnt==28)||(stateCnt==38)||(stateCnt==48))
        //{   checkDrBrSpannung(0);         //Wird nicht benötigt       
        //}
        if(stateCnt == 50)
        {   stateCnt = 0;
            stmEMK = 0;
        }
        break;
        default: //-------------------------------------------
        stmEMK = 0;
        stateCnt = 0;
        break;
    }   
}
//---Ende EMK_DrBrErkennung()---------------------------------------------------


//------------------------------------------------------------------------------
//	DrahtbruchErkennung() wird alle 10ms aufgerufen
//	Konzept: Die Drahtbruchspannung DB- liegt dauerhaft an 0V, liegt über 1MOhm an L2.
//	Die Drahtbruchspannung DB+ wechselt von 0V auf 5V, liegt über jeweils 1M an L1 und L3.
//	DB+ High Phase = 260ms, Low Phase = 80ms. Das entspricht 2,94Hz
//	Duch die Unsymetrie der Phasen kann sich keine Motorfrequenz aufsynchronisieren,
//	was fälschlicherweise zum Drahtbruchfehler führen würde.
//	High Phase: Am Ende der 260ms wird der Algorithmus einmal durchlaufen.
//	Bei < 4Hz wird am Ende der High Phase festgestellt, ob eventuell ein Drahtbruch vorliegt,
//	und ein Merker gesetzt (eventuellDB). < 4Hz ist dann der Fall, wenn cntTimeOut250ms nach 250ms =0 ist.
//	Low Phase: Am Ende der 80ms wird der Algorithmus einmal durchlaufen.
//	Bei < 65Hz (Brummspannung) und gesetztem Merker (eventuellDB) wird bei Drahtbruch
//	der cntDrahtBruchErr inkrementiert.
//------------------------------------------------------------------------------
void DrahtbruchErkennung(uint8_t Pegel)
{	//###test###static uint8_t counter = 0;
    static uint8_t eventuellDB = 0;
	
	cnt10msRoutinen ++;

	//DB+ high Phase dauert 260ms (von 0-26). DB+ low Phase dauert 80ms (von 26-34/0) Entspricht 2,94Hz.
	//###test###if(counter < 34) counter ++;
	//###test###if(counter == 34) counter = 0;
	//###test###if(counter == 26)
    if(Pegel == 1)
	{	//Ende der DrahtBruchSignal high Phase, 260ms	
        //Beim DN3PD2 wird hier noch der 250ms Timeout abgefragt. Darauf wird hier verzichtet, weil
        //bei CCP und steigender Flanke der 250ms Timeout aufgezogen wird. Dadurch würde bei kleiner 
        //SchmittTrigger Hysterese, bei der die 2,94Hz Drahtbruch-Impulse durchkommen, 
        //die Drahtbrucherkennung ausgehebelt.
		if(IstFrqX10 < 40)	//250ms nach letztem Impuls (entspricht < 4Hz). 4Hz = IstFrqX10 von 40
		{	//> 2,99V (8bit 231, 10bit 927)
			if(avg20msADC > 927)
			{	//Gleichspannung am Eingang oder Drahtbruch
				eventuellDB = 1;
			}
			else
			{	//keine Gleichspannung und kein Drahtbruch
				eventuellDB = 0;
			}
		}
		else	// > 4Hz Motorfrequenz
		{	eventuellDB = 0;
			if(cntDrahtBruchErr > 0) cntDrahtBruchErr --;
			//if(cntDrahtBruchErr == 0)  flDrahtBruchErr = 0;	//Drahtbruchfehler rastet ein
		}
		//DB+ auf low schalten, es beginnt die low-Phase.	
		LATDbits.LATD1 = 1;     //Invertierung durch angesteuerten Transistor
	}
	//else if(counter == 0)
    else if(Pegel == 0)
	{	//Ende der DragtBruchSignal low Phase. 80ms, nachdem der counter von 26 bis 34/0 gezählt hat, 
		if(IstFrqX10 < 650)		//< 65Hz /Brummspannung
		{	if(eventuellDB == 1)
			{	// > 1,165V (8bit 90, 10bit 361 ) und < 1,836V (8bit 142, 10bit 569)
				if((avg20msADC > 361)&&(avg20msADC < 569))
				{	//Gleichspannung am Eingang oder Drahtbruch
					if(cntDrahtBruchErr < 10) cntDrahtBruchErr += 1;	//entspricht 3,2 Sek
					if(cntDrahtBruchErr >= 10) flDrahtBruchErr = 1;
				}
				else
				{	if(cntDrahtBruchErr > 0) cntDrahtBruchErr --;
					//if(cntDrahtBruchErr == 0)  flDrahtBruchErr = 0;
				}
			}
			else	//Merker eventuellDB == 0
			{	if(cntDrahtBruchErr > 0) cntDrahtBruchErr --;
				//if(cntDrahtBruchErr == 0)  flDrahtBruchErr = 0;
			}
		}
		else
		{	//keine Gleichspannung und kein Drahtbruch
			if(cntDrahtBruchErr > 0) cntDrahtBruchErr --;
			//if(cntDrahtBruchErr == 0)   flDrahtBruchErr = 0;
		}
		//DB+ auf high schalten, es beginnt die high-Phase.
		LATDbits.LATD1 = 0;     //Invertierung durch angesteuerten Transistor
	}
}
//------DrahtbruchErkennung()---------------------------------------------------


//------------------------------------------------------------------------------
//	getEmkSpannung() wir alle 1ms aufgerufen
//	Ermittelt aus 50 ADC-Werte den kleinsten (minValue) und den größten (maxValue).
//	maxValue minus minValue = SpannungSS. Die SpannungSS wird mit EMK-Ab/Ein-Schaltschw
//  verglichen und EmkStatus ermittelt und das TX.bf.EMK gesetzt. 
//	Liegt der ADC-Wert außerhalb 0,2V bis 3,1V, 
//  dann läuft der Motor oder es liegt ein Drahtbruch vor.
//  Die neuen EMK_avg und EMK_Peak Werte werden in analogFrqDetection()
//  von der ISR übernommen und gespeichert.
//  In EmkVerzoegerung():
//  Wenn TX.bf.EMK gesetzt ist, kann die Verzögerungszeit ablaufen, nach Ablauf
//  wird TX.bf.EMK_Verz auch gesetzt, und dann kann das CAN EMK Bit gesetzt werden.
//  Synchronisierung von EMK und Drahtbrucherkennung:
//  Das Drahtbruchsignal verursacht einen Spannungshub im EMK Siganlpfad. Damit
//  der Spannungshub nicht die Messung von EMK SpannungSS verfälscht, muss das
//  Schalten der Dr.Br.Spannung mit der EMK synchronisiert werden, sodass
//  die Flanken der Dr.Br.Spannung bei der EMK Ermittlung vermieden werden.
//  Die 50ms Blöcke der EMK Ermittlung werden zeitlich so platziert, dass innerhalb
//  der 50ms Blöcke keine Siganlflanke vorkommt. In der Funktion getEmkSpannung()
//  wird per Statemachine stmDrBrEMK der Ablauf von EMK Ermittlung und Dr.Br.Erkennung
//  gesteuert. Siehe Dokument Drahtbruch-Emk-Delta.xlsx.
//  Übergabe Parameter:
//    Pause = 1 beim Umschalten der DrBrSpannung, dann wird die Delta Auswertung pausiert.
//    Pause = 0 wenn die Delta Auswertung durchgeführt werden soll.
//    Die Peak Auswertung wird in beiden Fällen durchgeführt.
//    stateCnt: Von 1 bis 50, Pause=0, bei 50 wird das Delta ausgewertet und DeltaStatus gesetzt.
//              Von 1 bis 20, wenn Pause=1 ist.
//------------------------------------------------------------------------------
void getEmkSpannung(uint8_t stateCnt, uint8_t Pause)
{   //###test###uint8_t cnt50Samples = 0;
    static uint16_t minValue = 0x3FF, maxValue = 0;
    
    //###test###TODO Überarbeiten Extremwerte und Drahtbruchsituation, Pause
  
	//###test###cnt1msRoutinen ++;
    //cnt50Samples = stateCnt;
     
    if((PS.Par.EmkMode == 0)
    || ((CanRX[0] & 0x80) != 0)
    || (flParamKomplett == 0))
    {   EmkStatus = 0;
        TX.Bytes[8] = 0x00;
        //###test###cnt50Samples = 0;
        minValue = 0x3FF;
        maxValue = 0;   
        EmkDelta = 0;
    }
    else if(PS.Par.EmkMode == 1)
    {   if(Pause == 0)
        {   //Bei Pause == 0 EmkDelta Ermittlung durchführen
            //###test###cnt50Samples ++;	
            //###test###if((EMK_avg >= k0V2)&&(EMK_avg <= k3V1))
            //###test### {
            if(EMK_avg < minValue) minValue = EMK_avg;          //minValue ermitteln
            if(EMK_avg > maxValue) maxValue = EMK_avg;          //maxValue ermitteln
            if(stateCnt == 50)		//cnt50Samples, nach 50 samples (50mS) min/maxValue auswerten
            {	
                EmkDelta = (maxValue - minValue);  //delta kann höchstens 2,9V sein
                TX.Bytes[8] = (EmkDelta >> 2);     //10Bit Wert --> 8Bit
                if(EmkDelta > PS.Par.EMKAbschaltschw) EmkStatus = (EmkStatus << 1);	     //0 in das Schieberegister schieben
                if(EmkDelta < PS.Par.EMKEinschaltschw) EmkStatus = ((EmkStatus << 1) +1);   //1 in das Schieberegister schieben
                //wieder zurücksetzen
                //###test###cnt50Samples = 0;
                minValue = 0x3FF;
                maxValue = 0;
            }
            //###test### }
            /*
            else
            { 	//Bei Extremwerte sofort EmkStatus beschreiben
                EmkStatus = (EmkStatus << 1);        //0 einschieben
                TX.Bytes[8] = 0xFF;
                //wieder zurücksetzen
                //###test###cnt50Samples = 0;
                minValue = 0x3FF;
                maxValue = 0;
                EmkDelta = 1023;
            }
            */
        }
        else //Pause == 1, EmkDelta pausieren, EmkStatus bleibt erhalten.
        {   minValue = 0x3FF;
            maxValue = 0;
            //###test###EmkDelta = 0;
        }
    
        //Peakwert counter zum Abfangen von großen Frequenzen. Unabhängig von Pause.
        if((EMK_Peak < k0V2)||(EMK_Peak > k3V1))	 //Extremwerte
        {	if(cntEmkPeak < 50) cntEmkPeak ++;
        }
        else										 //kein Extremwert
        {	if(cntEmkPeak > 0) cntEmkPeak --;
        }
        if(cntEmkPeak == 50)                         //zum Abfangen von Frq. > 1000Hz
        {	EmkStatus = 0;
            TX.Bytes[8] = 0xFF;
            //wieder zurücksetzen
            //###test###cnt50Samples = 0;
            //###test###minValue = 0x3FF;
            //###test###maxValue = 0;
        }
        //------------------------------- 
        /*
        if((TX.bf.ErrDrahtBr==1)||(TX.bf.ErrExtendedDrBr))
        {	EmkStatus = 0;
            TX.Bytes[8] = 0x00;
            //wieder zurücksetzen
            //###test###cnt50Samples = 0;
            minValue = 0x3FF;
            maxValue = 0;
            EmkDelta = 0;
        }
        */
        //-------------------------------   
        if((EmkStatus & 0b11111111) == 0b11111111) TX.bf.EMK = 1; //8-fach Messung, Dauer 400ms
        if((EmkStatus & 0b11111111) == 0b00000000) TX.bf.EMK = 0;	
    }  
}
//---Ende getEmkSpannung()------------------------------------------------------


//------------------------------------------------------------------------------
//checkDrBrSpannung() wird von stm_EMK_DrBrErk() aufgerufen,
//Ein DrBrSpannung-Zyklus dauerert 340ms. Pro Zyklus wird die Funktion
//5x mit Pegel=0 (entspricht 0V) und 5x mit Pegel=1 (entspricht 6,8V) aufgerufen.
//------------------------------------------------------------------------------
void checkDrBrSpannung(uint8_t Pegel)
{   //uint8_t cntDrBrSpLowErr, cntDrBrSpHighErr, flDrBrSpannung
    //kDrBrSpLow, kDrBrSpHighMin, kDrBrSpHighMax
    if(Pegel == 0)
    {   if(ADC_DrBrSp > kDrBrSpLow) cntDrBrSpLowErr ++;
        else
        {   if(cntDrBrSpLowErr > 0) cntDrBrSpLowErr --;
        }
    }
    else //Pegel = 1
    {   if((ADC_DrBrSp < kDrBrSpHighMin)||(ADC_DrBrSp > kDrBrSpHighMax))
        {   cntDrBrSpHighErr ++;
        }
        else
        {   if(cntDrBrSpHighErr > 0) cntDrBrSpHighErr --;
        }
    }
    if((cntDrBrSpLowErr > 14)||(cntDrBrSpHighErr > 14))
    {   flDrBrSpannung = 1;                 //nach ca. 1s
    }
}
//---Ende checkDrBrSpannung()---------------------------------------------------


//------------------------------------------------------------------------------
//checkDiffVerst() wird alle 1ms aufgerufen
//Bei Servoantriebe und DrBr. an W schalten die Differenzverst. mit der PWM Frq. eine 
//Rechteckspannung mit 5Vss. Gleichgerichtet ergibt sich ca. 3V gegen GND.
//Diese gleichgerichtete Spannung wird analog erfasst über 128ms, Min/Max und Durchschnitt 
//ermittelt, dann die Plausibilität geprüft.
//Bei Timeout und DiffSp Durchschnitt > 1,94V --> Fehler.
//Bei > 20Hz liegt eine pulsierende Spannung an, der Durchschnitt ist daher klein. Bei > 0,25V OK.
//------------------------------------------------------------------------------
void checkDiffVerst(void)
{	static uint8_t cntDiff = 0; 	//minDiff = 255, maxDiff = 0;
	static uint32_t avgDiff = 0;

	cnt1msRoutinen ++;

	if(cntDiff < 128) cntDiff ++;
	//Bei cntDiffErr 1 bis 128 Summe bilden 
	avgDiff += DiffVerst;							//aufsummieren
	//if(adcDIFF < minDiff) minDiff = adcDIFF;		//min Wert ermitteln
	//if(adcDIFF > maxDiff) maxDiff = adcDIFF;		//max Wert ermitteln

	if(cntDiff >= 128)
	{	//Nach 128ms Durchschnitt ausrechnen, Plausibilität prüfen
		avgDiff = (avgDiff >> 7);					//Durchschnitt = Summe / 128
		//Plausibilität prüfen
		if(cntTimeOut1S < 51)                       //<2Hz.	Timeout von 500ms, von 100 -> 50 dauert 500ms. 100 -> 0 dauert 1s.
		{	if(avgDiff > 397)						//5V, 8bit: 99d = 1,94V. Entspricht 3,3V, 10bit: 397d = 1,28V
			{	cntDiffErr +=2;
				if(cntDiffErr > 17) flDiffErr = 1;	//18/2=9, 9x128ms = 1152ms.  Zusammen: 500ms+1152ms = 1652ms
			}
			else
			{	if(cntDiffErr > 0) cntDiffErr --;
			}

		}
		else if(IstFrqX10 >= 200)
		{	//>=200 = >= 20Hz
			if(avgDiff < 78)						//5V, 8bit: 13d = 0,25V. Entspricht 3,3V, 10bit: 78d = 0,25V
			{	
				cntDiffErr +=2;
				if(cntDiffErr > 17) flDiffErr = 1;	//18/2=9, 9x128ms = 1152ms.  Zusammen: 500ms+1152ms = 1652ms
			}
			else
			{	if(cntDiffErr > 0) cntDiffErr --;
			}
		}
		else if(IstFrqX10 < 200)
		{	//< 200 = < 20Hz
			if(cntDiffErr > 0) cntDiffErr --;
		}
		//-----------------------------------
		//DebugArr[0] = avgDiff;
		//DebugArr[1] = cntDiffErr;
		//DebugArr[2] = _flDiffErr;
		//-----------------------------------
		cntDiff = 0;
		avgDiff = 0;  
	}
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//ADCauswerten() wird alle 10ms aufgerufen.
//Folgende Werte wurden in analogFrqDetection() ausgelesen: 
//ADC1V65, ADC3V3, ADC12V, ADC_DrBrSp. Die Auflösung ist jeweils 10bit. 
//------------------------------------------------------------------------------
void ADCauswerten(void)
{   cnt10msRoutinen ++;
    if((ADC3V3 > k3V3TestMax)||(ADC3V3 < k3V3TestMin))
    {   cntErr3V3Test ++;
        if(cntErr3V3Test > 30)  //30 entspricht 300ms
        {   flErr3V3Test = 1;               
        }
    }
    else
    {   if(cntErr3V3Test > 0) cntErr3V3Test --;      
    }

    if((ADC1V65 > k1V65TestMax)||(ADC1V65 < k1V65TestMin))
    {   cntErr1V65Test ++;
        if(cntErr1V65Test > 30) //30 entspricht 300ms
        {   flErr1V65Test = 1;               
        }
    }
    else
    {   if(cntErr1V65Test > 0) cntErr1V65Test --;      
    }

    if((ADC12V > k12VTestMax)||(ADC12V < k12VTestMin))
    {   cntErr12VTest ++;
        if(cntErr12VTest > 30) //30 entspricht 300ms
        {   flErr12VTest = 1;               
        }
    }
    else
    {   if(cntErr12VTest > 0) cntErr12VTest --;      
    }
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void ADC_Config(void)
{   //Ch3 = L3-Integrator, Ch4 = 1,65V, Ch9 = EMK-Spannung
    //Ch12 = 6,8V DrahtbruchTaktSpannung,  Ch13 = Temperatur, Ch14 = 12V, Ch17=3,3V-Test
    //ANSELA/C und TRISA/C Bits werden in init() gesetzt
    //Eine ADC Wandlung dauert ca. 1µs. Ein Scan von 6 channels 6µs.
    //Ein Scan auf 50µs verlängert, damit das timing dem DN3PD2 entspricht: 6x 8,3µs. Sampletime=7,3µs.
    //Während 150µs werden 4 Scans durchlaufen.
    //Alle 0,5ms den Scan starten, dann 4x alle 50µs ein Interrupt.
    //Pro 1ms werden dann 8 Scans durchgeführt, 8 Werte für EMK-Auswertung.
        
    //Conversion Time = 8 ? TCORESRC)  + (Bit Resolution + 2.5) ? TADCORE
    //Conversion Time = 8 x 1/80MHz    +           10,5         x  1/40MHz
    //Conversion Time = 0,1µs          +                 0,26µs            =  0,36µs
    //6 Conversions x 4 Scans = 24 Conversions.
    //Nach 150µs sollen die 24 Conversions fertig sein.   
    
    ADCON5Hbits.WARMTIME = 10;  //1024 x 1/20MHz = 51,2µs
    ADCON1Lbits.ADON = 0;
    ADCON1Hbits.FORM = 0;       //0=integer. Defaultmodus: unsigned und nicht pseudodifferentiell.
    ADCON1Hbits.SHRRES = 2;     //1=8bit,Ergebnis >>4. 2=10bit,Ergebnis >>2. 3=12bit Resolution. 
    ADCON3Lbits.REFSEL = 0;     //VrefH = AVdd, VrefL = AVss
    ADCON3Hbits.CLKSEL = 1;     //1=Fosc=80Mhz
    ADCON3Hbits.CLKDIV = 0;     //0+1=1 Clock Devider 80MHz   
    ADCON2Lbits.SHRADCS = 1;    //1x2=2 80MHz/2=40MHz   1Tad = 1/40MHz = 25ns. (Min. ist 14,3ns)
    ADCON2Hbits.SHRSAMC = 305;  //Sample Time = (305+2) x  TAD = 307 x 25ns =  7,68µs (Der Wert 305 wurde empirisch ermittelt!)
    ADCON1Lbits.ADON = 1;       //ADC Power ON
    ADCON5Lbits.SHRPWR = 1;
    while(ADCON5Lbits.SHRRDY == 0) {;}  //1: Power is ready
    ADCON3Hbits.SHREN = 1;      //Shared enabled
    ADTRIG0Hbits.TRGSRC3 = 1;   //Analog Channels (3 bis 17), 1= Common SW trigger
    ADTRIG1Lbits.TRGSRC4 = 1;
    ADTRIG2Lbits.TRGSRC9 = 1;
    ADTRIG3Lbits.TRGSRC12 = 1;
    ADTRIG3Lbits.TRGSRC13 = 1;
    ADTRIG3Hbits.TRGSRC14 = 1;
    ADTRIG4Lbits.TRGSRC17 = 1;
    //ADCON3Lbits.SWCTRG = 1;   Das setzen des Common Trigger Bits triggert den Scan von AN3-AN17
    IFS6bits.ADCAN17IF = 0;     //löschen in ISR
    ADIEHbits.IE17 = 1;         //IR enable of channel17. 
    IPC22bits.ADCIP = 6;
    IFS5bits.ADCIF = 0;         //löschen in ISR
    IEC5bits.ADCIE = 1;
      
    //Wenn kein Scan gemacht werden soll sondern einzelne Channels getriggert werden sollen,
    //dann mit ADCON3Lbits.CNVCHSEL den Channel wählen und ADCON3Lbits.CNVRTCH einzeln triggern,
    //die ADTRIG0Hbits.TRGSRC3... dürfen dann nicht verwendet werden.
    //In den 6 Buffer ADCBUF3,4,9,13,14 und 17 steht jeweils das Ergebnis nach dem Scan
    //Format der Ergebnisse, uint: 12Bit=rechtsbündig, 10Bit todo: >>2. 8Bit todo >>4.
    //Unsigned Integer (SIGNx = 0, FORM = 0), 8Bit resolution
    //0 0 0 0 d07 d06 d05 d04 d03 d02 d01 d00 1 0 0 0
    //Nach lesen der Ergebnisse die IFs in IFS5/6 löschen.
    //ADIELbits.IE0-15, entspricht den ADC channels
    
    //IFS5 ADCAN4IF ADCAN3IF ADCAN2IF ADCAN1IF ADCAN0IF         ADCIF
    //IFS6 ADCAN20IF ADCAN19IF ADCAN18IF ADCAN17IF ADCAN16IF ADCAN15IF ADCAN14IF ADCAN13IF
    //     ADCAN12IF ADCAN11IF ADCAN10IF ADCAN9IF ADCAN8IF ADCAN7IF ADCAN6IF ADCAN5IF
    //IFS7 ADCAN23IF ADCAN22IF ADCAN21IF
    //IEC5 ADCAN4IE ADCAN3IE ADCAN2IE ADCAN1IE ADCAN0IE         ADCIE
    //IEC6 ADCAN20IE ADCAN19IE ADCAN18IE ADCAN17IE ADCAN16IE ADCAN15IE ADCAN14IE ADCAN13IE
    //     ADCAN12IE ADCAN11IE ADCAN10IE ADCAN9IE ADCAN8IE ADCAN7IE ADCAN6IE ADCAN5IE
    //IEC7 ADCAN23IE ADCAN22IE ADCAN21IE
    //IPC22 ADCAN0IP ?                                         ADCIP
    //IPC23 ADCAN4IP ? ADCAN3IP ? ADCAN2IP ? ADCAN1IP
    //IPC24 ADCAN8IP ? ADCAN7IP ? ADCAN6IP ? ADCAN5IP
    //IPC25 ADCAN12IP ? ADCAN11IP ? ADCAN10IP ? ADCAN9IP
    //IPC26 ADCAN16IP ? ADCAN15IP ? ADCAN14IP ? ADCAN13IP
    //IPC27 ADCAN20IP ? ADCAN19IP ? ADCAN18IP ? ADCAN17IP
    //IPC28 ADCAN23IP ? ADCAN22IP ? ADCAN21IP
}
//------------------------------------------------------------------------------