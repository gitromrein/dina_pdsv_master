#include <xc.h>
#include "p33CK256MP505.h"
#include "Diagnose.h"
#include "Auswertung.h"
#include <stdint.h>
//#include "Can.h"
//#include "Transistoren.h"

uint8_t stmLatchDZ1 = 0;
uint8_t flLatchTX = 0, flLatchRX = 0;
uint16_t Latch_IstDzIX_1 = 0;
uint16_t Latch_SollDzIX_1 = 0; 
uint32_t Latch_Position1 = 0;
uint8_t Latch_Betriebsarten1 = 0; 
uint8_t Latch_DzFlags1 = 0;
uint8_t Latch_flPosUW1 = 0;
//uint8_t Latch_flPositionErr1 = 0;
uint8_t Latch_flFrqIxErr1 = 0;
uint8_t LatchCH2ErrA = 0, LatchCH2ErrB = 0;
uint8_t Latch_AbschaltQuelle = 0;
uint8_t Latch_SerTOErr = 0, Latch_CanTOErr = 0, Latch_PortTestErr = 0, Latch_3V3Err = 0;
//------------------------------------------------------------------------------
//LatchDZ() wird jede 1ms aufgerufen
//------------------------------------------------------------------------------
void LatchDZ(void)
{   uint8_t i;

    if((PS.Par.DrehzahlMode > 0)&&(PS.Par.DrehzahlMode < 7))//------------------
    {   //DZ- oder Fenster-Überwachung
        switch(stmLatchDZ1)
        {   case 0:           
            //wenn das DZ bit ==1, dann weiterschalten
            if((CanTX[1] & 0x02) != 0)  stmLatchDZ1 = 1;               
            break;
            case 1:
            if((CanTX[1] & 0x02) != 0)  ; //DZ bit ==1, warten
            else
            {   //DZ bit wurde abgeschaltet, Latch Situation beginnt
                //IPK Frame latchen
                for(i = 0; i < ProtokollUart; i++)
                {   LatchTX.Bytes[i] = TX.Bytes[i];                   
                }
                LatchTX.bf.Frametyp = kLatch2;
                flLatchTX = 1;                      //wird in der IPK nach Übernahme gelöscht
                stmLatchDZ1 = 0;                    //weiterschalten
            }
            break;
            default:
            break;
        }
    }
    else if(PS.Par.DrehzahlMode == 7)//-----------------------------------------
    {   //Bereichs-Überwachung
        switch(stmLatchDZ1)
        {   case 0:
            //wenn die Bereichsbits != 0, dann weiterschalten
            if((CanTX[1] & 0x0F) != 0)  stmLatchDZ1 = 1;               
            break;
            case 1:
            if((CanTX[1] & 0x0F) != 0)  ; //Bereichsbits != 0, warten
            else
            {   //Bereichsbits alle 0, Latch Situation beginnt
                //DZ bit wurde abgeschaltet, Latch Situation beginnt
                //IPK Frame latchen
                for(i = 0; i < ProtokollUart; i++)
                {   LatchTX.Bytes[i] = TX.Bytes[i];                   
                }
                LatchTX.bf.Frametyp = kLatch2; 
                stmLatchDZ1 = 0;                    //weiterschalten
            }
            break;
            default:
            break;
        }
    }
    else //---------------------------------------------------------------------
    {   ;   //DZ Üerwachung nicht aktiv, nichts tun          
    }   
}   
//-----Ende LatchDZ() ----------------------------------------------------------


/*
void DiagnoseTest(void)
{   static uint8_t cnt = 0;
    
    CanTX[5] = cnt;
    if(cnt == 64)
    {
        CanTX[6] = (ParamCRC16 >> 8);
        CanTX[7] = ParamCRC16;
    }
    else
    {   CanTX[6] = PS.Bytes[cnt*2];
        CanTX[7] = PS.Bytes[(cnt*2)+1];     
    }   
    cnt ++;  
    if(cnt > 64) cnt = 0;
}
*/

//------------------------------------------------------------------------------
//Diagnose Index 0-9 ist mit den IPK / Latch Frames belegt.
//Index 10 ist das Typenschild. Index 11 beinhaltet nur 0xAA für den Slave.
//Es gibt 3 IPK Frames:
//1. Der Standard Frame (cntFrame = 0) mit den aktuellen Laufzeitdaten, eine Kopie des IPK Frames.
//2. Der RX Latch-Frame (cntFrame = 1), der die RX Latchdaten enthält, eine Kopie des gelatchten Master IPK Frames.
//3. Der TX Latch-Frame (cntFrame = 2), der die TX Latchdaten enthält, eine Kopie des gelatchten Slave IPK Frames.
//Wenn keine Latch-Frames vorhanden sind, dann wird dafür der Standard Frame gesendet.
//Die Frames werden nacheinander komplett gesendet. Es dauert 3 x 12 x 0,3s = 10,8s bis alles übertragen ist.
//------------------------------------------------------------------------------
void Diagnose(void)
{   
    static uint8_t cntTypenschild = 0;
    static uint8_t DiagBuffer[ProtokollUart], cntFrame = 0;
    uint8_t i;
    
    CanTX[5] ++;
    if(CanTX[5] > 11) CanTX[5] = 0;
    
    switch(CanTX[5])
    {   case 0:
        if(cntFrame == 0)       //Standard Frame
        {   cntFrame = 1;
            for(i = 0; i < ProtokollUart; i++)
            {   DiagBuffer[i] = TX.Bytes[i];
            }
        }
        else if(cntFrame == 1)  //LatchRX Frame
        {   cntFrame = 2;
            if(LatchRX.bf.Frametyp != 0)
            {   for(i = 0; i < ProtokollUart; i++)
                {   DiagBuffer[i] = LatchRX.Bytes[i];
                }
            }                   
            else            //oder wenn LatchRX nicht vorhanden Standard Frame
            {   for(i = 0; i < ProtokollUart; i++)
                {   DiagBuffer[i] = TX.Bytes[i];
                }
            }
        }
        else if(cntFrame == 2)
        {   cntFrame = 0;
            if(LatchTX.bf.Frametyp != 0)
            {   for(i = 0; i < ProtokollUart; i++)
                {   DiagBuffer[i] = LatchTX.Bytes[i];
                }
            }                   
            else            //oder wenn LatchTX nicht vorhanden Standard Frame
            {   for(i = 0; i < ProtokollUart; i++)
                {   DiagBuffer[i] = TX.Bytes[i];
                }
            }   
        }
        CanTX[6] = DiagBuffer[0];
        CanTX[7] = DiagBuffer[1];
        break;
        case 1:
        CanTX[6] = DiagBuffer[2];
        CanTX[7] = DiagBuffer[3];
        break;
        case 2:
        CanTX[6] = DiagBuffer[4];
        CanTX[7] = DiagBuffer[5];
        break;
        case 3:
        CanTX[6] = DiagBuffer[6];
        CanTX[7] = DiagBuffer[7];
        break;
        case 4:
        CanTX[6] = DiagBuffer[8];
        CanTX[7] = DiagBuffer[9];
        break;
        case 5:
        CanTX[6] = DiagBuffer[10];
        CanTX[7] = DiagBuffer[11];
        break;
        case 6:
        CanTX[6] = DiagBuffer[12];
        CanTX[7] = DiagBuffer[13];
        break;
        case 7:
        CanTX[6] = DiagBuffer[14];
        CanTX[7] = DiagBuffer[15];
        break;
        case 8:
        CanTX[6] = DiagBuffer[16];
        CanTX[7] = DiagBuffer[17];
        break;
        case 9:
        CanTX[6] = DiagBuffer[18];
        CanTX[7] = DiagBuffer[19];
        break;
        case 10:
        CanTX[6] = cntTypenschild;
        CanTX[7] = Typenschild[cntTypenschild];
        cntTypenschild ++;
        if(cntTypenschild > 15) cntTypenschild = 0;
        break;
        case 11:
        CanTX[6] = 0xAA;
        CanTX[7] = 0x00;
        //Temperatur
        //CanTX[7] = (ADCwert4 >>2);
        break;
        default:
        CanTX[6] = 0;
        CanTX[7] = 0;    
        break;    
    }
}
