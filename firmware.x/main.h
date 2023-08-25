#ifndef MAIN_H
#define	MAIN_H

#include <stdint.h>
int main(void);

#define FW_Version  0x01
//Typenschild:
#define Hardware	0x12	//Schaltplan und Layout V1.1
#define FW1         0x0		//PDSV-V0.1.1.0
#define FW2         0x1
#define FW3         0x1
#define FW4         0x0
#define ArtNr1      'P'     //PDSV
#define ArtNr2      'D'
#define ArtNr3      '0'
#define ArtNr4      '1'



//Kennungen der Vario Module
//20: Zentralmodul ohne LCD über Derivatverwaltung
//21: InOut Karte
//22: Drehzahlkarte
//23: Relaismodul
//24: Feldbus
//25: IN Modul
//26: Netzwerk
//27: Resolver
//28: SSI Modul
//29: DSIV Modul
//30: Gateway
//31: BiSS Modul
//32: PDSV Modul
#define Kennung     32

#define _Flag1ms    FlagsTime1.bit0
#define _Flag2ms    FlagsTime1.bit1
#define _Flag4ms    FlagsTime1.bit2
#define _Flag10ms    FlagsTime1.bit3
#define _Flag50ms    FlagsTime1.bit4
#define _Flag100ms    FlagsTime1.bit5
#define _Flag500ms    FlagsTime1.bit6
#define _Flag1000ms    FlagsTime1.bit7
/*frei bis bit15*/


#define _flErrorPorts     FlagsError1.bit0
//frei bis bit15

typedef struct
{   unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
    unsigned bit8: 1;
    unsigned bit9: 1;
    unsigned bit10: 1;
    unsigned bit11: 1;
    unsigned bit12: 1;
    unsigned bit13: 1;
    unsigned bit14: 1;
    unsigned bit15: 1;
} volatile Bitfeld;

/*
//Der Datentyp unionCanBuff wird als CAN Transferbuffer verwendet, um auf
//einfache Weise den Datenaustausch Word nach Byte oder DWord nach Byte
//zu ermöglichen.
//Word 0-2 (entsprechend Byte 0-5) beinhalten CAN Message ID und DLC Bits.
//Word 3-6 (entsprechend Byte 6-13) beinhalten die 8 CAN Datenbyte.
//Word 4   (entsprechend Byte 14-15) beinhaltet Filter Hit Bits
typedef union
{	uint8_t byte[16];
	uint16_t word[8];
    uint32_t dword[4];
} unionCanBuf;
*/

//extern uint8_t triggerCanTX0, triggerCanTX1;
extern uint8_t fl05ms, fl1ms, fl4ms, fl10ms, fl100ms, fl300ms;


#endif	/* MAIN_H */

