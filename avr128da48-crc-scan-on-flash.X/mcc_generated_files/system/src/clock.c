/**
  @Company
    Microchip Technology Inc.

  @Description
    This Source file provides APIs.
    Generation Information :
    Driver Version    :   1.0.0
*/
/*
Copyright (c) [2012-2020] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/


#include <xc.h>
#include "../clock.h"

void CLOCK_Initialize(void)
{
    // Set the CLKCTRL module to the options selected in the user interface.
    
    //CLKOUT disabled; CLKSEL Internal high-frequency oscillator; 
    ccp_write_io((void*)&(CLKCTRL.MCLKCTRLA),0x0);

    //PDIV 6X; PEN enabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKCTRLB),0x11);

    //LOCKEN disabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKLOCK),0x0);

    //EXTS disabled; OSCHFS disabled; OSC32KS disabled; PLLS disabled; SOSC disabled; XOSC32KS disabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKSTATUS),0x0);

    //AUTOTUNE disabled; FREQSEL 4 MHz system clock (default); RUNSTDBY disabled; 
    ccp_write_io((void*)&(CLKCTRL.OSCHFCTRLA),0xC);

    //TUNE 0x0; 
    ccp_write_io((void*)&(CLKCTRL.OSCHFTUNE),0x0);

    //RUNSTDBY disabled; 
    ccp_write_io((void*)&(CLKCTRL.OSC32KCTRLA),0x0);

    //MULFAC PLL is disabled; RUNSTDBY disabled; SOURCE disabled; 
    ccp_write_io((void*)&(CLKCTRL.PLLCTRLA),0x0);

    //CSUT 1k cycles; ENABLE disabled; LPMODE disabled; RUNSTDBY disabled; SEL disabled; 
    ccp_write_io((void*)&(CLKCTRL.XOSC32KCTRLA),0x0);

}

/**
 End of File
*/