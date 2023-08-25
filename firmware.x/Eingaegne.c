#include <xc.h>
#include "p33CK256MP505.h"
#include "Eingaenge.h"
#include "main.h"
#include "SKR_ZwHd.h"
#include <stdint.h>

static uint16_t debInput4ms = 0, debInput16ms = 0;

//-----------------------------------------------------------------------------------------------------------------------------------------
//	debounceInput4ms() wird alle 1ms aufgerufen.
//	Entprellt die Signale der Eingangsklemmen
//	_Input1_8: 	Eingänge
//	debInput4ms: entprellte Eingänge
//	cntDebounce1: Entprellzähler1
//	cntDebounce2: Entprellzähler2
//	The purpose of this routine is to debounce, i.e. digitally low pass filter
//	inputs. The algorithm handles upto 8 bits at a time. An input is considered
//	filtered if it has not changed states in the last 4 samples.
//	2-bit cyclic vertical counters count the 4 samples. As long as there is no
//	change, the counters are held in the reset state of 00b. When a change is detected
//	between the current sample and the filtered or debounced sample, the counters
//	are incremented. The counting sequence is 00,01,10,11,00... When the counters
//	roll over from 11b to 00b, the debounced state is updated. If the input changes
//	back to the filtered state while the counters are counting, then the counters
//	are re-initialized to the reset state and the filtered state is unaffected.
//	In other words, a glitch or transient input has been filtered.
//-----------------------------------------------------------------------------------------------------------------------------------------
void debounceInput4ms(void)
{   static uint16_t cntDebounce1 = 0, cntDebounce2 = 0;
    uint16_t notiz = 0, input = 0;

	cnt1msRoutinen ++;
    
    input = PortsEinlesen();
   
    //Querschlusserkennung der SKR Eingänge
	QSchlErkSKR((uint8_t)input);

	// Entprellen
	cntDebounce1 = (cntDebounce1 ^ cntDebounce2);	// vertikale Zähler erhöhen
	cntDebounce2 = (~cntDebounce2);					// vertikale Zähler erhöhen
	notiz = (input ^ debInput4ms);					// Änderungen in notiz
	cntDebounce2 = (notiz & cntDebounce2);	// wo keine Änderung zurücksetzen
	cntDebounce1 = (notiz & cntDebounce1);	// wo keine Änderung zurücksetzen
	notiz =(cntDebounce1 | cntDebounce2);
	debInput4ms = (notiz & debInput4ms);
	notiz = (~notiz);
	notiz = (notiz & input);
	debInput4ms = (notiz | debInput4ms);
	// Entprellen ist fertig
}
//----Ende debounceInput()-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------
//	debounceInput16ms() wird alle 4ms aufgerufen.
//	Entprellt die Signale der Eingangsklemmen
//	debInput4ms: 	Eingänge
//	debInput16ms: entprellte Eingänge
//	cntDebounce3: Entprellzähler1
//	cntDebounce4: Entprellzähler2
//	The purpose of this routine is to debounce, i.e. digitally low pass filter
//	inputs. The algorithm handles upto 8 bits at a time. An input is considered
//	filtered if it has not changed states in the last 4 samples.
//	2-bit cyclic vertical counters count the 4 samples. As long as there is no
//	change, the counters are held in the reset state of 00b. When a change is detected
//	between the current sample and the filtered or debounced sample, the counters
//	are incremented. The counting sequence is 00,01,10,11,00... When the counters
//	roll over from 11b to 00b, the debounced state is updated. If the input changes
//	back to the filtered state while the counters are counting, then the counters
//	are re-initialized to the reset state and the filtered state is unaffected.
//	In other words, a glitch or transient input has been filtered.
//-----------------------------------------------------------------------------------------------------------------------------------------
void debounceInput16ms(void)
{       static uint16_t cntDebounce3 = 0, cntDebounce4 = 0;
	uint16_t notiz, input;
	cnt4msRoutinen ++;
	input = debInput4ms;
	// Entprellen
	cntDebounce3 = (cntDebounce3 ^ cntDebounce4);	// vertikale Zähler erhöhen
	cntDebounce4 = (~cntDebounce4);					// vertikale Zähler erhöhen
	notiz = (input ^ debInput16ms);				// Änderungen in notiz
	cntDebounce4 = (notiz & cntDebounce4);	// wo keine Änderung zurücksetzen
	cntDebounce3 = (notiz & cntDebounce3);	// wo keine Änderung zurücksetzen
	notiz =(cntDebounce3 | cntDebounce4);
	debInput16ms = (notiz & debInput16ms);
	notiz = (~notiz);
	notiz = (notiz & input);
	debInput16ms = (notiz | debInput16ms);
	// Entprellen ist fertig
}
//----Ende debounceInput()-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------
//	selectInput(), wird alle 4ms aufgerufen. Übernimmt die Entprellzeiten gem. d. Parameter.
//-----------------------------------------------------------------------------------------------------------------------------------------
void selectInput(void)
{   //static uint8_t CanTX3alt = 0, cntTriggerCan = 0;
    uint8_t input4ms, input16ms;
	cnt4msRoutinen ++;
    input4ms = (uint8_t)debInput4ms;
    input16ms =  (uint8_t)debInput16ms;
	TX.Bytes[1] = ((Param[0] & input4ms) | ((~Param[0]) & input16ms));
    
    if(TX.bf.ErrPortTest == 1) TX.Bytes[1] &= 0xF0;
    
	//CanTX[3] = (TX.Bytes[1] & RX.Bytes[1]); //#####test###########
    CanTX[3] = TX.Bytes[1];                   //#####test###########
}
//----Ende selectInput()-----------------------------------------------------------------------------------------------------

uint8_t PortsEinlesen(void)
{   
    uint16_t pad = 0;
    uint8_t input = 0;
    
    pad = PORTC;
    if((pad & 0x2000)!= 0) input |= 0x01; //IN1 = RC13
    if((pad & 0x1000)!= 0) input |= 0x02; //IN2 = RC12
    pad = PORTB;
    if((pad & 0x4000)!= 0) input |= 0x04; //IN3 = RB14
    if((pad & 0x8000)!= 0) input |= 0x08; //IN4 = RB15
    return input;
}


