#ifndef INIT_H
#define	INIT_H

#include <stdint.h>
#include "RamTest.h"
#include "RomTest.h"
#include "Frequenz.h"
#include "Uart.h"

void init(void);
extern uint16_t Steckplatz;
//extern uint8_t SerTX[];
extern uint16_t cntLedAlternativ;
extern volatile uint8_t Param[64];
extern volatile uint8_t DNCO[256];
extern uint8_t DncoCopy[256];
extern ParamStruct PS;
extern str_ipk RX, TX, LatchRX, LatchTX;
extern uint16_t AdrRom, AdrRomCopy;

/*
extern uint16_t __attribute__((address(0x1000))) RamZelleBackup;
extern uint16_t __attribute__((address(0x1002))) RamBackup;
extern uint16_t __attribute__((address(0x1004))) idxPattern;
extern uint16_t __attribute__((address(0x1006))) RamZelleBackupINV;
extern uint16_t __attribute__((address(0x1008))) RamBackupINV;
extern uint16_t __attribute__((address(0x100A))) *ptrRamZelle;
extern uint16_t __attribute__((address(0x100C))) *ptrRam;
extern uint16_t __attribute__((address(0x100E))) dummy;
*/  
//###################test####################################
extern uint16_t __attribute__((address(0x2000))) RamZelleBackup;
extern uint16_t __attribute__((address(0x2002))) RamBackup;
extern uint16_t __attribute__((address(0x2004))) idxPattern;
extern uint16_t __attribute__((address(0x2006))) RamZelleBackupINV;
extern uint16_t __attribute__((address(0x2008))) RamBackupINV;
extern uint16_t __attribute__((address(0x200A))) *ptrRamZelle;
extern uint16_t __attribute__((address(0x200C))) *ptrRam;
extern uint16_t __attribute__((address(0x200E))) dummy;

#endif	/* INIT_H */

