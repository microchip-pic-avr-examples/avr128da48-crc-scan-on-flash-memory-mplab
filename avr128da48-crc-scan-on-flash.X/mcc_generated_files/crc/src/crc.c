/**
  CRCSCAN Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    crcscan.c

  @Summary
    This is the generated driver implementation file for the CRCSCAN

  @Description
    This source file provides APIs for CRCSCAN.
    Generation Information :
        Driver Version    :  1.0.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 v2.20
        MPLAB             :  MPLABX v5.40
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

#include "../crc.h"

/**
 * \brief Initialize crcscan interface
 */
int8_t CRC_Initialize(void) 
{
    //SRC CRC on entire flash; 
    CRCSCAN.CTRLB = 0x0;

    //ENABLE disabled; NMIEN disabled; RESET disabled; 
    CRCSCAN.CTRLA = 0x0;

    return 0;
}

/**
 * \brief Reset CRCSCAN module
 *
 * \return The status whether CRCSCAN is reset or not
 * \retval false The CRCSCAN module is not reset
 * \retval true The CRCSCAN module is reset
 */
bool CRC_Reset(void) 
{
    if ((CRCSCAN.CTRLA & CRCSCAN_NMIEN_bm) && (CRCSCAN.STATUS & CRCSCAN_BUSY_bm)) {
        return false;
    } else {
        CRCSCAN.CTRLA |= CRCSCAN_RESET_bm;
        return true;
    }
}

ISR(NMI_vect)
{
    /* Insert your CRCSCAN interrupt handling code here */
}