#ifndef GETSTECKPLATZ_H
#define	GETSTECKPLATZ_H

#include <stdint.h>

/*** Typedefs ***/
typedef enum ADR_STATUS {
  	WAITING,
	START_,
	BYTE_,
	COMPLETE,
	READY,
	RCVD,
	FAIL
  } ADR_STATUS_t;


/* ************************************************************ */
/* Hardware Abstraction Layer (HAL) Defines                     */
/* ************************************************************ */
//ADRIN=RD10, ADROUT=RC7.  
//ADRES-L und ADRES-R wurden im Layout am T-Bus Stecker vertauscht.
//Die Steckplatzadresse kommt über ADRES-R herein!
#define  ADROUT_AN     		(LATCbits.LATC7 = 1)	//ADRES-L
#define  ADROUT_AUS    		(LATCbits.LATC7 = 0)	//ADRES-L
#define  ADROUT_MASK        (PORTC & 0x0080)        //ADRES-L
#define  ADRIN_MASK         (PORTD & 0x0400)        //ADRES-R
#define  ADRIN_AN           (LATDbits.LATD10 = 1)	//ADRES-R
#define  ADRIN_AUS          (LATDbits.LATD10 = 0)	//ADRES-R

#define  SET_ADROUT_INPUT   (TRISCbits.TRISC7 = 1)	//ADRES-L
#define  SET_ADROUT_OUTPUT	(TRISCbits.TRISC7 = 0)	//ADRES-L
#define  SET_ADRIN_INPUT 	(TRISDbits.TRISD10 = 1)	//ADRES-R
#define  SET_ADRIN_OUTPUT 	(TRISDbits.TRISD10 = 0)	//ADRES-R
  
  
#define RESET_TIMER		(TMR1 = 0)
#define START_TIMER		(T1CONbits.TON = 1)
#define STOP_TIMER		(T1CONbits.TON = 0)
#define TIMER_IF		(IFS0bits.T1IF)

void ports_init(void);
uint8_t getSteckplatz(void);
uint8_t get_adrbit(void);
uint8_t get_adrout(void);
void set_adrbit(uint8_t);
void set_adrout(uint8_t);
void wait_ms(uint8_t);
uint8_t wait_adrout_fall(uint8_t);
uint8_t get_adr_next(uint8_t, uint16_t);

#endif	/* GETSTECKPLATZ_H */

