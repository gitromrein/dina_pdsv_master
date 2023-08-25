#include <xc.h>
#include "p33CK256MP505.h"
#include "Frequenz.h"
#include "FrequenzVergleich.h"
#include <stdint.h>

uint8_t Muting = 0;

//------------------------------------------------------------------------------
//FrequenzVergleich() wird jede 1ms aufgerufen
//------------------------------------------------------------------------------
void FrequenzVergleich()
{       
    cnt1msRoutinen ++;
    
    if(PS.Par.DrehzahlMode == 0)
    {   keinFrqVergl();   //deaktiviert
    }
    else if(PS.Par.DrehzahlMode < 3)
    {   DzFrqVergl();
    }
    else if(PS.Par.DrehzahlMode < 7)
    {   FensterFrqVergl();
    }
    else if(PS.Par.DrehzahlMode == 7)
    {   BereichFrqVergl();
    }   
} //Ende FrequenzVergleich()----------------------------------------------------


//------------------------------------------------------------------------------
//Frequenzvergleich ist deaktiviert, PS.Par.DrehzahlMode == 0
//------------------------------------------------------------------------------
void keinFrqVergl(void)
{   StatusLimit0 = 0;
    StatusLimit1 = 0;
    StatusLimit2 = 0;
    StatusLimit3 = 0;
    TX.Bytes[4] &= 0xF0;
} //Ende keinFreqVergl()--------------------------------------------------------


//------------------------------------------------------------------------------
//Modus1FrqVergl() wird alle 1ms aufgerufen if(PS.Par.DrehzahlMode < 3)
//------------------------------------------------------------------------------
void DzFrqVergl(void)
{   static uint16_t VglLimit0 = 0, VglLimit1 = 0, DelayVglLimit1 = 0; 
    static uint16_t timeout0 = 0, timeout1 = 0; 
    static uint16_t cntDelayDz1off = 0, cntDelaySSToff = 0;
    static uint8_t BA = 0, DncoIX = 0;
    //eventuell globale Variable Betriebarten nicht verwenden
    
    if(fl10ms == 1) //----------------------------------------------------------
    {   BA = 0;
        Muting = 0;
        Betriebsarten = (TX.Bytes[2] & 0x0F); //###test###wieder AND verknüpfen(TX.Bytes[2] & RX.Bytes[2] & 0x0F);
        if(Betriebsarten == 0) BA = 0;
        else if((Betriebsarten & 0x08) != 0) Muting = 1;
        else if((Betriebsarten & 0x04) != 0) BA = 3;
        else if((Betriebsarten & 0x02) != 0) BA = 2;
        else if((Betriebsarten & 0x01) != 0) BA = 1;
        
        //Die Validierstufen (0,1,2,3) begrenzt die Validierung
        Validierung = (((TX.Bytes[2] & RX.Bytes[2] & 0x30) >> 4) & PS.Par.Validierstufen);
        DncoIX = TX.Bytes[14];
        
        if((PS.Par.DNCO & 0x01) == 0)
        {   //keine DNCO Funktion
            //Stillstand
            FrqLimit0 = PS.Par.arrFrqLimits[0][0];
            FrqLimHyst0 = PS.Par.arrFrqHyst[0][0];
            timeout0 = PS.Par.arrFrqTimeout[0][0];
            //Drehzahl
            if(BA == 0)
            {   //bei BA=0 gibt es keine Validierfunktion
                FrqLimit1 = FrqLimit0;
                FrqLimHyst1 = FrqLimHyst0;
                timeout1 = timeout0;
            }    
            else if(BA <= 3)    
            {   FrqLimit1 = PS.Par.arrFrqLimits[BA][Validierung];
                FrqLimHyst1 = PS.Par.arrFrqHyst[BA][Validierung];
                timeout1 = PS.Par.arrFrqTimeout[BA][Validierung];
            }
        }
        else if((PS.Par.DNCO & 0x10) != 0)
        {   //Bit0 ==1, Bit4 ==1: DNCO Funktion für BA == 3
            //Stillstand
            FrqLimit0 = PS.Par.arrFrqLimits[0][0];
            FrqLimHyst0 = PS.Par.arrFrqHyst[0][0];
            timeout0 = PS.Par.arrFrqTimeout[0][0];
            //Drehzahl
            if(BA == 0)
            {   //bei BA=0 gibt es keine Validierfunktion
                FrqLimit1 = FrqLimit0;
                FrqLimHyst1 = FrqLimHyst0;
                timeout1 = timeout0;
            }
            else if(BA <= 2)
            {   FrqLimit1 = PS.Par.arrFrqLimits[BA][Validierung];
                FrqLimHyst1 = PS.Par.arrFrqHyst[BA][Validierung];
                timeout1 = PS.Par.arrFrqTimeout[BA][Validierung];               
            }
            else if(BA == 3)
            {   //DNCO Funktion
                FrqLimit1 = (((uint16_t)DNCO[DncoIX] << 8) + DNCO[DncoIX+1]);
                if(Validierung == 1)      FrqLimit1 = ((((uint32_t)FrqLimit1) * 115) >> 7); //x 0,90
                else if(Validierung == 2) FrqLimit1 = ((((uint32_t)FrqLimit1) * 109) >> 7); //x 0,85
                else if(Validierung == 3) FrqLimit1 = ((((uint32_t)FrqLimit1) * 102) >> 7); //x 0,80
                FrqLimHyst1 = ((((uint32_t)FrqLimit1) * 115) >> 7);                         //x 0,90
                if(FrqLimit1 == 0)
                {   timeout1 = 500;             //500 = 5000ms                 
                }
                else if(FrqLimit1 < 130)        //130 = 10-fache Frequenz von 13Hz
                {   timeout1 = 1300/FrqLimit1;  //entspricht Periodenzeit x 1,3. Ergebnis x 10ms (Raster des Decrementierens)                 
                }
                else timeout1 = 10;             //10 = 100ms              
            }            
        }
        else if((PS.Par.DNCO & 0x10) == 0)
        {   //Bit0 ==1, Bit4 ==0: DNCO Funktion für BA == 0,1,2,3
            //Stillstand
            FrqLimit0 = (((uint16_t)DNCO[DncoIX +96] << 8) + DNCO[DncoIX +97]);
            FrqLimHyst0 = ((((uint32_t)FrqLimit1) * 115) >> 7); //x 0,90
            if(FrqLimit0 == 0)
            {   timeout0 = 500;             //500 = 5000ms                 
            }
            else if(FrqLimit0 < 130)        //130 = 10-fache Frequenz von 13Hz
            {   timeout0 = 1300/FrqLimit0;  //entspricht Periodenzeit x 1,3. Ergebnis x 10ms (Raster des Decrementierens)                
            }
            else timeout0 = 10;             //10 = 100ms 
            //Drehzahl
            if(BA == 0)
            {   FrqLimit1 = FrqLimit0;
                FrqLimHyst1 = FrqLimHyst0;
                timeout1 = timeout0;
            }
            else
            {   if(BA == 3)
                {   FrqLimit1 = (((uint16_t)DNCO[DncoIX] << 8) + DNCO[DncoIX +1]);
                }
                else if(BA == 2)
                {   FrqLimit1 = (((uint16_t)DNCO[DncoIX + 32] << 8) + DNCO[DncoIX + 33]);                  
                }
                else if(BA == 1)
                {   FrqLimit1 = (((uint16_t)DNCO[DncoIX + 64] << 8) + DNCO[DncoIX + 65]);                 
                }
                if(Validierung == 1)      FrqLimit1 = ((((uint32_t)FrqLimit1) * 115) >> 7); //x 0,90
                else if(Validierung == 2) FrqLimit1 = ((((uint32_t)FrqLimit1) * 109) >> 7); //x 0,85
                else if(Validierung == 3) FrqLimit1 = ((((uint32_t)FrqLimit1) * 102) >> 7); //x 0,80
                FrqLimHyst1 = ((((uint32_t)FrqLimit1) * 115) >> 7);                         //x 0,90
                if(FrqLimit1 == 0)
                {   timeout1 = 500;             //500 = 5000ms                 
                }
                else if(FrqLimit1 < 130)        //130 = 13Hz = 10-fache Frequenz von 13Hz
                {   timeout1 = 1300/FrqLimit1;  //entspricht Periodenzeit x 1,3. Ergebnis x 10ms (Raster des Decrementierens)                 
                }
                else timeout1 = 10;             //10 = 100ms          
            }
        }
        if(Muting==1)
        {   FrqLimit1 = 0xFFFF;
            FrqLimHyst1 = 0xFFFF;
            timeout1 = 10;           //######test##########notwendig??? 
        }
    } //Ende 10ms Abfrage. Die Werte für FrqLimit0/1, FrqLimHyst0/1 und timeout0/1 liegen vor.
      
    //1ms Raster, die Werte aus dem 10ms Raster können hier verwendet werden
    //Bei LimitBit=1 wird der FrqLimit Wert übernommen, andernfalls der Hysteresewert.
    //Mit dem Vergleich IstFreqX10 und VglLimit wird StatusLimit gesetzt, was in Auswertung() weiterverwendet wird.
    if(TX.bf.Limit0 == 1) VglLimit0 = FrqLimit0;
    else                  VglLimit0 = FrqLimHyst0;
    if(TX.bf.Limit1 == 1) VglLimit1 = FrqLimit1;
    else                  VglLimit1 = FrqLimHyst1;
     
    //1ms Raster
    //Vergleich von Ist und Soll Frequenz, Status setzen
    //Bei NeueMessung==1: Bei IstDrehzahl > Soll in Status eine 0 einschieben, andernfalls eine 1
    //Wenn keine neue Messung vorliegt, wird die StatusLimit-Register nicht geändert, 
    //außer bei Timeouts.
    
    if(cntDelaySSToff > 0) cntDelaySSToff --;
    if(cntDelayDz1off > 0) cntDelayDz1off --;   //Abschaltverzögerung, wird aufgezogen bei IstFrq < SollFrq, und Timeout
    //###test### Neu: Abschaltverzögerung ++
    if(cntDelayDz1off == 0)
    {   DelayVglLimit1 = VglLimit1;        
    }
    else //Verzögerungszeit läuft noch ab
    {   if(PS.Par.ProzentAbschVerz == 0)
        {   //Während der Verzögerungszeit wird praktisch gemutet
            DelayVglLimit1 = 0xFFFF;
        }
        else
        {   DelayVglLimit1 = ((((uint32_t)VglLimit1) * (100 + PS.Par.ProzentAbschVerz)) / 100);
        }       
    }
    
    if(NeueMessung == 1)
    {   NeueMessung = 0;    //wird in getFrequenz() bei neuer Signalflanke gesetzt.
        //Stillstand 
        if(IstFrqX10 >= VglLimit0)
        {   //führt zum Abschalten
            if(cntDelaySSToff == 0)
            {   if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit0 = (StatusLimit0 << 1); } //eine Null von rechts einschieben
                else              { StatusLimit0 = (StatusLimit0 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben           
            }
        }
        else
        {   //führt zum Einschalten
            if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit0 = ((StatusLimit0 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit0 = ((StatusLimit0 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
            if((PS.Par.ControlBits & 0x04) != 0)   //Verzögerter Stillstand
            {   cntDelaySSToff = (((uint16_t)PS.Par.Abschaltverz) *10);
            }
        }
        //Drehzahl 1
/*        if(IstFrqX10 >= VglLimit1)
        {   //führt zum Abschalten
            if(cntDelayDz1off == 0)
            {   if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit1 = (StatusLimit1 << 1); } //eine Null von rechts einschieben
                else              { StatusLimit1 = (StatusLimit1 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben 
            }  
        }
        else
        {   //führt zum Einschalten    
            if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit1 = ((StatusLimit1 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit1 = ((StatusLimit1 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
            cntDelayDz1off = (((uint16_t)PS.Par.Abschaltverz) *10);
        }
*/
        if(IstFrqX10 >= DelayVglLimit1) //###test### Neu: Abschaltverzögerung ++ ###############
        {   //führt zum Abschalten
            if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit1 = (StatusLimit1 << 1); } //eine Null von rechts einschieben
            else              { StatusLimit1 = (StatusLimit1 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben 
        }
        else
        {   //führt zum Einschalten    
            if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit1 = ((StatusLimit1 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit1 = ((StatusLimit1 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
            //cntDelayDz1off = (((uint16_t)PS.Par.Abschaltverz) *10);
        }
        if(IstFrqX10 < VglLimit1) cntDelayDz1off = (((uint16_t)PS.Par.Abschaltverz) *10);
    }

    if(cntTimeOut0 > timeout0)
    {   //Timeout "abgelaufen"
        StatusLimit0 = 0xFF;
        if((PS.Par.ControlBits & 0x04) != 0)    //Verzögerter Stillstand
        {   cntDelaySSToff = (((uint16_t)PS.Par.Abschaltverz) *10);
        }
    }
    if((cntTimeOut1 > timeout1)||(Muting==1))
    {   //Timeout "abgelaufen"
        StatusLimit1 = 0xFF;
        cntDelayDz1off = (((uint16_t)PS.Par.Abschaltverz) *10);
    }
    
    //Limit1 und Limit2 Bits auf IPK setzten, Nullen führen zum Abschalten
    if((StatusLimit0 & ((1 << PS.Par.XfachMessung) -1)) == 0) TX.bf.Limit0 = 0;
	else                                                      TX.bf.Limit0 = 1;
	if((StatusLimit1 & ((1 << PS.Par.XfachMessung) -1)) == 0) TX.bf.Limit1 = 0;
	else					       					          TX.bf.Limit1 = 1;
    
} //Ende DzFrqVergl()-----------------------------------------------------------


//------------------------------------------------------------------------------
//FensterÜW, wird alle 1ms aufgerufen, if(PS.Par.DrehzahlMode < 7)
//------------------------------------------------------------------------------
void FensterFrqVergl(void)
{   static uint16_t VglLimit0 = 0, VglLimit1 = 0, VglLimit2 = 0, DelayVglLimit2 = 0;
    static uint16_t timeout0 = 0, timeout1 = 0, timeout2 = 0;
    static uint16_t cntDelayFrqMinOff = 0, cntDelayFrqMaxOff = 0, cntDelaySSToff = 0;
    
    if(fl10ms == 1) //----------------------------------------------------------
    {   //Nur auf Muting abfragen, Betriebsarten 1,2,3 gibt es nicht
        //Betriebsarten = (TX.Bytes[2] & RX.Bytes[2] & 0x0F); //###test###wieder aktivieren
        Betriebsarten = (TX.Bytes[2] & 0x0F);
        if((Betriebsarten & 0x08) != 0) Muting = 1;
        else                            Muting = 0;
        
        //Die Validierstufen (0,1,2,3) begrenzt die Validierung
        Validierung = (((TX.Bytes[2] & RX.Bytes[2] & 0x30) >> 4) & PS.Par.Validierstufen);
        
        //Stillstand
        FrqLimit0 = PS.Par.arrFrqLimits[0][0];
        FrqLimHyst0 = PS.Par.arrFrqHyst[0][0];
        timeout0 = PS.Par.arrFrqTimeout[0][0];
        //Limit1, Frequenz Min      
        FrqLimit1 = PS.Par.arrFrqLimits[1][Validierung];
        FrqLimHyst1 = PS.Par.arrFrqHyst[1][Validierung];
        timeout1 = PS.Par.arrFrqTimeout[1][Validierung];
        //Limit2, Frequenz Max      
        FrqLimit2 = PS.Par.arrFrqLimits[2][Validierung];
        FrqLimHyst2 = PS.Par.arrFrqHyst[2][Validierung];
        timeout2 = PS.Par.arrFrqTimeout[2][Validierung];
        
        if(Muting == 1)
        {   FrqLimit1 = 0;
            FrqLimHyst1 = 0;
            timeout1 = 0;       //###test###
            FrqLimit2 = 0xFFFF;
            FrqLimHyst2 = 0xFFFF;
            timeout2 = 10;      //###test###
        }
     
    } //Ende 10ms Abfrage. Die Werte für FrqLimit0/1/2, FrqLimHyst0/1/2 und timeout0/1/2 liegen vor.
      
    //1ms Raster, die Werte aus dem 10ms Raster können hier verwendet werden
    //Bei LimitBit=1 wird der FrqLimit Wert übernommen, andernfalls der Hysteresewert.
    //Mit dem Vergleich IstFreqX10 und VglLimit wird StatusLimit gesetzt, was in Auswertung() weiterverwendet wird.
    //Stillstand
    if(TX.bf.Limit0 == 1) VglLimit0 = FrqLimit0;        //Stillstandfrq. ist nicht überschritten
    else                  VglLimit0 = FrqLimHyst0;
    //Fenster Frequenz Min: Umgekehrte Hysterese-Logik
    if(TX.bf.Limit1 == 0) VglLimit1 = FrqLimit1;        //Frequenz Min ist überschritten, > FrqMin + Hysterese
    else                  VglLimit1 = FrqLimHyst1;
    //Fenster Frequenz Max
    if(TX.bf.Limit2 == 1) VglLimit2 = FrqLimit2;        //Frequenz Max ist nicht überschritten
    else                  VglLimit2 = FrqLimHyst2;
     
    
    //1ms Raster
    //Vergleich von Ist und Soll Frequenz, Status setzen
    //Bei NeueMessung==1: Bei IstDrehzahl > Soll in Status eine 0 einschieben, andernfalls eine 1
    //Wenn keine neue Messung vorliegt, wird die StatusLimit-Register nicht geändert, 
    //außer bei Timeouts.
    
    if(cntDelaySSToff > 0) cntDelaySSToff --; 
    if(cntDelayFrqMinOff > 0) cntDelayFrqMinOff --;     //Abschaltverzögerung, wird aufgezogen bei > FreqMin.
    if(cntDelayFrqMaxOff > 0) cntDelayFrqMaxOff --;     //Abschaltverzögerung, wird aufgezogen bei < FreqMax und im Timeout
    //###test### Neu: Abschaltverzögerung ++
    if(cntDelayFrqMaxOff == 0)
    {   DelayVglLimit2 = VglLimit2;        
    }
    else //Verzögerungszeit läuft noch ab
    {   if(PS.Par.ProzentAbschVerz == 0)
        {   //Während der Verzögerungszeit wird praktisch gemutet
            DelayVglLimit2 = 0xFFFF;
        }
        else
        {   DelayVglLimit2 = ((((uint32_t)VglLimit2) * (100 + PS.Par.ProzentAbschVerz)) / 100);
        }       
    }
    
    
    if(NeueMessung == 1)
    {   NeueMessung = 0;    //wird in getFrequenz() bei neuer Signalflanke gesetzt.
        //Stillstand -----------------------------------------------------------
        if(IstFrqX10 >= VglLimit0)
        {   //führt zum Abschalten des Limit0 Bits
            if(cntDelaySSToff == 0)
            {   if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit0 = (StatusLimit0 << 1); } //eine Null von rechts einschieben
                else              { StatusLimit0 = (StatusLimit0 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben
            }
        }
        else
        {   //führt zum Einschalten des Limit0 Bits
            if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit0 = ((StatusLimit0 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit0 = ((StatusLimit0 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
            if((PS.Par.ControlBits & 0x04) != 0)   //Verzögerter Stillstand
            {   cntDelaySSToff = (((uint16_t)PS.Par.Abschaltverz) *10);
            }
        }
        //Frequenz Min ---------------------------------------------------------
        if(IstFrqX10 >= VglLimit1)
        {   //führt zum Abschalten des Limit1 Bits, ist aber im Fenster            
            if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit1 = (StatusLimit1 << 1); } //eine Null von rechts einschieben
            else              { StatusLimit1 = (StatusLimit1 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben 
            cntDelayFrqMinOff = (((uint16_t)PS.Par.Abschaltverz) *10);
        }
        else
        {   //führt zum Einschalten des Limit1 Bits, ist aber außerhalb des Fensters  
            if(cntDelayFrqMinOff == 0)
            {   if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit1 = ((StatusLimit1 << 1) +1); } //eine 1 von rechts einschieben
                else { StatusLimit1 = ((StatusLimit1 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
            }
        }
        //Frequenz Max ---------------------------------------------------------
/*        if(IstFrqX10 >= VglLimit2)
        {   //führt zum Abschalten des Limit2 Bits
            if(cntDelayFrqMaxOff == 0)
            {   if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit2 = (StatusLimit2 << 1); } //eine Null von rechts einschieben
                else              { StatusLimit2 = (StatusLimit2 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben 
            }
        }
        else
        {   //führt zum Einschalten des Limit2 Bits
            if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit2 = ((StatusLimit2 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit2 = ((StatusLimit2 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
            cntDelayFrqMaxOff = (((uint16_t)PS.Par.Abschaltverz) *10);
        } //--------------------------------------------------------------------
*/
        //Frequenz Max ---------------------------------------------------------
        //###test### Neu: Abschaltverzögerung ++
        if(IstFrqX10 >= DelayVglLimit2) //###test### Neu: Abschaltverzögerung ++ ###############
        {   //führt zum Abschalten
            if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit2 = (StatusLimit2 << 1); } //eine Null von rechts einschieben
            else              { StatusLimit2 = (StatusLimit2 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben 
        }
        else
        {   //führt zum Einschalten des Limit2 Bits  
            if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit2 = ((StatusLimit2 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit2 = ((StatusLimit2 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
            //cntDelayDz1off = (((uint16_t)PS.Par.Abschaltverz) *10);
        }
        if(IstFrqX10 < VglLimit2) cntDelayFrqMaxOff = (((uint16_t)PS.Par.Abschaltverz) *10);
        //----------------------------------------------------------------------
    }
    //Timeouts
    //StillstandsÜW
    if(cntTimeOut0 > timeout0)
    {   //Timeout "abgelaufen"
        StatusLimit0 = 0xFF;
        if((PS.Par.ControlBits & 0x04) != 0)    //Verzögerter Stillstand
        {   cntDelaySSToff = (((uint16_t)PS.Par.Abschaltverz) *10);
        }
    }  
    //FensterÜW
    //Muting hat höhere Priorität wie Timeout
    if(Muting == 1)
    {   //Frequenz Min
        StatusLimit1 = 0x00;    //führt zum Einschalten der FensterÜW
        cntDelayFrqMinOff = (((uint16_t)PS.Par.Abschaltverz) *10);
        //Frequenz Max
        StatusLimit2 = 0xFF;    //führt zum Einschalten der FesterÜW
        cntDelayFrqMaxOff = (((uint16_t)PS.Par.Abschaltverz) *10);
    }
    else //Timeouts
    {   //Frequenz Min
        if(cntTimeOut1 > timeout1)
        {   //Timeout "abgelaufen"
            if(cntDelayFrqMinOff == 0)  //die Abschaltverzögerung muss noch abgelaufen sein
            {   StatusLimit1 = 0xFF;    //führt zum Abschalten
            }
        }
        //Frequenz Max
        if(cntTimeOut2 > timeout2)
        {   //Timeout "abgelaufen"
            StatusLimit2 = 0xFF;    //führt zum Einschalten
            cntDelayFrqMaxOff = (((uint16_t)PS.Par.Abschaltverz) *10);
        }       
    }
      
    //Limit0, Limit1 und Limit2 Bits auf IPK setzten
    if((StatusLimit0 & ((1 << PS.Par.XfachMessung) -1)) == 0) TX.bf.Limit0 = 0;
	else                                                      TX.bf.Limit0 = 1;
	if((StatusLimit1 & ((1 << PS.Par.XfachMessung) -1)) == 0) TX.bf.Limit1 = 0;
	else					       					          TX.bf.Limit1 = 1;
    if((StatusLimit2 & ((1 << PS.Par.XfachMessung) -1)) == 0) TX.bf.Limit2 = 0;
	else					       					          TX.bf.Limit2 = 1;  
} //Ende FensterFrqVergl()------------------------------------------------------


//------------------------------------------------------------------------------
//Wird alle 1ms aufgerufen, if(PS.Par.DrehzahlMode == 7)
//------------------------------------------------------------------------------
void BereichFrqVergl(void)
{   static uint16_t VglLimit0 = 0, VglLimit1 = 0, VglLimit2 = 0, VglLimit3 = 0;
    static uint16_t timeout0 = 0, timeout1 = 0, timeout2 = 0, timeout3 = 0;
    
    if(fl10ms == 1) //----------------------------------------------------------
    {   //Limit0
        FrqLimit0 = PS.Par.arrFrqLimits[0][0];
        FrqLimHyst0 = PS.Par.arrFrqHyst[0][0];
        timeout0 = PS.Par.arrFrqTimeout[0][0];
        //Limit1     
        FrqLimit1 = PS.Par.arrFrqLimits[1][0];
        FrqLimHyst1 = PS.Par.arrFrqHyst[1][0];
        timeout1 = PS.Par.arrFrqTimeout[1][0];
        //Limit2      
        FrqLimit2 = PS.Par.arrFrqLimits[2][0];
        FrqLimHyst2 = PS.Par.arrFrqHyst[2][0];
        timeout2 = PS.Par.arrFrqTimeout[2][0];
        //Limit3      
        FrqLimit3 = PS.Par.arrFrqLimits[3][0];
        FrqLimHyst3 = PS.Par.arrFrqHyst[3][0];
        timeout3 = PS.Par.arrFrqTimeout[3][0];       
     
    } //------------------------------------------------------------------------
      
    //1ms Raster, die Werte aus dem 10ms Raster können hier verwendet werden
    if(TX.bf.Limit0 == 1) VglLimit0 = FrqLimit0;   //Limit0 ist nicht überschritten
    else                  VglLimit0 = FrqLimHyst0;
    if(TX.bf.Limit1 == 1) VglLimit1 = FrqLimit1;   //Limit1 ist nicht überschritten
    else                  VglLimit1 = FrqLimHyst1;
    if(TX.bf.Limit2 == 1) VglLimit2 = FrqLimit2;   //Limit2 ist nicht überschritten
    else                  VglLimit2 = FrqLimHyst2;
    if(TX.bf.Limit3 == 1) VglLimit3 = FrqLimit3;   //Limit2 ist nicht überschritten
    else                  VglLimit3 = FrqLimHyst3;
       
    //1ms Raster
    //Vergleich von Ist und Soll Frequenz, Status setzen
    //Bei NeueMessung==1: Bei IstDrehzahl > Soll in Status eine 0 einschieben, andernfalls eine 1
    //Wenn keine neue Messung vorliegt, wird die StatusLimit-Register nicht geändert, 
    //außer bei Timeouts.
    
    if(NeueMessung == 1)
    {   NeueMessung = 0;    //wird in getFrequenz() bei neuer Signalflanke gesetzt.
        //Limit0 ---------------------------------------------------------------
        if(IstFrqX10 >= VglLimit0)
        {   //führt zum Abschalten des Limit0 Bits
            if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit0 = (StatusLimit0 << 1); } //eine Null von rechts einschieben
            else              { StatusLimit0 = (StatusLimit0 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben
        }
        else
        {   //führt zum Einschalten des Limit0 Bits
            if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit0 = ((StatusLimit0 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit0 = ((StatusLimit0 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
        }
        //Limit1 ---------------------------------------------------------------
        if(IstFrqX10 >= VglLimit1)
        {   //führt zum Abschalten des Limit1 Bits           
            if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit1 = (StatusLimit1 << 1); } //eine Null von rechts einschieben
            else              { StatusLimit1 = (StatusLimit1 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben 
        }
        else
        {   //führt zum Einschalten des Limit1 Bits  
            if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit1 = ((StatusLimit1 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit1 = ((StatusLimit1 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
        }
        //Limit2 ---------------------------------------------------------------
        if(IstFrqX10 >= VglLimit2)
        {   //führt zum Abschalten des Limit2 Bits
            if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit2 = (StatusLimit2 << 1); } //eine Null von rechts einschieben
            else              { StatusLimit2 = (StatusLimit2 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben 
        }
        else
        {   //führt zum Einschalten des Limit2 Bits
            if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit2 = ((StatusLimit2 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit2 = ((StatusLimit2 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
        }
        //Limit3 ---------------------------------------------------------------
        if(IstFrqX10 >= VglLimit3)
        {   //führt zum Abschalten des Limit3 Bits
            if(IstFrqX10 >= PS.Par.FrqSchwelle) { StatusLimit3 = (StatusLimit3 << 1); } //eine Null von rechts einschieben
            else              { StatusLimit3 = (StatusLimit3 << PS.Par.XfachMessung); } //Xfach Nullen von rechts einschieben 
        }
        else
        {   //führt zum Einschalten des Limit2 Bits
            if(IstFrqX10 >= PS.Par.FrqSchwelle)         { StatusLimit3 = ((StatusLimit3 << 1) +1); } //eine 1 von rechts einschieben
            else { StatusLimit3 = ((StatusLimit3 << PS.Par.XfachMessung) +((1 << PS.Par.XfachMessung) -1)); } //Xfach 1en von rechts einschieben
        } //--------------------------------------------------------------------
    }
    //Timeouts
    if(cntTimeOut0 > timeout0)
    {   //Timeout "abgelaufen"
        StatusLimit0 = 0xFF;
    }
    if(cntTimeOut1 > timeout1)
    {   //Timeout "abgelaufen"
        StatusLimit1 = 0xFF;
    }
    if(cntTimeOut2 > timeout2)
    {   //Timeout "abgelaufen"
        StatusLimit2 = 0xFF;
    }
    if(cntTimeOut3 > timeout3)
    {   //Timeout "abgelaufen"
        StatusLimit3 = 0xFF;
    } 
  
      
    //Limit Bits auf IPK setzten
    if((StatusLimit0 & ((1 << PS.Par.XfachMessung) -1)) == 0) TX.bf.Limit0 = 0;
	else                                                      TX.bf.Limit0 = 1;
	if((StatusLimit1 & ((1 << PS.Par.XfachMessung) -1)) == 0) TX.bf.Limit1 = 0;
	else					       					          TX.bf.Limit1 = 1;
    if((StatusLimit2 & ((1 << PS.Par.XfachMessung) -1)) == 0) TX.bf.Limit2 = 0;
	else					       					          TX.bf.Limit2 = 1;
    if((StatusLimit3 & ((1 << PS.Par.XfachMessung) -1)) == 0) TX.bf.Limit3 = 0;
	else					       					          TX.bf.Limit3 = 1;
    
} //Ende BereichFrqVergl()------------------------------------------------------

