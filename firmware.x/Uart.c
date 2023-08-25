#include <xc.h>
#include "p33CK256MP505.h"
#include "Uart.h"
#include "main.h"
#include <stdint.h>

// UartRxTx() wird alle 1ms aufgerufen.
// Steuert die IPK (serielle Schnittstelle) der µC.
// SerTX[] und SerRX[] halten die 20 Datenbytes, die seriell ausgetauscht werden.
// cntSer zählt von 0 bis 19. Das Byte SerTX[0] wird mit Bit9=1 gesendet.
// Dadurch wird die Ser.Schnittstelle synchronisiert.
// Nach aufrufen von UartRxTx() wird das Receiveregister RCREG nach SerRX[cntSer] gesp.
// Dann cntSer ++ und SerTX[cntSer] nach TXREG geschrieben und damit gesendet.
// Der Slave-µC hat 1ms Zeit um das entsprechende Byte zurückzusenden.


uint8_t flSerTOErr = 0, cntSerTOErr = 0;
//uint16_t cntReceiveTOErr = 0;
str_ipk RX, TX, LatchRX, LatchTX;


//------------------------------------------------------------------------------
void UartRxTx(void)
{   uint8_t RXwert = 0, Bit9 = 0, i = 0;
    static uint8_t cntSer = ProtokollUart-1;

    cnt1msRoutinen ++;
    
    //received byte ------------------------------------------------------------
    RXwert = 0;
    if(U1STAHbits.URXBE == 0)
    {   //Buffer not empty
        if(U1STAbits.PERR == 1) Bit9 = 1;      //Address Mode: 1 = Address received. PERR is readonly.
        else                    Bit9 = 0;  
        //if(cntReceiveTOErr > 0) cntReceiveTOErr --;
    } 
    else
    {   //Receivebuffer ist empty, nichts empfangen
        ; //if(cntReceiveTOErr < 2002) cntReceiveTOErr +=2;
          //if(cntReceiveTOErr > 2000) { flSerTOErr = 1; CanTX[0] |= 0x80; }
    }
    
    while(U1STAHbits.URXBE == 0)          //lesen bis der Buffer empty ist. Empty: URXBE=1
    {       RXwert = U1RXREG;    
    }
    //U1MODEbits.UARTEN = 0;      //disable Erratasheet###test###
    //U1STAbits.OERR = 0;
    //U1MODEbits.UARTEN = 1;      //enabled
    
    if(cntSer == 0)
    {   if(Bit9 == 1)
        {   //Bit9==1, OK
            SerRXU[0] = RXwert;
            cntSer ++;
        }
        else    
        {   //Bit9==0, falscher Wert  
            if(TX.bf.ParamKompl == 1) cntSerTOErr +=2;
            else                      cntSerTOErr = 0;
            if(cntSerTOErr > 250)
            {	flSerTOErr = 1;
                cntSerTOErr = 250;              // verhindert Überlauf
            }
        }            
    }
    else    //cntSer > 0
    {   if(Bit9 == 0)
        {   //bit9==0, OK
            SerRXU[cntSer] = RXwert; //U1RXREG wurde oben schon zwischengespeichert
            //nach dem letzten gesendeten Datenbyte SerTXU[17] Puffer tauschen
            //SerRXU[] / SerTXU[] sind die Uart schreib/lese Arrays
            //SerRXB[] / SerTXB[] sind die gepufferten Arrays
            //SerRX[] / SerTX[] sind die schreib/lese Arrays für main()
            //In SerRXU sind die aktellen RX Daten
            //In SerTXB sind die aktuellen TX Daten 
            if(cntSer == (ProtokollUart -1))
            {   //Zu diesem Zeitpunkt wird der 
                //empfangene IPK-Datensatz SerRXU[0-19] dem Programm übergeben,
                //und ein neuer Datensatz SerTXB[0-19] zum Senden geladen.
                //for(i=0; i < ProtokollUart; i++)
                //{   SerTXU[i] = SerTXB[i];
                //    SerRXB[i] = SerRXU[i];                    
                //}
                
                if((LatchTX.bf.Frametyp != 0)&&(flLatchTX == 1))
                {   //Latchbit ist gesetzt, der Frame wird einmalig in SerTXU[] kopiert.
                    flLatchTX = 0;
                    for(i=0; i < ProtokollUart; i++)
                    {   SerTXU[i] = LatchTX.Bytes[i];
                    }                   
                }
                else    //Normalfall
                {   for(i=0; i < ProtokollUart; i++)
                    {   SerTXU[i] = SerTXB[i];                    
                    }                    
                }
                               
                if((SerRXU[0] & 0x03) != 0)    //&&(flLatchRX == 0)) //wird wahrscheinlich für den Ablauf nicht benötigt.
                {   //Latchbit ist gesetzt, der Frame wird einmalig in LatchRX kopiert.
                    //flLatchRX = 1;                                 //wird wahrscheinlich für den Ablauf nicht benötigt.
                    for(i=0; i < ProtokollUart; i++)
                    {   LatchRX.Bytes[i] = SerRXU[i];
                        SerRXB[i] = SerRXU[i];
                    }                   
                }
                else    //Normalfall
                {   for(i=0; i < ProtokollUart; i++)
                    {   SerRXB[i] = SerRXU[i];                    
                    }                    
                }
                //Zur Datensynchronisation für den VergleichIX1zuIX2()
                //wird IstDzIX1 gelatched nach IpkLatchIstIX1_a und vorher
                //der alten Latchwert weitergeschoben nach IpkLatchIstIX1_b.
                //Wenn der IPK-Datensatz SerRXU[0-17] nach einer Laufzeit von 
                //18x1ms angekommen ist, sind die Daten 18ms alt. 
                //Der 18ms alte IstDzIX1 steht dann in IpkLatchIstIX1_b.
                //VergleichIX1zuIX2() verwendet IpkLatchIstIX1_b.
                //IpkLatchIstIX1_b = IpkLatchIstIX1_a;
                //IpkLatchIstIX1_a = IstDzIX_verwendet1;
            }   
            cntSer ++;
            if(cntSer > (ProtokollUart-1)) cntSer = 0;
            if(cntSerTOErr >0)	cntSerTOErr --;
        }
        else //bit9==1, falscher Wert 
        {   cntSer = 0;    
            if(TX.bf.ParamKompl == 1) cntSerTOErr +=2;
            else                      cntSerTOErr = 0;
            if(cntSerTOErr > 250)
            {	flSerTOErr = 1;
                cntSerTOErr = 250;               // verhindert Überlauf
            }
        }
    }

    //transmit byte ------------------------------------------------------------     
    if(cntSer == 0) 
    {   //U1P1 beschreiben: Das Byte wird mit gesetztem 9.Bit gessendet
        U1P1 =    SerTXU[cntSer]; 
        //U1TXREG = SerTXU[cntSer]; 
    }
    else
    {   //TX 9.bit nicht setzen
        U1TXREG = SerTXU[cntSer]; 
    } 
}   //UartRxTx() End -----------------------------------------------------------


void SteckplatzAustauschen(uint16_t SteckplatzNr)
{   uint8_t stateSteckplatz = 0;
    while(IFS0bits.T1IF == 0)   ;   //warten
    IFS0bits.T1IF = 0;
    while(1)
    {   switch(stateSteckplatz)
        {   case 0:
                U1P1    = (SteckplatzNr & 0xFF); //9. Bit ist gesetzt, da Address Mode
                //U1TXREG = (SteckplatzNr & 0xFF);
                while(IFS0bits.T1IF == 0)   ;   //warten
                IFS0bits.T1IF = 0;
              
                if(U1STAHbits.URXBE == 0)  stateSteckplatz = 1;  //RX Buffer not empty, es wurde ein byte empfangen
                else                      stateSteckplatz = 0;
                break;
            case 1:
 
                if((U1RXREG & 0xFF) == (SteckplatzNr & 0xFF)) 
                stateSteckplatz = 2;
                else                                                                            
                stateSteckplatz = 0;
                break;
            case 2:

                U1TXREG = 0xAA;
                while(IFS0bits.T1IF == 0)   ;   //warten
                IFS0bits.T1IF = 0;
                if(U1STAHbits.URXBE == 0)  stateSteckplatz = 3; //RX Buffer not empty, es wurde ein byte empfangen
                else                      stateSteckplatz = 0;
                break;
            case 3:
                if((U1RXREG & 0xFF) == 0xAA) stateSteckplatz = 4;
                else                         stateSteckplatz = 0;
                break;
        }
       if( stateSteckplatz == 4) 
       break; //break while(1)
    }
}

void UartConfig(void)
{   //#define ProtokollUart   18      //Array SerRX/TX[] 18byte groß
    
    /*
    UART1, TX=Pin29, RX=Pin30 
    RPINR18bits.U1RXR = 72;     //SerRX RP72
    RPOR12bits.RP57R = 1;       //SerTX RP57
    LATCbits.LATC9 = 1;         //SerTX
    TRISCbits.TRISC9 = 0;
    LATDbits.LATD8 = 0;         //SerRX
    TRISDbits.TRISD8 = 1;
    */   
    U1MODEHbits.BCLKSEL = 2;    //Fosc = 80MHz
    U1MODEHbits.BCLKMOD = 0;    //0: Legacy Mode. 1: Fractional Baudrate Generation.
    U1MODEbits.BRGH = 1;        //Baudrate: 0=clock/16. 1=clock/4
    U1BRG = 155;                 //128,2kBaud = 80MHz / ((1+ 155)*4)    //###test### mit 256kB testen mit U1BRG= 77
    U1BRGH = 0;   
    U1MODEbits.MOD = 4;         //Asynchron 9Bit Modus (mit address detect?###test###)
    //U1MODEbits.MOD = 0;       //Asynchron 8Bit Modus
    //Wenn U1P1 beschrieben wird, dann wird mit gesetztem 9. Bit gesendet.
    //Demnach könnte in U1P1 statt einer festen Adresse irgendwelche Daten geschrieben werden!!!!!!
    U1P2 = 0x11;                //RX Adresse im Address Mode. Notwendig für das 9. Bit
    U1P3 = 0x00;                //RX Adress Maske. Bei 0x00 wird jede Adresse akzeptiert
    U1MODEbits.UARTEN = 1;      //enabled
    U1MODEbits.URXEN = 1;       //RX enabled
    U1MODEbits.UTXEN = 1;       //TX enabled
    U1STA = 0;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;        //damit nach aktivieren der UART kein Interrupt ausgelöst wird
}

//------------------------------------------------------------------------------
//IpkDatenPuffern() wird am Ende von main() alle 1ms aufgerufen.
//Die Daten für die Serielle Schnittstelle TX.Bytes[] und RX.Bytes[] (früher SerRX/TX) werden in SerRXB/TXB.
//gepuffert. Der STM32 UART IR wird während der Pufferung gesperrt.
//In der UART ISR werden die Daten in SerRXU/TXU. gelesen/geschrieben
//SerRXU[] / SerTXU[] sind die Uart schreib/lese Arrays
//SerRXB[] / SerTXB[] sind die gepufferten Arrays
//RX.Bytes[] (SerRX[]) / TX.Bytes[] (SerTX[]) sind die schreib/lese Arrays für main()
//------------------------------------------------------------------------------
void IpkDatenPuffern(void)
{   uint8_t i;
    cnt1msRoutinen ++;
    //STM32: NVIC_IntDisable(IVT_INT_USART3);
    TX.Bytes[IPK_FW] =  FW_Version;
    TX.Bytes[IPK_End] = ProtokollEnde;
   
    for(i=0;i < (ProtokollUart);i++)
    {   SerTXB[i] = TX.Bytes[i];
        RX.Bytes[i] = SerRXB[i];
    }
    //STM32: NVIC_IntEnable(IVT_INT_USART3);
}
//------------------------------------------------------------------------------

void UartDebug(uint16_t wert1, uint16_t wert2)
{   uint16_t i;

    UartConfig();
    
    while(U1STAHbits.UTXBE == 0) { ; }      //Warten bis Sendepuffer leer ist.                     
    U1TXREG = (((uint16_t)(wert1)) >> 8);
    for(i = 1; i < 0x1500;  i ++)           //warteschleife 10ms
    { __builtin_nop(); }
    
    while(U1STAHbits.UTXBE == 0) { ; }      //Warten bis Sendepuffer leer ist.                     
    U1TXREG = ((uint16_t)(wert1));
    for(i = 1; i < 0x1500;  i ++)           //warteschleife 10ms
    { __builtin_nop(); }
    
    while(U1STAHbits.UTXBE == 0) { ; }      //Warten bis Sendepuffer leer ist.                     
    U1TXREG = (((uint16_t)(wert2)) >> 8);
    for(i = 1; i < 0x1500;  i ++)           //warteschleife 10ms
    { __builtin_nop(); }
    
    while(U1STAHbits.UTXBE == 0) { ; }      //Warten bis Sendepuffer leer ist.                     
    U1TXREG = ((uint16_t)(wert2));
    for(i = 1; i < 0x1500;  i ++)           //warteschleife 10ms
    { __builtin_nop(); }
	
    while(U1STAHbits.UTXBE == 0) { ; }      //Warten bis Sendepuffer leer ist.                     
    U1TXREG = ((uint16_t)0xFF);
    for(i = 1; i < 0x1500;  i ++)           //warteschleife 10ms
    { __builtin_nop(); }
}
