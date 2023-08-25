#include <xc.h>
#include "p33CK256MP505.h"
#include "GetErrors.h"
#include "Frequenz.h"
#include "Can.h"
#include "RamTest.h"
#include "RomTest.h"
#include "ADC.h"
#include "Auswertung.h"
#include <stdint.h>

uint8_t flFrqErr = 0;
uint16_t cntFrqErr = 0;
uint8_t cntInUngleichErr = 0, InUngleichErr = 0;
uint8_t cntFW_Err = 0, flFW_Err = 0;
uint16_t cntQuittErr = 0;
uint8_t flQuittErr = 0;
uint16_t cntEmk1KanalErr = 0;
uint8_t flEmk1KanalErr = 0;
uint8_t cntErrParCRCVgl = 0, flErrParCRCVgl = 0;
uint8_t flSoftErr = 0;
uint8_t flCanTimeOutErr = 0;
uint8_t Parameter = 0;
volatile uint8_t cntAdcIrTO = 0;
uint8_t flAdcIrTO = 0;
uint8_t cntFrameVglErr = 0;

//------------------------------------------------------------------------------
//GetErrors() wird alle 10ms aufgerufen.    
//Die Errorflags werden in Auswertung() angewendet.
//Bei schweren Fehlern wird schon hier das CAN Errflag gesetzt.
//------------------------------------------------------------------------------
void GetErrors(void)
{   static uint8_t stmQuittErrors = 0;  
    uint8_t i = 0;
    cnt10msRoutinen ++;
    
    if((TX.bf.ParamKompl==1)&&(RX.bf.ParamKompl==1)) flParamKomplett = 1;
    else                                             flParamKomplett = 0;
    
    //Errors die unabhängig von flParamKomplett gesetzt werden
    if(flErr3V3Test == 1)
    {                               CanTX[0] |= 0x80;
                                    TX.bf.Err3V3 = 1;
    }
    if(flErr1V65Test == 1)
    {                               CanTX[0] |= 0x80;
                                    TX.bf.Err1V65 = 1;
    }
    if(flErr12VTest == 1)
    {                               CanTX[0] |= 0x80;
                                    TX.bf.Err12V = 1;
    }
    if(flCanTimeOutErr == 1)
    {                               CanTX[0] |= 0x80;
                                    TX.bf.ErrCanTO = 1;
    }
    if(flSerTOErr == 1)
    {                               CanTX[0] |= 0x80;
                                    TX.bf.ErrIPK = 1;
    } 
    
    if(flParamKomplett==1)
    {   getQuittierError();
        getEmkErr();
        getFrequenzErr();
        getInUngleichErr();
        if(fl100ms == 1) 
        {   getCanTimeOutErr(); 
            FW_Vergleich();
            IpkFrameVergleich();
            if(getSoftErr() != 0) //bei != ein 2. mal aufrufen
            {   if(getSoftErr() != 0) flSoftErr = 1;                
            }
        }
        cntAdcIrTO ++;
        if(cntAdcIrTO > 100)
        {   //ADC ISR Timeout: CAN Error setzen
            flAdcIrTO = 1;
            CanTX[0] |= 0x80;
        }
        //----------------------------------------------------------------------
        if(flErrTriggerADC == 1)        
        {                               TX.bf.ErrTimeOutADC = 1;
                                        CanTX[0] |= 0x80;
        }
        //----------------------------------------------------------------------                                      
        if(flErrParCanCRC == 1)
        {                               TX.bf.ErrParCanCRC = 1;      //CAN Parameter CRC16 check
                                        CanTX[0] |= 0x80;
        }
        else 
        {                               TX.bf.ErrParCanCRC = 0;
                                        ; //CanTX[0] &= 0x7F;         //nur bei ResetErrors() löschen
        }
        if(flErrParCRCVgl == 1)         
        {                               TX.bf.ErrParCRCVgl = 1;      //CRC16 Vergleich IPK
                                        CanTX[0] |= 0x80;
        }
        else
        {                               TX.bf.ErrParCRCVgl = 0;
                                        ; //CanTX[0] &= 0x7F;         //nur bei ResetErrors() löschen
        }
        
        if(flFW_Err == 1)               TX.bf.ErrFW = 1;
        else                            ;   //TX.bf.ErrFW = 0;
        //----------------------------------------------------------------------
        //if(flDrBrSpannung == 1)         TX.bf.ErrDrahtbrPuls = 1; //###test###aktivieren mit neuer HW
        //else                            TX.bf.ErrDrahtbrPuls = 0; //###test###aktivieren mit neuer HW
                                      
        if(TX.bf.ErrParPlaus == 1)
        {                       //CAN Param. sind nicht plausibel wird in CAN writeParamStruct() gesetzt
                                        CanTX[0] |= 0x80;
        }
        else
        {                               //CanTX[0] &= 0x7F;  //###test###nur bei ResetErrors() löschen    
        }
        
        if(flSoftErr == 1)              TX.bf.SoftErr = 1;
        else                            TX.bf.SoftErr = 0;
        
        if(flQuittErr == 1)             TX.bf.ErrQuitt = 1;
        else                            TX.bf.ErrQuitt = 0;
        
        if(flPortsErr == 1)
        {   //Alle Eingänge auf IPK nullen, siehe auch selectInput()
                                        TX.Bytes[1] &= 0xF0;
                                        TX.bf.ErrPortTest = 1;
        }
        else                            TX.bf.ErrPortTest = 0;
                                      
        //TX.bf.ErrEingTest1          //ErrEing.Isolator.Test1 nur 2. Kanal        
        
        if(InUngleichErr != 0)          TX.bf.ErrEingUngleich = 1;
        else                            TX.bf.ErrEingUngleich = 0; 
        
        //----------------------------------------------------------------------
        if(flDrahtBruchErr == 1)        TX.bf.ErrDrahtBr = 1;
        else                            TX.bf.ErrDrahtBr = 0;
        
//###test###if(flDiffErr == 1)              TX.bf.ErrExtendedDrBr = 1;
//###test###else                            TX.bf.ErrExtendedDrBr = 0;
        
        if(flErrGleichePhase == 1)      TX.bf.ErrPhasenlage = 1;
        else                            TX.bf.ErrPhasenlage = 0;
        
        if(flErrRichtungsWechsel == 1)  TX.bf.ErrRichtung = 1;
        else                            TX.bf.ErrRichtung = 0;
        
        if(fl1500Hz == 1)               TX.bf.Err1500Hz = 1;
        else                            TX.bf.Err1500Hz = 0;
        
        if(flFrqErr == 1)               TX.bf.ErrFrqVgl = 1;
        else                            TX.bf.ErrFrqVgl = 0;
        
        if(flAdcFrqErr == 1)            TX.bf.ErrAnalogDigital = 1;
        else                            TX.bf.ErrAnalogDigital = 0;
        
        if(flEmk1KanalErr == 1)         TX.bf.ErrEMK1Kan = 1;
        else                            TX.bf.ErrEMK1Kan = 0;
        
        if(_flSkrErr1 == 1)             TX.bf.ErrSKR1 = 1;
        else                            TX.bf.ErrSKR1 = 0;
        if(_flSkrErr2 == 1)             TX.bf.ErrSKR2 = 1;
        else                            TX.bf.ErrSKR2 = 0;
        if(_flSkrQSErr1 == 1)           TX.bf.ErrQuersSKR1 = 1;
        else                            TX.bf.ErrQuersSKR1 = 0;
        if(_flSkrQSErr2 == 1)           TX.bf.ErrQuersSKR2 = 1;
        else                            TX.bf.ErrQuersSKR2 = 0;
        if((SkrErrGrundst & 0x01)!=0)   TX.bf.ErrGrundstSKR1 = 1;
        else                            TX.bf.ErrGrundstSKR1 = 0;
        if((SkrErrGrundst & 0x02)!=0)   TX.bf.ErrGrundstSKR2 = 1;
        else                            TX.bf.ErrGrundstSKR2 = 0; 
    }
    else //flParamKomplett==0
    {   ResetErrors(ohneKritischeErr);       
    }
    
    switch(stmQuittErrors)
    {   case 0:
        //warten auf steigende Flanke RTSK = QuittErrors Flag
        if((CanRX[4] & 0x80) != 0)  stmQuittErrors = 1;                    
        break;
        case 1:
        //warten auf fallende Flanke
        if((CanRX[4] & 0x80) == 0)
        {   stmQuittErrors = 2;
            ResetErrors(mitKritischeErr); 
            stmLatchDZ1 = 0;
            flLatchTX = 0;
            //flLatchRX = 0;
            for(i=0; i < ProtokollUart; i++)
            {   LatchTX.Bytes[i] = 0;
                LatchRX.Bytes[i] = 0;
            }
        }
        break;   
        case 2:
        stmQuittErrors = 0;                   
        break;
    }
}//-----Ende GetErrors()--------------------------------------------------------


//------------------------------------------------------------------------------
//ResetErrors() wird aufgerufen, wenn Parameter nicht komplett sind
//und wenn die Errors mit dem CAN RTSK-bit quittiert werden.
//Bit 0             1               2               3               4               5               6               7
//SKR2-Err      SKR1-Err
//ErrFW-Vers	LimitParamErr	ParamChksumErr	Par.Kopie.Err	PortTestErr     IPK-Err         CAN-TO-Err      3V3-TestErr
//12V-Err       1,65V-Err       Eing.Unglei     ErrAdcFrq       Frq.VerglErr	Frq>1500Hz      ErrDrahtbruch	QuittErr
//frei          frei            frei            frei            Dr.Br.PulsErr	Eing.TestErr	ChsVergleichErr	EMK-1Kan.Err
//------------------------------------------------------------------------------
void ResetErrors(uint8_t KritischeErr)
{   //uint8_t i = 0;
    if(KritischeErr == mitKritischeErr)
    {   CanTX[0] &= 0x7F;       //CAN Master Errorflag  
        cntSerTOErr = 0;
        flSerTOErr = 0;
        cntCanTimeOut = kCanTimeOut;
        flCanTimeOutErr = 0;     
        cntErr3V3Test = 0;
        flErr3V3Test = 0;
        cntErr1V65Test = 0;
        flErr1V65Test = 0;
        cntErr12VTest = 0;
        flErr12VTest = 0;
        TX.Bytes[11] &= 0xE0;   //Kritische Error löschen
        RX.Bytes[11] &= 0xE0;
        cntErrTriggerADC = 0; 
        flErrTriggerADC = 0;
    }
    cntPortTestErr = 0;
    flPortsErr = 0;    
    cntFrqErr = 0;
    flFrqErr = 0;
    cntFW_Err = 0;
    flFW_Err = 0;
    cntQuittErr = 0;
    flQuittErr = 0;
    flSoftErr = 0;
    cntErrParCRCVgl= 0;
    flErrParCRCVgl= 0;
    cnt1500Hz = 0;
    fl1500Hz = 0;
    cntErrRichtungsWechsel = 0;
    flErrRichtungsWechsel = 0;
    cntErrGleichePhase = 0;
    flErrGleichePhase = 0;
    cntAdcFrqErr = 0;
    flAdcFrqErr = 0;
    cntDiffErr = 0;
    flDiffErr = 0;
    cntDrahtBruchErr = 0;
    flDrahtBruchErr = 0;
    cntDrBrSpLowErr = 0;
    cntDrBrSpHighErr = 0;
    flDrBrSpannung = 0;
    cntEmk1KanalErr = 0;
    flEmk1KanalErr = 0;
    stmLatchDZ1 = 0;
    _flSkrErr1 = 0;
    _flSkrErr2 = 0;
    _flSkrQSErr1 = 0;
    _flSkrQSErr2 = 0;
    SkrErrGrundst = 0;  
    cntInUngleichErr = 0; 
    InUngleichErr = 0; 
    cntFrameVglErr = 0;
    TX.Bytes[11] &= 0x1F;   //Nicht Kritische Error löschen
    RX.Bytes[11] &= 0x1F;
    TX.Bytes[12] = 0;
    RX.Bytes[12] = 0;
    TX.Bytes[13] = 0;
    RX.Bytes[13] = 0;
    TX.Bytes[14] = 0;   
    RX.Bytes[14] = 0;
}//------Ende ResetErrors() ----------------------------------------------------


//------------------------------------------------------------------------------
//getInUngleichErr() wird alle 10ms aufgerufen.
//------------------------------------------------------------------------------
void getInUngleichErr(void)
{   //Nach 2,5Sek Ungleichheit der Eingänge werden die entsprechenden 
    //Ungleichbits gesetzt, sie erscheinen in der Diagnose.
    //Kein Slokoff, mit ResetErrors() wieder gelöscht.
    //if(SerTX[1] != SerRX[1])
    if((TX.Bytes[1] & 0x0F)!=(RX.Bytes[1] & 0x0F))
    {   cntInUngleichErr ++;
        if(cntInUngleichErr == 0xFF) InUngleichErr = ((TX.Bytes[1] & 0x0F)^(RX.Bytes[1] & 0x0F));       
    }
    else
    {   cntInUngleichErr = 0;       
    }    
}//------Ende getInUngleichErr()------------------------------------------------


//------------------------------------------------------------------------------
//getFrequenzErr() wird alle 10ms aufgerufen.
//Die IstFreqX10 von Master und Slave werden miteinander ins Verhältnis gesetzt.
//Der Frequenzvergleich ist nicht aktiv bei:
//Him==1, Muting, FrqX10 < 20 (2Hz), < Stillstandfrq
//Fehlersituation bei Frequenz ratio von > 1,3, entsprechen > 30%.
//Die Messwerte liegen als 10-fache Frequenz vor.
//Der kleinste FreqX10 sollte 2 sein, entsprechend 0,2Hz nach Mess-TO
//Formel:
//Größerer Wert x 100, geteilt durch kleinerer Wert.
//Fehler bei Ergebnis >= 130, ratio > 1,3
//------------------------------------------------------------------------------
void getFrequenzErr(void)
{   uint16_t RxFreq = 0, TxFreq = 0;
    uint32_t ratio = 0;
    
    TxFreq = IstFrqX10;
    if(TxFreq == 0) TxFreq = 1;
    RxFreq = (((uint16_t)RX.Bytes[6]) <<8) + RX.Bytes[7];
    if(RxFreq == 0) RxFreq = 1;

    if((Muting == 1)
    ||((TX.bf.Limit0 ==1)&&(RX.bf.Limit0 == 1)) //Stillstand == 1
    ||((TxFreq < PS.Par.FrqVglSchw)&&(RxFreq < PS.Par.FrqVglSchw)))        //Parametrierte Frequenzschwelle
    {   //Kein Freqenz Vergleich
        cntFrqErr = 0;
    }
    else //Frequenz vergleichen
    {   if(TxFreq > RxFreq)
        {   ratio = (100 * (uint32_t)TxFreq) / RxFreq;           
        }
        else
        {   ratio = (100 * (uint32_t)RxFreq) / TxFreq;           
        }
        
        if(ratio > 130)
        {   cntFrqErr ++;
            if(cntFrqErr > 400) flFrqErr = 1;   //400 = 4s
        }
        else
        {   if(cntFrqErr > 0) cntFrqErr --;           
        }  
    }  
}//------Ende getFrequenzErr() -------------------------------------------------


//------------------------------------------------------------------------------
//getPositionErr() wird alle 10ms aufgerufen.
//------------------------------------------------------------------------------
void getPositionErr(void)
{   ;
}//------Ende getPositionErr() -------------------------------------------------


//------------------------------------------------------------------------------
//getCanTimeOutErr() wird alle 100ms aufgerufen.
//In getCanBytes() wird cntCanTimeOut mit Konstante aufgezogen, entspr. 3Sek.
//------------------------------------------------------------------------------
void getCanTimeOutErr(void)
{   
    if(cntCanTimeOut > 0) cntCanTimeOut --;
    else flCanTimeOutErr = 1;   
}//------getCanTimeOutErr()-----------------------------------------------------


//------------------------------------------------------------------------------
//FW_Vergleich() wird alle 100ms aufgerufen
//------------------------------------------------------------------------------
void FW_Vergleich(void)
{   
    if(TX.Bytes[IPK_FW] != RX.Bytes[IPK_FW])
    {   cntFW_Err ++;
        if(cntFW_Err > 25) flFW_Err = 1; //25 = 2,5s
    }
    else
    {   flFW_Err = 0; 
        cntFW_Err = 0;
    }
} //------Ende FW_Vergleich()---------------------------------------------------


//------------------------------------------------------------------------------
//IpkFrameVergleich() wird alle 100ms aufgerufen
//Prüft auf FrameEnde von Master und Slave.
//Bei Kurzschluss TX/RX ist das FrameEnde gleich.
//------------------------------------------------------------------------------
void IpkFrameVergleich(void)
{   if(TX.Bytes[19] == RX.Bytes[19])
    {   cntFrameVglErr ++;
        if(cntFrameVglErr > 25) flSerTOErr = 1;
    }
    else
    {   cntFrameVglErr = 0;
    }
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//getSoftErr() wird alle 100ms aufgerufen
//Die Parameter in PS werden mit der Copy verglichen
//Die ParChs wird bei der Gelegenheit neu ermittelt und auf die IPK geschrieben
//------------------------------------------------------------------------------
uint8_t getSoftErr(void)
{   uint16_t i; 
    uint8_t softerr = 0;
    
    for(i = 0; i< SizePS; i++)
    {   if(Param_copy.Bytes[i] != PS.Bytes[i]) softerr = 1;
    }
    for(i = 0; i< 256 ; i++)
    {   if(DncoCopy[i] != DNCO[i]) softerr = 1; 
    }

    //TX.Bytes[17] = checksum;
    if((TX.Bytes[16]!=RX.Bytes[16])||(TX.Bytes[17]!=RX.Bytes[17])) cntErrParCRCVgl ++;
    else cntErrParCRCVgl = 0;
    if(cntErrParCRCVgl > 25) flErrParCRCVgl = 1;
    
    return softerr;
} //----------------------------------------------------------------------------


//------------------------------------------------------------------------------
//getEmkErr() wird alle 10ms aufgerufen
//Die EMK Messwerte werden in TX.Bytes[8] als 8Bit Werte übertragen
//Wenn die Werte extrem unterschiedlich sind, wird ein Fehler erkannt.
//------------------------------------------------------------------------------
void getEmkErr(void)
{   
    if(PS.Par.EmkMode != 0)
    {   if(IstFrqX10 > 100) //Frequenz > 10Hz
        {   if(((TX.Bytes[8] > k8Bit3V0)&&(RX.Bytes[8] < k8Bit0V1))
            || ((TX.Bytes[8] < k8Bit0V1)&&(RX.Bytes[8] > k8Bit3V0)))
            {   cntEmk1KanalErr ++;
                if(cntEmk1KanalErr > 300) flEmk1KanalErr = 1;
            }
            else
            {   if(cntEmk1KanalErr > 0) cntEmk1KanalErr --;       
            } 
        }
        else //Frequenz ist <= 10Hz
        {   cntEmk1KanalErr = 0;
        }
    }
    else //EmkMode == 0
    {   cntEmk1KanalErr = 0;
        flEmk1KanalErr = 0;
    }
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//QuittierError() wird alle 10ms aufgerufen
//------------------------------------------------------------------------------
void getQuittierError(void)
{   //Quittiersignal auf CAN > 10s high? Nur bei ManualQuitt
    if((PS.Par.DrehzahlMode == 1)
    || (PS.Par.DrehzahlMode == 3)
    || (PS.Par.DrehzahlMode == 4))
    {
        if((CanRX[4] & 0x40) != 0) 
        {   if(cntQuittErr < 1000) cntQuittErr++;
        }
        else
        {   if(cntQuittErr >= 10)    cntQuittErr -= 10;
            else if(cntQuittErr > 0) cntQuittErr --;
        }
        if(cntQuittErr == 1000)
        {   flQuittErr = 1;
            QuittReset();
        }
        else if(cntQuittErr == 0)
        {   flQuittErr = 0;     
        }
    }
    else
    {   cntQuittErr = 0;
        flQuittErr = 0; 
    }
} //----------------------------------------------------------------------------
