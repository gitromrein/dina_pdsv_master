#ifndef EINGAENGE_H
#define	EINGAENGE_H

#include <stdint.h>
#include "Uart.h"
uint8_t PortsEinlesen(void);
void debounceInput4ms(void);
void debounceInput16ms(void);
void selectInput(void);

extern uint8_t CanTX[8];
extern str_ipk TX;
extern volatile uint8_t Param[64];
extern uint8_t cnt1msRoutinen, cnt4msRoutinen;

#endif	/* EINGAENGE_H */

