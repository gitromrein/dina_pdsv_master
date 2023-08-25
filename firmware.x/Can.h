#ifndef     CAN_H
#define     CAN_H

#include <stdint.h>
#include "main.h"
#include "Frequenz.h"
#include "Uart.h"

#define NUM_CAN_BUFFERS 16
#define kCanZyklus      30      //entspricht 300ms
#define kMaxCanRate     10      //entspricht 10ms
#define kCanTimeOut     30      //entspricht 3Sek
#define CanRxFifo2      C1FIFOSTA2bits.TFNRFNIF
#define EmkVerst        LATBbits.LATB13
#define EmkVerstSchwelle 86     //entspricht 86mVss
#define VerstGross       1      //mit 1 wird der Analogschalter eingeschaltet
#define VerstKlein       0      //mit 0 wird der Analogschalter ausgeschaltet
#define Verst_klein      3
#define Verst_gross     23

//CAN RX/TX Message ID
typedef struct _CAN_MSGOBJ_ID
{   unsigned SID:11;
    unsigned long EID:18;
    unsigned SID11:1;
    unsigned unimplemented1:2;
} CANFD_MSGOBJ_ID;

//CANFD RX Message Object Control
typedef struct _CAN_RX_MSGOBJ_CTRL
{   unsigned DLC:4;
    unsigned IDE:1;
    unsigned RTR:1;
    unsigned BRS:1;
    unsigned FDF:1;
    unsigned ESI:1;
    unsigned unimplemented1:2;
    unsigned FilterHit:5;
    unsigned unimplemented2:16;
} CANFD_RX_MSGOBJ_CTRL;

//CANFD RX Message Object.
typedef union _CAN_RX_MSGOBJ
{   struct
    {   CANFD_MSGOBJ_ID id;
        CANFD_RX_MSGOBJ_CTRL ctrl;
    } bF;
    uint16_t word[8];
    uint8_t byte[16]; //Die letzten 8 Byte sind die Datenbyte
} CANFD_RX_MSGOBJ;

//CAN TX Message Object Control
typedef struct _CANFD_TX_MSGOBJ_CTRL
{   unsigned DLC:4;
    unsigned IDE:1;
    unsigned RTR:1;
    unsigned BRS:1;
    unsigned FDF:1;
    unsigned ESI:1;
    unsigned long SEQ:23;
} CANFD_TX_MSGOBJ_CTRL;

//CAN TX Message Object. Datenbyte sind hier nicht enthalten.
typedef union _CANFD_TX_MSGOBJ
{   struct
    {   CANFD_MSGOBJ_ID id;
        CANFD_TX_MSGOBJ_CTRL ctrl;
    } bF;
    uint16_t word[8];
    uint8_t byte[16]; //Die letzten 8 Byte sind die Datenbyte
} CANFD_TX_MSGOBJ;

void getCanBytes(void);
void stmParameter(void);
void eraseParamStruct(void);
void writeParamStruct(void);
void copyParam(void);               //Parameter kopieren, CHs bilden
//void ParamChsPruefen(void);
void CanTX0(void);
void CanTX1(void);
void triggerCanTX(void);
void CanConfig(void);

extern uint8_t CanRX[8], CanTX[8];
extern str_ipk RX, TX, LatchTX;
extern uint8_t flErrParCanCRC, flParamKomplett;
extern uint8_t fl10ms, cnt1msRoutinen, cnt100msRoutinen;
extern uint16_t IstFrqX10, FrqLimit0, FrqLimit1, FrqLimit2, FrqLimit3;
extern ParamStruct PS, Param_copy;
extern volatile uint8_t IR_CanRx;
extern volatile uint8_t Param[64], DNCO[256];
extern uint8_t DncoCopy[256];
extern uint16_t Steckplatz;
extern uint16_t EmkDelta;
extern uint8_t cntEmkPeak, EmkStatus;  //#######test#########löschen
extern volatile uint16_t __attribute__((aligned(4), address(0x1060) ))CanBuffer[];
//extern volatile uint16_t __attribute__((aligned(4), address(0x2010) ))CanBuffer[];
extern uint8_t flQuittErr;
//extern uint8_t _flQ2, sswSKR2;           //#######test#########löschen
//extern uint16_t ADC1V65, ADC3V3, ADC12V; //#######test#########löschen

#endif	/* CAN_H */         

