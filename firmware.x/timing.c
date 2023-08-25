#include <xc.h>
#include "p33CK256MP505.h"
#include "timing.h"
#include "RamTest.h"
#include <stdint.h>

uint8_t fl05ms = 0, fl1ms = 0, fl2ms = 0, fl4ms = 0, fl10ms = 0, fl100ms = 0;
uint8_t fl300ms = 0;

void timing(void)
{   //wird alle 0,5ms aufgerufen
    static uint8_t cnt1ms = 0, cnt2ms = 0, cnt4ms = 0, cnt10ms = 0, cnt100ms = 0;
    static uint16_t cnt300ms = 0;   // cnt500ms = 0, cnt1000ms = 0;
    
    IR_fl1ms = 0;
    
    //Bei CCP1 overflow das flag löschen, damit CCP1 nicht blockiert wird.
    if(CCP1STATLbits.ICOV == 1)  CCP1STATLbits.ICOV = 0;
    
    fl05ms = 1;
    
    cnt1ms ++;
    if(cnt1ms == 2)
    {	cnt1ms = 0;
        fl1ms = 1;
    }
    cnt2ms ++;
    if(cnt2ms == 4)
    {	cnt2ms = 0;
        fl2ms = 1;
    }
    cnt4ms ++;
    if(cnt4ms == 8)
    {	cnt4ms = 0;
        fl4ms = 1;
    }
    cnt10ms ++;
    if(cnt10ms == 20)
    {	cnt10ms = 0;
        fl10ms = 1;
    }
    /*
    cnt50ms ++;
    if(cnt50ms == 100)
    {	cnt50ms = 0;
        fl50ms = 1;
    }
    */
    cnt100ms ++;
    if(cnt100ms == 200)
    {	cnt100ms = 0;
        fl100ms = 1;
        if(cntLedAlternativ > 0) cntLedAlternativ --;
        else				     flLedAlternativ = 0;
    }
    cnt300ms ++;
    if(cnt300ms == 600)
    {	cnt300ms = 0;
        fl300ms = 1;
    }
    /*
    cnt500ms ++;
    if(cnt500ms == 1000)
    {	cnt500ms = 0;
        fl500ms = 1;
    }
    cnt1000ms ++;
    if(cnt1000ms == 2000)
    {	cnt1000ms = 0;
        fl1000ms = 1;
    } 
    */
    
    //Programmlaufzeit Kontrolle
    
    cntTimingMain = 0;
    cntTiming ++;
    if(cntTiming > 50) ;    //###test###ErrorLoop(0xF0);
}

void Reset_Cnt_Flags(void)
{   //Am Ende der main() Schleife werden Flags und cnt gelöscht.
    fl05ms = 0;
    fl1ms = 0;
    fl4ms = 0;
    fl10ms = 0;
    fl100ms = 0;
    fl300ms = 0;
    cnt1msRoutinen = 0;
    cnt4msRoutinen = 0;
    cnt10msRoutinen = 0;
    cnt100msRoutinen = 0;   
}


