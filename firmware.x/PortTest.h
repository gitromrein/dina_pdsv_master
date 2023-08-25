#ifndef PORTTEST_H
#define	PORTTEST_H

#include <stdint.h>
//#include "Transistoren.h"

void PortTest(void);
void wait(void);

extern uint8_t flParamKomplett;
extern uint8_t CanTX[];
extern uint8_t cnt100msRoutinen;



#endif	/* PORTTEST_H */

