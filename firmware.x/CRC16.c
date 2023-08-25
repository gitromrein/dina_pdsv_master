#include <xc.h>
#include "p33CK256MP505.h"
#include "CRC16.h"
#include "Frequenz.h"
#include <stdint.h>

uint16_t getCRC16(uint8_t ParSatz)
{   uint16_t i, size = 0;
    uint16_t CRC16_Ergebnis = 0;
    uint8_t * ptrByte;              //Pointer auf Byte
    ptrByte = (uint8_t*)&CRCDATL;   //Pointer zeigt auf CRDATL Byte Adresse

    CRC16_Ergebnis = 0;
    CRCCONLbits.CRCEN = 1;
    CRCCONLbits.CRCGO = 0;
    CRCWDATL = 0x0000;
    CRCWDATH = 0x0000;          //NOTE:Non-direct Initial value fed to CRCWDAT
    CRCXORL = 0x1021;           //0x90D9;           // Generator Polynomial
    CRCXORH = 0x0000;
    CRCCONLbits.CRCISEL = 0;
    CRCCONHbits.PLEN = 15;      //Length of polynomial-1"
    CRCCONHbits.DWIDTH = 7;     //Länge der Daten -1
    CRCCONLbits.LENDIAN = 0;
    IFS3bits.CRCIF = 0;         //CRC status Flag is Cleared
    CRCCONLbits.CRCGO = 1;      //Stattet die CRC Kalkulation
    
    //Entweder ZMV-Param oder Laufz.Param, DNCO kommt jeweils dazu
    if(ParSatz == ZMV_ParSatz)
    {   size = SizeZMV_Par;
        for(i=0; i < size; i++)
        {   while(CRCCONLbits.CRCMPT != 1) { ; }    //wait
            *ptrByte = Param[i];
        }       
    }  
    else if(ParSatz == Laufz_ParSatz)
    {   size = SizePS;
        for(i=0; i < size; i++)
        {   while(CRCCONLbits.CRCMPT != 1) { ; }    //wait
            *ptrByte = PS.Bytes[i];
        }        
    }
    
    size = SizeDNCO;                
    for(i=0; i < size; i++)
    {   while(CRCCONLbits.CRCMPT != 1) { ; }        //wait
        *ptrByte = DNCO[i];
    } 
      
    
    CRCCONLbits.CRCGO = 0;
    IFS3bits.CRCIF = 0;
    *ptrByte = 0;               //weiter 2 Byte mit 0 durchschieben
    *ptrByte = 0;
    CRCCONLbits.CRCGO = 1;
    
    while(IFS3bits.CRCIF != 1) ; //warten auf IR Flag (Ergebnis fertig)
    IFS3bits.CRCIF = 0;
    //Ergebnis liegt for in CRCWDATL
    CRC16_Ergebnis = CRCWDATL;
    CRCCONLbits.CRCGO = 0;
    CRCCONLbits.CRCEN = 0;
    
    return CRC16_Ergebnis;  
}

/*
uint16_t getCRC16(ParamStruct *ptrPS, uint8_t Anzahl)
{   uint8_t i;
    static uint16_t CRC16_Ergebnis = 0;
    uint8_t * ptrByte;              //Pointer auf Byte
    //Anzahl = 64;
    ptrByte = (uint8_t*)&CRCDATL;   //Pointer zeigt auf CRDATL Byte Adresse

    CRC16_Ergebnis = 0;
    CRCCONLbits.CRCEN = 1;
    CRCCONLbits.CRCGO = 0;
    CRCWDATL = 0x0000;
    CRCWDATH = 0x0000;          //NOTE:Non-direct Initial value fed to CRCWDAT
    CRCXORL = 0x90D9;           // Generator Polynomial
    CRCXORH = 0x0000;
    CRCCONLbits.CRCISEL = 0;
    CRCCONHbits.PLEN = 15;    //Length of polynomial-1"
    CRCCONHbits.DWIDTH = 7;     //Länge der Daten -1
    CRCCONLbits.LENDIAN = 0;
    IFS3bits.CRCIF = 0;           //CRC status Flag is Cleared
    CRCCONLbits.CRCGO = 1;      //Stattet die CRC Kalkulation
    
    for(i=0;i<Anzahl;i++)       //0 bis 127
    {   while(CRCCONLbits.CRCMPT != 1) { ; } //warten bis buffer leer ist
        *ptrByte = ptrPS->Bytes[i];
    }
    CRCCONLbits.CRCGO = 0;
    IFS3bits.CRCIF = 0;
    *ptrByte = 0;
    *ptrByte = 0;
    CRCCONLbits.CRCGO = 1;
    
    while(IFS3bits.CRCIF != 1) ; //warten auf IR Flag (Ergebnis fertig)
    IFS3bits.CRCIF = 0;
    //Ergebnis liegt for in CRCWDATL
    CRC16_Ergebnis = CRCWDATL;
    CRCCONLbits.CRCGO = 0;
    CRCCONLbits.CRCEN = 0;
    
    return CRC16_Ergebnis;  
}
*/

/*
//Test CRC16 Funktion ----------------------------------------------------------
uint16_t getCRC16(void)
{   uint8_t i;
    static uint16_t CRC16_Ergebnis = 0;
    uint8_t * ptrByte;              //Pointer auf Byte
    uint8_t Anzahl = 0;
    Anzahl = 64;
    ptrByte = (uint8_t*)&CRCDATL;   //Pointer zeigt auf CRDATL Byte Adresse

    CRC16_Ergebnis = 0;
    CRCCONLbits.CRCEN = 1;
    CRCCONLbits.CRCGO = 0;
    CRCWDATL = 0x0000;
    CRCWDATH = 0x0000;          //NOTE:Non-direct Initial value fed to CRCWDAT
    CRCXORL = 0x90D9;           // Generator Polynomial
    CRCXORH = 0x0000;
    CRCCONLbits.CRCISEL = 0;
    CRCCONHbits.PLEN = 15;    //Length of polynomial-1"
    CRCCONHbits.DWIDTH = 7;     //Länge der Daten -1
    CRCCONLbits.LENDIAN = 0;
    IFS3bits.CRCIF = 0;           //CRC status Flag is Cleared

    CRCCONLbits.CRCGO = 1;      //Stattet die CRC Kalkulation
    for(i=0;i<Anzahl;i++)       //0 bis 127
    {   while(CRCCONLbits.CRCMPT != 1) { ; } //warten bis buffer leer ist
        *ptrByte = ParamTest[i];
    }
    CRCCONLbits.CRCGO = 0;
    IFS3bits.CRCIF = 0;
    *ptrByte = 0;
    *ptrByte = 0;
    CRCCONLbits.CRCGO = 1;
    
    while(IFS3bits.CRCIF != 1) ; //warten auf IR Flag (Ergebnis fertig)
    IFS3bits.CRCIF = 0;
    //Ergebnis liegt for in CRCWDATL
    CRC16_Ergebnis = CRCWDATL;
    CRCCONLbits.CRCGO = 0;
    CRCCONLbits.CRCEN = 0;
    
    return CRC16_Ergebnis;
}
*/
//------------------------------------------------------------------------------


