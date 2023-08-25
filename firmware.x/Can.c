#include <xc.h>
#include "p33CK256MP505.h"
#include <stdint.h>
#include "Can.h"
#include "main.h"
#include "Frequenz.h"
#include "CRC16.h"
#include <stdint.h>
//#include "Transistoren.h"

uint8_t triggerCanTX0 = 0, triggerCanTX1 = 0;

uint8_t cntTriggerCanTX0 = kCanZyklus; 
uint8_t cntRateCanTX0 = kMaxCanRate;
uint8_t cntTriggerCanTX1 = 35; 
uint8_t cntCanTimeOut = kCanTimeOut;
uint16_t ParamCRC16 = 0;

CANFD_TX_MSGOBJ* ptrCanTxMsg = 0;
CANFD_RX_MSGOBJ* ptrCanRxMsg = 0; 
volatile CANFD_RX_MSGOBJ* ptrCanRxMsg_IR = 0;


void getCanBytes(void)
{   uint8_t i; 
    //FIFO2 is not empty, enthält Messages bis CanRxFifo2 == 0
    while(CanRxFifo2 == 1)
    {   ptrCanRxMsg = (CANFD_RX_MSGOBJ *)C1FIFOUA2L;
        if(ptrCanRxMsg -> bF.ctrl.DLC == 2)
        {   //DLC==2, es sind 2 bytes, --> dnco index
            TX.Bytes[14] = ptrCanRxMsg -> byte[8];
            i = ptrCanRxMsg -> byte[9];                //dummy
        }
        else if(ptrCanRxMsg -> bF.ctrl.DLC == 8)
        {   //DLC==8, es sind 8 CanDaten bytes
            for(i=0;i<8;i++)
            {   CanRX[i] = ptrCanRxMsg -> byte[8+i];            
            }         
        } 
        C1FIFOCON2Lbits.UINC = 1;   //FIFO2 Zeiger auf nächste Message
        TX.Bytes[2] = (CanRX[3] & 0x0F);       //SerTX[2] Betriebsarten
        TX.Bytes[2] |= (CanRX[4] & 0x30);         //SerTX[2] Validierbits
        
        if((CanRX[0] & 0x40) != 0) stmParameter();
        __builtin_nop();
        __builtin_nop();
        
        cntCanTimeOut = kCanTimeOut;
    }
}

void CanTX0(void)
{   uint16_t i, notiz;
    if(C1FIFOSTA1bits.TFNRFNIF == 1)
    {   //TX FIFO1 ist nicht voll
        ptrCanTxMsg = (CANFD_TX_MSGOBJ *)C1FIFOUA1L;    //TX FIFO1 Adresse
        notiz = (((uint16_t)(Steckplatz + 0x80)) << 3); //ID Steckplatz + 0.
        ptrCanTxMsg -> bF.id.SID = notiz;               //ID Steckplatz + 0.
        ptrCanTxMsg -> bF.id.EID = 0;
        ptrCanTxMsg -> bF.ctrl.DLC = 8;                 //8 bytes
        ptrCanTxMsg -> bF.ctrl.IDE = 0;                 //Standard frame
        ptrCanTxMsg -> bF.ctrl.RTR = 0;
        ptrCanTxMsg -> bF.ctrl.BRS = 0 ;                //no Switch bit rate
        ptrCanTxMsg -> bF.ctrl.FDF = 0;                 //no CANFD frame
        for(i=0;i<8;i++)
        {   ptrCanTxMsg -> byte[8+i] = CanTX[i];       
        }
        //UINC und TXREQ setzen. Dadurch wird sichergestellt, dass während einer 
        //laufenden Übertragung auch die neue Message übertragen wird.
        C1FIFOCON1L |= 0x0300;  
        cntTriggerCanTX0 = kCanZyklus;
        cntRateCanTX0 = kMaxCanRate;
        triggerCanTX0 = 0;
    }   
}   

void CanTX1(void)
{   uint16_t i, notiz;
    if(C1FIFOSTA1bits.TFNRFNIF == 1)
    {   //TX FIFO1 ist nicht voll
        ptrCanTxMsg = (CANFD_TX_MSGOBJ *)C1FIFOUA1L;    //TX FIFO1 Adresse
        notiz = ((((uint16_t)(Steckplatz + 0x80)) << 3)+1); //ID Steckplatz + 1.
        ptrCanTxMsg -> bF.id.SID = notiz;               //ID Steckplatz + 1.
        ptrCanTxMsg -> bF.id.EID = 0;
        ptrCanTxMsg -> bF.ctrl.DLC = 8;                 //8 bytes
        ptrCanTxMsg -> bF.ctrl.IDE = 0;                 //Standard frame
        ptrCanTxMsg -> bF.ctrl.RTR = 0;
        ptrCanTxMsg -> bF.ctrl.BRS = 0 ;                //no Switch bit rate
        ptrCanTxMsg -> bF.ctrl.FDF = 0;                 //no CANFD frame

        //CAN Byte 1,2,3,4
        if(PS.Par.DrehzahlMode == 0)                    //deaktivierte ÜW            
        {   ptrCanTxMsg -> byte[8] = 0;
            ptrCanTxMsg -> byte[9] = 0;
            ptrCanTxMsg -> byte[10] = 0;
            ptrCanTxMsg -> byte[11] = 0;
        }
        else if(PS.Par.DrehzahlMode < 3)                //DZ-ÜW
        {   ptrCanTxMsg -> byte[8] = (FrqLimit1 >> 8);  //DZ
            ptrCanTxMsg -> byte[9] = FrqLimit1;
            ptrCanTxMsg -> byte[10] = (FrqLimit0 >> 8); //Stillstand
            ptrCanTxMsg -> byte[11] = FrqLimit0;
        }
        else if(PS.Par.DrehzahlMode < 7)                //Fenster-ÜW
        {   ptrCanTxMsg -> byte[8] = (FrqLimit2 >> 8);  //Fenster Max
            ptrCanTxMsg -> byte[9] = FrqLimit2;
            ptrCanTxMsg -> byte[10] = (FrqLimit1 >> 8); //Fenster Min
            ptrCanTxMsg -> byte[11] = FrqLimit1;
        }
        else if(PS.Par.DrehzahlMode == 7)               //Bereichs-ÜW
        {   ptrCanTxMsg -> byte[8] = (FrqLimit3 >> 8);  //Bereich Max
            ptrCanTxMsg -> byte[9] = FrqLimit3;
            ptrCanTxMsg -> byte[10] = (FrqLimit0 >> 8); //Bereich Min
            ptrCanTxMsg -> byte[11] = FrqLimit0;
        }
        //CAN Byte 5,6,7,8           
        ptrCanTxMsg -> byte[12] = (IstFrqX10 >> 8);     //10fache IstFrequenz
        ptrCanTxMsg -> byte[13] = IstFrqX10;  
        
        //Aus EMK Messwert den Spannungewert berechnen      
        if(EmkVerst == VerstKlein)
        {   notiz = (((uint32_t)EmkDelta * 100)/89);    //###test### *100/93
            ptrCanTxMsg -> byte[14] = (notiz >> 8);     //EMK bei Verst.3
            ptrCanTxMsg -> byte[15] = notiz;
        }
        else //VerstGross
        {   notiz = (((uint32_t)EmkDelta * 100)/681);   //###test### *100/713
            ptrCanTxMsg -> byte[14] = (notiz >> 8);     //EMK bei Verst.23
            ptrCanTxMsg -> byte[15] = notiz;               
        } 
 
        //ptrCanTxMsg -> byte[14] = (PS.Par.FrqVglSchw >> 8);     //###test###
        //ptrCanTxMsg -> byte[15] = PS.Par.FrqVglSchw;            //###test###
        
/*      ptrCanTxMsg -> byte[12] = EmkStatus;     //EMK Peakmessung###test###
        ptrCanTxMsg -> byte[13] = cntEmkPeak;
        ptrCanTxMsg -> byte[14] = (EmkDelta >> 8);     //EMK###test###
        ptrCanTxMsg -> byte[15] = EmkDelta;            //EMK###test###
*/
/* 
        ptrCanTxMsg -> byte[8] = 0;
        ptrCanTxMsg -> byte[9] = 0;
                  
        ptrCanTxMsg -> byte[10] = ADC1V65 >> 8;
        ptrCanTxMsg -> byte[11] = ADC1V65;
                  
        ptrCanTxMsg -> byte[12] = ADC3V3 >> 8;
        ptrCanTxMsg -> byte[13] = ADC3V3;
                  
        ptrCanTxMsg -> byte[14] = ADC12V >> 8;
        ptrCanTxMsg -> byte[15] = ADC12V;  
*/
        
        //UINC und TXREQ setzen. Dadurch wird sichergestellt, dass während einer 
        //laufenden Übertragung auch die neue Message übertragen wird.
        C1FIFOCON1L |= 0x0300;  
        //Transmit Trigger counter mit Transmit-Zyklus Parameter aufziehen
        i = Param[16];
        if((i > 4) && (i < 51)) cntTriggerCanTX1 = i;    //5 = 50ms und 50 = 500ms
        else                    cntTriggerCanTX1 = 30;   //30 = 300ms
        triggerCanTX1 = 0;
    }
}

void triggerCanTX(void)
{   static uint8_t CanTX0alt[8] = { 0,0,0,0, 0,0,0,0 };
    
    cnt1msRoutinen ++;
    //CanTX0 triggern wenn Daten sich geändert haben oder 300ms um sind
    if(cntRateCanTX0 > 0)  cntRateCanTX0 --;
    if(cntRateCanTX0 == 0)
    {   if((CanTX0alt[1] != CanTX[1])||(CanTX0alt[2] != CanTX[2])||(CanTX0alt[3] != CanTX[3]))
        {   triggerCanTX0 = 1; 
            cntRateCanTX0 = kMaxCanRate;
            CanTX0alt[1] = CanTX[1];
            CanTX0alt[2] = CanTX[2];
            CanTX0alt[3] = CanTX[3];                       
        }
    }
    //CanTX0 triggern wenn 300ms um sind
    if(fl10ms == 1)
    {   if(cntTriggerCanTX0 > 0)    cntTriggerCanTX0 --;
        if(cntTriggerCanTX0 == 0)  triggerCanTX0 = 1; 
        if(flParamKomplett == 1)
        {       if(cntTriggerCanTX1 > 0)    cntTriggerCanTX1 --;
                if(cntTriggerCanTX1 == 0)  triggerCanTX1 = 1;               
        }
    }           
    //Wenn triggerCanTX0==1, dann triggerCanTX1 um 1ms verzögern.
    if((triggerCanTX0 == 0) && (triggerCanTX1 == 1)) 
        CanTX1();
    if(triggerCanTX0 == 1)  
        CanTX0();
}


//Statemachine Parameter
void stmParameter(void)
{
    static uint8_t stmParam = 0;            //StateMachine neue Parameter
    uint16_t i = 0;
    
    switch(stmParam)
    {   case 0:
        /*if((CanRX[0] & 0x40) == 0)
        {   //Byte0.bit6 == 0, keine neuen Parameter
            stmParam = 0;
        } */
        if((CanTX[0] & 0x40) == 0)
        {   //CanRX[0]bit6 == 1, neue Parameter stehen an, bit6 setzen
            CanTX[0] |= 0x40;
            /*alte Parameter löschen*/
            for(i = 0; i < 32; i++) Param[i] = 0;   //###test###Achtung 32 war 64!!!!#############
            stmParam = 1;
        }
        break;
        case 1:
        if(((CanRX[0] & 0x40) != 0) && ((CanTX[0] & 0x40) != 0))
        {	//CanRX.6==1, CanTX.6==1: Speichern der Param
            if(CanRX[5] < 12) 				     //von 0-7 ok
            {	i = CanRX[5];                                   //pro indexwert 2 Parameterwerte
                i = i << 1;                                           // x2
                Param[i] =	 CanRX[6];             //CanRX-byte 6  in Parametertabelle
                Param[i+1] = CanRX[7];		    //CanRX-byte 7  in Parametertabelle
            }
            if(CanRX[5] == 11)
            {	//alle Parameter gespeichert
                CanTX[0] &= 0xBF;		//bit6=0;
                stmParam = 0;
                                
                ParamCRC16 = getCRC16(ZMV_ParSatz);
                if(ParamCRC16 != (((uint16_t)Param[54]<<8) + Param[55]))
                {   TX.bf.ErrParCanCRC = 1;                    
                }
                
                writeParamStruct(); //Parameter je nach Funktionsmode
                ParamCRC16 = getCRC16(Laufz_ParSatz);
                TX.Bytes[16] = (ParamCRC16 >> 8);
                TX.Bytes[17] = ParamCRC16;
                copyParam();  //für getSoftErr()
                TX.bf.ParamKompl = 1;

                //Alle Parameter wurden übertragen,
                //jetzt eventuell CAN Filter der Parameter-Messages sperren,
                //damit keine Parameter fehlerhafterweise überschrieben werden.
                //C1FLTCON0Lbits.FLTEN1 = 0; //Filter Standard ID+1 disabled
                //C1FLTCON0Hbits.FLTEN2 = 0; //Filter Standard ID+2 disabled
                //C1FLTCON0Hbits.FLTEN3 = 0; //Filter Standard ID+3 disabled
                //C1FLTCON1Lbits.FLTEN4 = 0; //Filter Standard ID+4 disabled                
            }
        }
        break;
    }
}

void writeParamStruct(void)
{   uint32_t notiz;
    uint8_t i, CanDzMode = 0;

    //Parameterstruktur löschen
    for(i = 0; i < SizePS; i++)
    {    PS.Bytes[i] = 0;
    }
    //Parameterstruktur je nach Modus initialisieren
    
    //###test#####################löschen#####################
    //Param[50] = 0x02;   //Fensterüw.
    //Param[50] = 0x82;   //Automatisch, Fensterüw.
    //Param[50] = 0x03;   //Bereichsüw
    //###test#####################löschen#####################
    
    CanDzMode = Param[50] & 0x0F;
    
    if(CanDzMode == 1)
    {   //Stillstand- und DZ-ÜW
        //Stillstand
        notiz = ((((uint16_t)Param[32]) << 8) + Param[33]);         //Limit0, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        //notiz = 50;    //###test###löschen 5Hz#####
        PS.Par.arrFrqLimits[0][0] = (uint16_t)notiz;              //Limit0
        PS.Par.arrFrqHyst[0][0] =   (uint16_t)((notiz * 90)/100); //Limit0 x 0,90
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[0][0] = (uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[0][0] = 10;
        //Drehzahl F1                       
        notiz = ((((uint16_t)Param[34]) << 8) + Param[35]);         //Limit1, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        //notiz = 500;    //###test###löschen 50Hz#####
        PS.Par.arrFrqLimits[1][0] = (uint16_t)notiz;                                                //Limit1
        PS.Par.arrFrqLimits[1][1] = (uint16_t)((notiz * 90)/100);                                   //Limit1 x 0,90  //Validierung1
        PS.Par.arrFrqLimits[1][2] = (uint16_t)((notiz * 85)/100);                                   //Limit1 x 0,85  //Validierung2
        PS.Par.arrFrqLimits[1][3] = (uint16_t)((notiz * 80)/100);                                   //Limit1 x 0,80  //Validierung3
        PS.Par.arrFrqHyst[1][0] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[1][0]) * 90)/100);   //Limit1 x 0,90
        PS.Par.arrFrqHyst[1][1] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[1][1]) * 90)/100);   //Limit1 x 0,90  //Validierung1
        PS.Par.arrFrqHyst[1][2] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[1][2]) * 90)/100);   //Limit1 x 0,90  //Validierung2
        PS.Par.arrFrqHyst[1][3] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[1][3]) * 90)/100);   //Limit1 x 0,90  //Validierung3
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[1][0]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[1][0] = 10;
        notiz =           PS.Par.arrFrqLimits[1][1];
        if(notiz < 130) {PS.Par.arrFrqTimeout[1][1]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[1][1] = 10; //Validierung1
        notiz =           PS.Par.arrFrqLimits[1][2];
        if(notiz < 130) {PS.Par.arrFrqTimeout[1][2]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[1][2] = 10; //Validierung2
        notiz =           PS.Par.arrFrqLimits[1][3];
        if(notiz < 130) {PS.Par.arrFrqTimeout[1][3]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[1][3] = 10; //Validierung3
        //Drehzahl F2
        notiz = ((((uint16_t)Param[36]) << 8) + Param[37]);         //Limit2, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        //notiz = 1000;    //###test###löschen 100Hz#####
        PS.Par.arrFrqLimits[2][0] = (uint16_t)notiz;                                                //Limit2
        PS.Par.arrFrqLimits[2][1] = (uint16_t)((notiz * 90)/100);                                   //Limit2 x 0,90  //Validierung1
        PS.Par.arrFrqLimits[2][2] = (uint16_t)((notiz * 85)/100);                                   //Limit2 x 0,85  //Validierung2
        PS.Par.arrFrqLimits[2][3] = (uint16_t)((notiz * 80)/100);                                   //Limit2 x 0,80  //Validierung3
        PS.Par.arrFrqHyst[2][0] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[2][0]) * 90)/100);   //Limit2 x 0,90
        PS.Par.arrFrqHyst[2][1] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[2][1]) * 90)/100);   //Limit2 x 0,90  //Validierung1
        PS.Par.arrFrqHyst[2][2] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[2][2]) * 90)/100);   //Limit2 x 0,90  //Validierung2
        PS.Par.arrFrqHyst[2][3] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[2][3]) * 90)/100);   //Limit2 x 0,90  //Validierung3
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[2][0]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[2][0] = 10;
        notiz =           PS.Par.arrFrqLimits[2][1];
        if(notiz < 130) {PS.Par.arrFrqTimeout[2][1]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[2][1] = 10; //Validierung1
        notiz =           PS.Par.arrFrqLimits[2][2];
        if(notiz < 130) {PS.Par.arrFrqTimeout[2][2]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[2][2] = 10; //Validierung2
        notiz =           PS.Par.arrFrqLimits[2][3];
        if(notiz < 130) {PS.Par.arrFrqTimeout[2][3]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[2][3] = 10; //Validierung3
        //Drehzahl F3
        notiz = ((((uint16_t)Param[38]) << 8) + Param[39]);         //Limit3, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        //notiz = 2000;    //###test###löschen 200Hz#####
        PS.Par.arrFrqLimits[3][0] = (uint16_t)notiz;                                                //Limit3
        PS.Par.arrFrqLimits[3][1] = (uint16_t)((notiz * 90)/100);                                   //Limit3 x 0,90  //Validierung1
        PS.Par.arrFrqLimits[3][2] = (uint16_t)((notiz * 85)/100);                                   //Limit3 x 0,85  //Validierung2
        PS.Par.arrFrqLimits[3][3] = (uint16_t)((notiz * 80)/100);                                   //Limit3 x 0,80  //Validierung3
        PS.Par.arrFrqHyst[3][0] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[3][0]) * 90)/100);   //Limit1 x 0,90
        PS.Par.arrFrqHyst[3][1] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[3][1]) * 90)/100);   //Limit3 x 0,90  //Validierung1
        PS.Par.arrFrqHyst[3][2] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[3][2]) * 90)/100);   //Limit3 x 0,90  //Validierung2
        PS.Par.arrFrqHyst[3][3] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[3][3]) * 90)/100);   //Limit3 x 0,90  //Validierung3
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[3][0]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[3][0] = 10;
        notiz =           PS.Par.arrFrqLimits[3][1];
        if(notiz < 130) {PS.Par.arrFrqTimeout[3][1]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[3][1] = 10; //Validierung1
        notiz =           PS.Par.arrFrqLimits[3][2];
        if(notiz < 130) {PS.Par.arrFrqTimeout[3][2]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[3][2] = 10; //Validierung2
        notiz =           PS.Par.arrFrqLimits[3][3];
        if(notiz < 130) {PS.Par.arrFrqTimeout[3][3]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[3][3] = 10; //Validierung3
        //Plausibilitäts-Vergleich: > 1200Hz, Abstand von Fenster-Min/Max, welche Validierung ist möglich.
        //Bei Nicht Plausibilität LimitParamErr setzen
        if((PS.Par.arrFrqLimits[0][0] > 12000)
        || (PS.Par.arrFrqLimits[1][0] > 12000)
        || (PS.Par.arrFrqLimits[2][0] > 12000)
        || (PS.Par.arrFrqLimits[3][0] > 12000))
        {   //Fehler: Frequenzlimits > 1200Hz
            TX.bf.ErrParPlaus = 1;
        }
        PS.Par.Validierstufen = 3;
    }
    else if(CanDzMode == 2)
    {   //Fenster-ÜW Stillstand
        notiz = ((((uint16_t)Param[32]) << 8) + Param[33]);         //Limit0, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        PS.Par.arrFrqLimits[0][0] = (uint16_t)notiz;              //Limit0
        PS.Par.arrFrqHyst[0][0] =   (uint16_t)((notiz * 90)/100); //Limit0 x 0,90
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[0][0] = (uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[0][0] = 10;
        //Fenster Frq-Min: 
        //Validierwerte und Hysterese gehen Richtung höhere Frequenz, also umgekehrt wie bei den Frq-Max Werten
        notiz = ((((uint16_t)Param[34]) << 8) + Param[35]);         //Limit1, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        PS.Par.arrFrqLimits[1][0] = (uint16_t)notiz;                                                //Limit1
        PS.Par.arrFrqLimits[1][1] = (uint16_t)((notiz * 110)/100);                                  //Limit1 x 1,10
        PS.Par.arrFrqLimits[1][2] = (uint16_t)((notiz * 115)/100);                                  //Limit1 x 1,15
        PS.Par.arrFrqLimits[1][3] = (uint16_t)((notiz * 120)/100);                                  //Limit1 x 1,20
        PS.Par.arrFrqHyst[1][0] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[1][0]) * 110)/100);  //x 1,10
        PS.Par.arrFrqHyst[1][1] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[1][1]) * 110)/100);  //x 1,10
        PS.Par.arrFrqHyst[1][2] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[1][2]) * 110)/100);  //x 1,10
        PS.Par.arrFrqHyst[1][3] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[1][3]) * 110)/100);  //x 1,10
        //Timeout Wert entspricht der Periodenzeit x 1,3 + 100ms
        if(notiz < 130) {PS.Par.arrFrqTimeout[1][0]=(((uint16_t)1300/notiz) + 10);} else PS.Par.arrFrqTimeout[1][0] = 20;
        notiz =           PS.Par.arrFrqLimits[1][1];
        if(notiz < 130) {PS.Par.arrFrqTimeout[1][1]=(((uint16_t)1300/notiz) + 10);} else PS.Par.arrFrqTimeout[1][1] = 20;
        notiz =           PS.Par.arrFrqLimits[1][2];
        if(notiz < 130) {PS.Par.arrFrqTimeout[1][2]=(((uint16_t)1300/notiz) + 10);} else PS.Par.arrFrqTimeout[1][2] = 20;
        notiz =           PS.Par.arrFrqLimits[1][3];
        if(notiz < 130) {PS.Par.arrFrqTimeout[1][3]=(((uint16_t)1300/notiz) + 10);} else PS.Par.arrFrqTimeout[1][3] = 20;
        //Fenster Frq-Max
        notiz = ((((uint16_t)Param[36]) << 8) + Param[37]);         //Limit2
        if(notiz == 0) notiz = 1;
        PS.Par.arrFrqLimits[2][0] = (uint16_t)notiz;                                                //Limit2, 10-fache Frequenz
        PS.Par.arrFrqLimits[2][1] = (uint16_t)((notiz * 90)/100);                                   //Limit2 x 0,90
        PS.Par.arrFrqLimits[2][2] = (uint16_t)((notiz * 85)/100);                                   //Limit2 x 0,85
        PS.Par.arrFrqLimits[2][3] = (uint16_t)((notiz * 80)/100);                                   //Limit2 x 0,80
        PS.Par.arrFrqHyst[2][0] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[2][0]) * 90)/100);   //x 0,90
        PS.Par.arrFrqHyst[2][1] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[2][1]) * 90)/100);   //x 0,90
        PS.Par.arrFrqHyst[2][2] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[2][2]) * 90)/100);   //x 0,90
        PS.Par.arrFrqHyst[2][3] =   (uint16_t)((((uint32_t)PS.Par.arrFrqLimits[2][3]) * 90)/100);   //x 0,90
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[2][0]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[2][0] = 10;
        notiz =           PS.Par.arrFrqLimits[2][1];
        if(notiz < 130) {PS.Par.arrFrqTimeout[2][1]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[2][1] = 10;
        notiz =           PS.Par.arrFrqLimits[2][2];
        if(notiz < 130) {PS.Par.arrFrqTimeout[2][2]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[2][2] = 10;
        notiz =           PS.Par.arrFrqLimits[2][3];
        if(notiz < 130) {PS.Par.arrFrqTimeout[2][3]=(uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[2][3] = 10; 
        //Plausibilitäts-Vergleich: > 1200Hz, Abstand von Fenster-Min/Max, welche Validierung ist möglich.
        //Bei Nicht Plausibilität LimitParamErr setzen
        if((PS.Par.arrFrqLimits[0][0] > 12000)  //>1200Hz
        || (PS.Par.arrFrqLimits[1][0] > 12000)
        || (PS.Par.arrFrqLimits[2][0] > 12000)
        || (((((uint32_t)PS.Par.arrFrqHyst[1][0]) *110)/100) >= PS.Par.arrFrqHyst[2][0]))
        {   //Fehler: Frq-min.Hyst * 1,1 >= Frq-max-Hyst bzw. Frequenzen > 1200Hz 
            TX.bf.ErrParPlaus = 1;
        }
        //maximale Validierungsstufe ermitteln
        if(     PS.Par.arrFrqHyst[2][3] > PS.Par.arrFrqHyst[1][3]) PS.Par.Validierstufen = 3;
        else if(PS.Par.arrFrqHyst[2][2] > PS.Par.arrFrqHyst[1][2]) PS.Par.Validierstufen = 2;
        else if(PS.Par.arrFrqHyst[2][1] > PS.Par.arrFrqHyst[1][2]) PS.Par.Validierstufen = 1;
        else                                                       PS.Par.Validierstufen = 0;
    }
    else if(CanDzMode == 3)
    {   //Bereichs-ÜW.
        //Limit0
        notiz = ((((uint16_t)Param[32]) << 8) + Param[33]);         //Limit0, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        PS.Par.arrFrqLimits[0][0] = (uint16_t)notiz;                //Limit0
        PS.Par.arrFrqHyst[0][0] =   (uint16_t)((notiz * 90)/100);   //Limit0 x 0,90
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[0][0] = (uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[0][0] = 10;
        //Bereichs-ÜW Limit1
        notiz = ((((uint16_t)Param[34]) << 8) + Param[35]);         //Limit1, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        PS.Par.arrFrqLimits[1][0] = (uint16_t)notiz;                //Limit1
        PS.Par.arrFrqHyst[1][0] =   (uint16_t)((notiz * 90)/100); //Limit1 x 0,90
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[1][0] = (uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[1][0] = 10;
        //Bereichs-ÜW Limit2
        notiz = ((((uint16_t)Param[36]) << 8) + Param[37]);         //Limit2, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        PS.Par.arrFrqLimits[2][0] = (uint16_t)notiz;                //Limit2
        PS.Par.arrFrqHyst[2][0] =   (uint16_t)((notiz * 90)/100); //Limit2 x 0,90
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[2][0] = (uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[2][0] = 10;
        //Bereichs-ÜW Limit3
        notiz = ((((uint16_t)Param[38]) << 8) + Param[39]);         //Limit3, 10-fache Frequenz
        if(notiz == 0) notiz = 1;
        PS.Par.arrFrqLimits[3][0] = (uint16_t)notiz;                //Limit3
        PS.Par.arrFrqHyst[3][0] =   (uint16_t)((notiz * 90)/100); //Limit3 x 0,90
        //Timeout Wert entspricht der Periodenzeit x 1,3.
        if(notiz < 130) {PS.Par.arrFrqTimeout[3][0] = (uint16_t)1300/notiz;} else PS.Par.arrFrqTimeout[3][0] = 10;
        //Plausibilität der Bereiche prüfen
        if((PS.Par.arrFrqLimits[0][0] >= PS.Par.arrFrqHyst[1][0])
        || (PS.Par.arrFrqLimits[1][0] >= PS.Par.arrFrqHyst[2][0])
        || (PS.Par.arrFrqLimits[2][0] >= PS.Par.arrFrqHyst[3][0]))
        {   //Fehler: Bereichsgrenzen überlappen sich oder sind zu eng beieinander  
            TX.bf.ErrParPlaus = 1;
        }
        //Validierung macht bei der BereichsÜW keinen Sinn
        PS.Par.Validierstufen = 0;
    }
    if(CanDzMode > 0)
    {   //weitere Parameter   
        PS.Par.FrqSchwelle = ((((uint16_t)Param[40]) << 8) + Param[41]);
        PS.Par.Anlaufzeit = ((((uint16_t)Param[42]) << 8) + Param[43]);
        if(PS.Par.Anlaufzeit > 12000) PS.Par.Anlaufzeit = 12000;                //auf 120s begrenzen      
        PS.Par.Abschaltverz = Param[48];            //0-255 x 10ms. Max 2,55s
        PS.Par.XfachMessung = ((Param[49] & 0x03)+1);                           //0 = 1x Messung, ..., 3 = 4x Messung
        //if((PS.Par.XfachMessung < 1)||(PS.Par.XfachMessung > 4)) PS.Par.XfachMessung = 1;       //############test##############
        PS.Par.Hysteresemode = ((Param[49] & 0x0C) >>2);                        //Messempfindlichkeit
        //PS.Par.AustrudelUW = ((Param[49] & 0x70) >>4);
        PS.Par.PhasenUW = ((Param[49] & 0x80) >>7);
        PS.Par.ProzentAbschVerz = Param[51];           
        if(PS.Par.ProzentAbschVerz > 100) TX.bf.ErrParPlaus = 1;    //auf 100% begrenzen            
        PS.Par.FrqVglSchw = (((uint16_t)Param[52])*10);
        if(PS.Par.FrqVglSchw < 20) TX.bf.ErrParPlaus = 1;           //2Hz also 20 ist Minimum
        
        //PS.Par.Funktionsmode = mode;
        if(CanDzMode == 1)                                          //St-DZ-ÜW
        {   if((Param[50]& 0x80)== 0)      PS.Par.DrehzahlMode = 1; //manuell
            else                           PS.Par.DrehzahlMode = 2; //autom        
        }
        else if(CanDzMode == 2)                                     //FensterÜW
        {   if((Param[50]& 0x80)== 0)                               //manuell/autom
            {   if(PS.Par.Anlaufzeit == 0) PS.Par.DrehzahlMode = 3; //manuell, ohne AnlaufÜW
                else                       PS.Par.DrehzahlMode = 4; //manuell, mit AnlaufÜW 
            }
            else
            {   if(PS.Par.Anlaufzeit == 0) PS.Par.DrehzahlMode = 5; //autom, ohne AnlaufÜW
                else                       PS.Par.DrehzahlMode = 6; //autom, mit AnlaufÜW
            }
        }
        else if(CanDzMode == 3)            PS.Par.DrehzahlMode = 7; //BereichsÜW,                                                            
        
        PS.Par.ControlBits = Param[10];  //mode 1+2
        PS.Par.DNCO = Param[12];        //mode 1
    }
    
    if((PS.Par.DrehzahlMode == 7)&&(PS.Par.Abschaltverz > 0))
    {   TX.bf.ErrParPlaus = 1;      
    }
    
    PS.Par.EmkMode = ((Param[50] & 0x70) >>4);
    if(PS.Par.EmkMode == 1)
    {   PS.Par.EMKEmpfkeit = ((((uint16_t)Param[44]) << 8) + Param[45]);
        //Außerhalb von 10 bis 700mV?
        if((PS.Par.EMKEmpfkeit < 10)||(PS.Par.EMKEmpfkeit > 700))
        {   TX.bf.ErrParPlaus = 1;
        }
        if(PS.Par.EMKEmpfkeit > EmkVerstSchwelle) 
        {   //Schaltungsbedingte Verstärkung = 3 (klein)
            //EMKEinschaltschw = EMKEmpfkeit * Verst.* ADC-Auflösung / 3300mV.  Verst.3*1023/3300*955/1000 = 0,89. 
            //EMKEinschaltschw = EMKEmpfkeit *  3    *  1023         / 3300
            //EMKEinschaltschw = EMKEmpfkeit * 0,93 * 0,955 Korrektur
            //EMKAbschaltschw = EMKEinschaltschw * 1,7
            EmkVerst = VerstKlein;           
            PS.Par.EMKEinschaltschw = (((((((uint32_t)PS.Par.EMKEmpfkeit)*Verst_klein)*1023)/3300)*955)/1000);
            PS.Par.EMKAbschaltschw = ((PS.Par.EMKEinschaltschw *17)/10);
            if(PS.Par.EMKAbschaltschw > 1000) PS.Par.EMKAbschaltschw = 1000;              //begrenzen auf 3,26V.
        }
        else
        {   //Schaltungsbedingte Verstärkung = 23 (groß)
            //EMKEinschaltschw = EMKEmpfkeit * Verst.* ADC-Auflösung / 3300mV.  Verst.23*1023/3300*955/1000 = 6,81
            //EMKEinschaltschw = EMKEmpfkeit *  23   *  1023         / 3300
            //EMKEinschaltschw = EMKEmpfkeit * 7,13 * 0,955 Korrektur
            //EMKAbschaltschw = EMKEinschaltschw * 1,7           
            EmkVerst = VerstGross;          //Schaltungsbedingte Verstärkung = 23
            PS.Par.EMKEinschaltschw = (((((((uint32_t)PS.Par.EMKEmpfkeit)*Verst_gross)*1023)/3300)*955)/1000);
            PS.Par.EMKAbschaltschw = ((PS.Par.EMKEinschaltschw *17)/10);                  //###test### Faktor 1,7 statt 3/2
            if(PS.Par.EMKAbschaltschw > 1000) PS.Par.EMKAbschaltschw = 1000;              //begrenzen auf 3,26V.
        }
    
        PS.Par.EMKEinschaltverz = ((((uint16_t)Param[46]) << 8) + Param[47]);
        if(PS.Par.EMKEinschaltverz > 3000) 
        {   TX.bf.ErrParPlaus = 1;            
        }     
    }
    else
    {   PS.Par.EMKEmpfkeit = 0;
        PS.Par.EMKEinschaltschw = 0;
        PS.Par.EMKAbschaltschw = 0;
        PS.Par.EMKEinschaltverz = 0;       
    }
    
    //Eingänge und SKRs
    PS.Par.Entprellung = Param[0];
    PS.Par.SkrQSErk = Param[2];
    PS.Par.SkrAutoMan = Param[3];
    PS.Par.SkrQDelay = Param[4];
    PS.Par.SkrAntival = Param[5];
    PS.Par.SkrPowerOn = Param[6];
    PS.Par.SkrPegel = Param[7];
    PS.Par.SkrAktiv = Param[8];
    PS.Par.CanRefresh = Param[16];
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Die Parameterstruktur wird kopiert (für SoftErr Routine).
//  Die Checksum wird ermittelt und auf die IPK geschrieben.
//------------------------------------------------------------------------------
void copyParam(void)
{   uint16_t i;
    for(i=0; i< SizePS; i++)
    {   Param_copy.Bytes[i] = PS.Bytes[i];    
    }
    for(i=0; i < 256; i++)
    {   DncoCopy[i] = DNCO[i];       
    }
}
//------------------------------------------------------------------------------


void CanConfig(void)
{   //uint16_t notiz;
    //CAN Kommunikation überarbeiten###test####################
    //CAN Clock = 80MHz und IO Pin Mapping wird in init() konfiguriert.
    C1CONLbits.CON = 1;
    //Place CAN module in configuration mode
    C1CONHbits.REQOP = 4;
    while(C1CONHbits.OPMOD != 4);
    //Set up the CANFD module for 0.125Mbps, 1Bit = 8µs. 
    //Prescaler 32/80MHz = 0,4µs = 1NTQ.  20NTQ = 8µs. 
    //Synch(fester Wert)                                    =  1 NTQ
    //NTimeSegment1 = PropagationSegment + PhaseSegment1    = 12 NTQ
    //NTimeSegment2 = PhaseSegment2                         =  7 NTQ   zusammen also 20 NTQ
 //   C1NBTCFGHbits.BRP = 31;     //Baudrate Prescaler 32
 //   C1NBTCFGHbits.TSEG1 = 11;   //NTimeSegment1 (PropagationSegment + PhaseSegment1) = 12
 //   C1NBTCFGLbits.TSEG2 = 6;    //TimeSegment2 (PhaseSegment2) = 7
 //   C1NBTCFGLbits.SJW = 3;      //Synchronization Jump Width = 4
 //   C1CONHbits.TXBWS = 8;       //8 = 256 x 8µs = 2048µs Wartezeit zwischen 2 TX Telegrammen
    
    //Prescaler 2/80MHz = 25ns = 1 NTQ.  320 NTQ = 8µs. 
    //Synch(fester Wert)                                    =  1 NTQ
    //NTimeSegment1 = PropagationSegment + PhaseSegment1    = 200 NTQ
    //NTimeSegment2 = PhaseSegment2                         = 119 NTQ   zusammen also 320 NTQ
    C1NBTCFGHbits.BRP = 1;      //0: Baudrate Prescaler = 2
    C1NBTCFGHbits.TSEG1 = 199;   //NTimeSegment1 (PropagationSegment + PhaseSegment1) = 200
    C1NBTCFGLbits.TSEG2 = 118;    //TimeSegment2 (PhaseSegment2) = 119
    C1NBTCFGLbits.SJW = 118;      //Synchronization Jump Width = 119
    C1CONHbits.TXBWS = 8;       //8 = 256 x Bittime (8µs) = 2048µs Wartezeit zwischen 2 TX Telegrammen
    
    //C1DBTCFGH = 0;            //betrifft CAN FD
    //C1DBTCFGL = 0;            //betrifft CAN FD
    //C1TDCH = 0x0002;          //betrifft CAN FD. TDCMOD is Auto
    //C1TDCL = 0x0F00;          //betrifft CAN FD
    // Initialize the C1FIFOBA with the start address of the CAN FIFO message buffer area.
    C1FIFOBAH = 0;
    C1FIFOBAL = (uint16_t) &CanBuffer;  
    C1CONHbits.STEF = 0;        //Kein STEF Buffer
    C1CONHbits.TXQEN = 0;       //Kein TXQueue
    //FIFO1 TX
    C1FIFOCON1Lbits.TXEN = 1;   //FIFO1 ist TX FIFO
    C1FIFOCON1Hbits.PLSIZE = 0; //8 Datenbyte pro Message
    C1FIFOCON1Hbits.FSIZE = 3;  //FIFO Tiefe 3+1=4 Messages
    C1FIFOCON1Hbits.TXAT = 3;   //3 = unbegrenzte Sendeversuche
    
    //FIFO2 RX für ID Steckplatz +0, wird per Polling ausgelesen
    C1FIFOCON2Lbits.TXEN = 0;   //FIFO2 ist RX FIFO
    C1FIFOCON2Hbits.PLSIZE = 0; //8 Datenbyte pro Message
    C1FIFOCON2Hbits.FSIZE = 3;  //FIFO Tiefe 3+1=4 Messages
    //Filter0 für FIFO2
    C1FLTOBJ0Hbits.EXIDE = 0;                           //Filter nur für Standard IDs
    C1FLTOBJ0Lbits.SID = (((uint16_t)Steckplatz) << 3); //Filter0
    C1MASK0Hbits.MIDE = 1;                              //Maske nur für Standard ID
    C1MASK0Lbits.MSID = 0b10001111111;                  //Standard ID+0
    C1FLTCON0Lbits.F0BP = 2;                            //Filterpointer, message stored in FIFO2
    C1FLTCON0Lbits.FLTEN0 = 1;                          //Filter aktiviert
            
    //FIFO3 RX für ID Steckplatz +1-4, wird per ISR ausgelesen
    C1FIFOCON3Lbits.TXEN = 0;   //FIFO3 ist RX FIFO
    C1FIFOCON3Hbits.PLSIZE = 0; //8 Datenbyte pro Message
    C1FIFOCON3Hbits.FSIZE = 7;  //FIFO Tiefe 7+1=8 Messages
    //Filter1 für FIFO3
    C1FLTOBJ1Hbits.EXIDE = 0;                           //Filter nur für Standard IDs
    C1FLTOBJ1Lbits.SID = ((((uint16_t)Steckplatz) << 3)+1); //Filter1
    C1MASK1Hbits.MIDE = 1;                              //Maske nur für Standard ID
    C1MASK1Lbits.MSID = 0b10001111111;                  //Standard ID+0
    C1FLTCON0Lbits.F1BP = 3;                            //Filterpointer, message stored in FIFO3
    C1FLTCON0Lbits.FLTEN1 = 1;                          //Filter aktiviert
    //Filter2 für FIFO3
    C1FLTOBJ2Hbits.EXIDE = 0;                           //Filter nur für Standard IDs
    C1FLTOBJ2Lbits.SID = ((((uint16_t)Steckplatz) << 3)+2); //Filter2
    C1MASK2Hbits.MIDE = 1;                              //Maske nur für Standard ID
    C1MASK2Lbits.MSID = 0b10001111111;                  //Standard ID+0
    C1FLTCON0Hbits.F2BP = 3;                            //Filterpointer, message stored in FIFO3
    C1FLTCON0Hbits.FLTEN2 = 1;                          //Filter aktiviert
    //Filter3 für FIFO3
    C1FLTOBJ3Hbits.EXIDE = 0;                           //Filter nur für Standard IDs
    C1FLTOBJ3Lbits.SID = ((((uint16_t)Steckplatz) << 3)+3); //Filter3
    C1MASK3Hbits.MIDE = 1;                              //Maske nur für Standard ID
    C1MASK3Lbits.MSID = 0b10001111111;                  //Standard ID+0
    C1FLTCON0Hbits.F3BP = 3;                            //Filterpointer, message stored in FIFO3
    C1FLTCON0Hbits.FLTEN3 = 1;                          //Filter aktiviert
    //Filter4 für FIFO3
    C1FLTOBJ4Hbits.EXIDE = 0;                           //Filter nur für Standard IDs
    C1FLTOBJ4Lbits.SID = ((((uint16_t)Steckplatz) << 3)+4); //Filter4
    C1MASK4Hbits.MIDE = 1;                              //Maske nur für Standard ID
    C1MASK4Lbits.MSID = 0b10001111111;                  //Standard ID+0
    C1FLTCON1Lbits.F4BP = 3;                            //Filterpointer, message stored in FIFO3
    C1FLTCON1Lbits.FLTEN4 = 1;                          //Filter aktiviert
    /* //Filter5 für FIFO3
    C1FLTOBJ5Hbits.EXIDE = 0;                           //Filter nur für Standard IDs
    C1FLTOBJ5Lbits.SID = ((((uint16_t)Steckplatz) << 3)+5); //Filter5
    C1MASK5Hbits.MIDE = 1;                              //Maske nur für Standard ID
    C1MASK5Lbits.MSID = 0b10001111111;                  //Standard ID+0
    C1FLTCON1Lbits.F5BP = 3;                            //Filterpointer, message stored in FIFO3
    C1FLTCON1Lbits.FLTEN5 = 1;                          //Filter aktiviert
    */
    
    /* Interrupts konfigurieren
    Einige Infos:	
    CAN1 RX     _C1RXInterrupt IFS1[15] IEC1[15] IPC7[14:12]
    CAN1 TX     _C1TXInterrupt IFS3[4]  IEC3[4]  IPC13[2:0]
    CAN1 Errors _CAN1Interrupt IFS1[9]  IEC1[9]  IPC6[6:4]
    C1INTL.RXIF	(Read Only).  C1INTH.RXIE
    C1FIFOCON1L-7L, FIFO1-7, entsprechende Interrupt enable bits
    Status bits:
    C1RXIFL.RFIF[7:1]  FIFO 1 bis 7 pending RX in FIFOs	(Status Read Only)
    C1FIFOSTA1-7, FIFO1-7, TFNRFNIF Not full/not empty, je nach RX oder TX configuration
    FIFOCI[4:0]: FIFO Message Index bits. Zeigt auf die nächste RX Adresse
    C1INTLbits.RXIF, Read only, löst den _C1RXInterrupt() aus, muss nicht gelöscht werden
    */
    //FIFO2 wird per polling ausgelesen (C1FIFOSTA2bits.TFNRFNIF), IR nicht aktivieren
    //FIFO3 wird per IR ausgelesen, IR aktivieren
    C1FIFOCON3Lbits.TFNRFNIE = 1;   //FIFO3 (RX) not empty IR enabled
    C1INTHbits.RXIE = 1;    //CAN RX main IR flag
    IPC7bits.C1RXIP = 6;    //IR priorität
    IFS1bits.C1RXIF = 0;    //IR flag
    IEC1bits.C1RXIE = 1;    //Oberste CAN IR Ebene, _C1RXInterrupt() enabled
    /*   
    
    //setup CAN Buffer0
    Can1MsgBuf[0][0] = ((Steckplatz + 0x80) <<5 );      //SID, SRR, IDE
    Can1MsgBuf[0][1] = 0;                                                   //EID
    Can1MsgBuf[0][2] = 0x0008;                                        //DLC (Anzahl Datenbytes
    Can1MsgBuf[0][7] = 0;
    //Datenbyte
    Can1MsgBuf[0][3] = (0x0000 + Kennung);                //Byte 1 / 0
    Can1MsgBuf[0][4] = 0x0000;                                        //Byte 3 / 2
    Can1MsgBuf[0][5] = 0x0000;                                        //Byte 5 / 4
    Can1MsgBuf[0][6] = 0x0000;                                        //Byte 7 / 6
    
    //setup CAN Buffer1
    Can1MsgBuf[1][0] = ((Steckplatz + 0x80) <<5 );      //SID, SRR, IDE
    Can1MsgBuf[1][0] |= (1 << 2);                                  //SID + 1
    Can1MsgBuf[1][1] = 0;                                                   //EID
    Can1MsgBuf[1][2] = 0x0008;                                        //DLC (Anzahl Datenbytes
    Can1MsgBuf[1][7] = 0;
    //Datenbyte
    Can1MsgBuf[1][3] = 0x0000;                                         //Byte 1 / 0
    Can1MsgBuf[1][4] = 0x0000;                                        //Byte 3 / 2
    Can1MsgBuf[1][5] = 0x0000;                                        //Byte 5 / 4
    Can1MsgBuf[1][6] = 0x0000;                                        //Byte 7 / 6
    */
    C1CONHbits.REQOP = 6;           //Sets Normal CAN 2.0 mode
    while(C1CONHbits.OPMOD != 6);
}
