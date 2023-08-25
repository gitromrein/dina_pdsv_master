#include <xc.h>
#include "p33CK256MP505.h"
#include "getSteckplatz.h"
#include <stdint.h>


/* ************************************************************ */
/* getSteckplatz()*/
/* ************************************************************ */
uint8_t getSteckplatz(void)
{

	uint8_t adr_byte = 0;
	uint8_t adr_byte_next = 0;
	uint16_t rack = 0;

	uint8_t a;

	SET_ADRIN_INPUT;
	SET_ADROUT_OUTPUT;
	set_adrout(1);

   	/* Warten auf High an Adresseingang */
	while( ADRIN_MASK == 0x00 ){};

    /* Warten auf fallende Flanke an Adresseingang */
   do{	
       while( ADRIN_MASK != 0)	;	//warten
	}	while(get_adrbit() != 0)	;

   	/* Einlesen des Adressbytes */
    for(a = 0; a<8; a++){
		switch (get_adrbit()){
        	case 0x00: break;
        	case 0x01: adr_byte |= (0x01<<a);
                   break;
        	default:   adr_byte = 0;
                   //adr_status = FAIL;
        }
	}

	/* Einlesen der Steckplatzbelegung */
    for(a = 0; a<16; a++)
	{
		switch (get_adrbit()) // Bit n
       	{
        	case 0x00: break;
        	case 0x01: rack |= ((unsigned int)0x0001<<a);
                   break;
        	default:   rack = 0;
                   //adr_status = FAIL;
        	}
	}

	/* Einlesen des Stop-Bits High */
	if(get_adrbit() != 1) return 20;

    
 
	/* wenn die Adresse erfolgreich eingelesen wurde */
	/* dann wird sie nach links zurueckgeschickt und */
	/* nach rechts um 1 erhöht herausgeschickt.      */
	adr_byte_next = get_adr_next(adr_byte, rack);

	//SET_ADRIN_OUTPUT;
	//SET_ADROUT_OUTPUT;
	wait_ms(1);
	//set_adrbit(1);    // Start 1
	set_adrout(1);    // Start 1
	wait_ms(10);
	//set_adrbit(0);    // Start 0
	set_adrout(0);    // Start 0
	wait_ms(10);

	for(a = 0; a<8; a++)	//unsigned char a
	{
		set_adrout((adr_byte_next)>>a);
		wait_ms(10);
	}

	for(a = 0; a<16; a++)	//unsigned char a
	{
        set_adrout((rack>>a)&0xFF);
		wait_ms(10);
	}

	set_adrout(1);    // Stop
	wait_ms(50);

    //SET_ADROUT_INPUT;
  	return(adr_byte);
}
/* ************************************************************ */
/* End - getSteckplatz()                                        */
/* ************************************************************ */

uint8_t get_adr_next(uint8_t myadr, uint16_t myrack)
{
	unsigned char result = 0;
	unsigned char counter = 0;
	unsigned int mask = 0x0001;

	for(counter=myadr+1; counter<16; counter++)
	{
		if((myrack & (mask<<counter)) != 0)
			{
				result = counter;
				return(result);
			}
	}
	//result = myadr+1;
	return(result);
}


/* ************************************************************ */
/* get_adrbit - Adressbit von linker Quelle einlesen            */
/* ************************************************************ */
uint8_t get_adrbit(void)
{
  uint8_t buffer;

  buffer = 0;
  wait_ms(4);
  if (ADRIN_MASK) 
      buffer |= 0x01;
  wait_ms(1);
  if (ADRIN_MASK) 
      buffer |= 0x02;
  wait_ms(1);
  if (ADRIN_MASK) 
      buffer |= 0x04;
  wait_ms(4);
  switch (buffer)
  {
      case 0x00: buffer = 0x00;
                break;
      case 0x07: buffer = 0x01;
                break;
      default: buffer = 0xFF;
  }
  return(buffer);
}

/* ************************************************************ */
/* set_adrbit - Adressbit nach linker Quelle ausgeben           */
/* ************************************************************ */
void set_adrbit(uint8_t value)
{
  if (value & 0x01) ADRIN_AN;
  else ADRIN_AUS;
  return;
}

/* ************************************************************ */
/* set_adrout - Adressbit nach rechtem Modul ausgeben           */
/* ************************************************************ */
void set_adrout(uint8_t value)
{
  if (value & 0x01) ADROUT_AN;
  else ADROUT_AUS;
  return;
}

/* ************************************************************ */
/* get_adrout - Adressbit von rechter Quelle einlesen           */
/* ************************************************************ */
uint8_t get_adrout(void)
{
  uint8_t buffer;

  buffer = 0;
  wait_ms(4);
  if (ADROUT_MASK) buffer |= 0x01;
  wait_ms(1);
  if (ADROUT_MASK) buffer |= 0x02;
  wait_ms(1);
  if (ADROUT_MASK) buffer |= 0x04;
  wait_ms(4);
  switch (buffer)
  {
      case 0x00: buffer = 0x00;
                break;
      case 0x07: buffer = 0x01;
                break;
      default: buffer = 0xFF;
  }
  return(buffer);
}


/* ************************************************************ */
/* wait_ms - ms Warteschleife                                   */
/* ************************************************************ */
void wait_ms(uint8_t ms)
{
	uint8_t cnt_ms = 0;
	RESET_TIMER;
	START_TIMER;
	while(cnt_ms < ms)			//solange cnt_ms < übergebene Zeit
	{	while(TIMER_IF == 0) 	//warten bis 1ms abgelaufen
		{ 	;
		}
  		TIMER_IF = 0;			//1ms ist um
		cnt_ms ++;
	}
	STOP_TIMER;
	RESET_TIMER;
  return;
}

/* ************************************************************ */
/* wait_ADROUT_FALL - warten bis fallende Flanke an ADROUT      */
/* ************************************************************ */
uint8_t wait_ADROUT_FALL(uint8_t ms)
{
	uint8_t cnt_ms = 0;
	RESET_TIMER;
	START_TIMER;
 	while(ADROUT_MASK != 0)
	{	if(TIMER_IF == 1)
		{	TIMER_IF = 0;
			cnt_ms ++;
		}
		if(cnt_ms > ms) break;
	}
	STOP_TIMER;
	RESET_TIMER;
	if(cnt_ms > ms) cnt_ms = 255;
	else			cnt_ms = 0;

  	return cnt_ms;
}
