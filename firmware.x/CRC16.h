#ifndef CRC16_H
#define	CRC16_H

#include <stdint.h>
#include "Frequenz.h"

#define SizeDNCO    256
#define SizeZMV_Par 54  //von 0-53. 54/55 ist CRC16

#define ZMV_ParSatz   1
#define Laufz_ParSatz 2

uint16_t getCRC16(uint8_t ParSatz);

extern ParamStruct PS;
extern volatile uint8_t Param[];
extern volatile uint8_t DNCO[];

#endif	/* CRC16_H*/

