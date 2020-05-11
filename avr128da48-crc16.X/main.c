/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

#define PAGE_SIZE   (512)
#define LED_ON()  LED0_SetLow()
#define LED_OFF() LED0_SetHigh()

void StartCrcScan(void);
void ApplicationTask(void);
void PITInterrupt(void);
void SwitchPressInterrupt(void);
extern void (*RTC_PIT_isr_cb)(void);


volatile uint8_t pitInterruptFlag = 0;
volatile uint8_t switchPressFlag = 0; 
const flash_adr_t crcStoredAddressMSB = 0x1FFFE; //Address at which crc MSB is stored
const flash_adr_t crcStoredAddressLSB = 0x1FFFF; //Address at which crc LSB is stored
const flash_adr_t appDataAddress = 0x1FC01; //Address at which data will be toggled
uint16_t preCalculatedCRC = 0; //variable to store crc 
uint8_t byteToBeWritten = 0xFF; //data to be written in the flash in runtime  
uint8_t flashPageBuffer[PAGE_SIZE] = {0}; //A buffer in memory the size of a flash page, used as a scratchpad


/*
    Main application
*/
int main(void)
{
    /* Initializes MCU, drivers and middleware */
    SYSTEM_Initialize();
    
    printf("\rSystem Initialized\n");
    
    //set interrupt handler for PIT interrupt
    RTC_PIT_isr_cb = &PITInterrupt;
    //set interrupt handler for switch press interrupt
    PORTC_SW0_SetInterruptHandler(SwitchPressInterrupt);
     
    //read "Pre-calculated crc" from flash
    preCalculatedCRC = FLASH_ReadFlashByte(crcStoredAddressLSB);
    preCalculatedCRC |= FLASH_ReadFlashByte(crcStoredAddressMSB) << 8;
    
    printf("\n\rPre-calculated CRC read from Flash is complete\n");
    printf("\rPre-calculated CRC : 0x%04X\n", preCalculatedCRC);
   
    
    /* application code */
    while (1)
    {
        ApplicationTask();
    }
    
}

/**
  @Param
  none
  @Returns
   none
  @Description
  This function checks whether the crc scan result and displays the status.
  It also toggles a byte data in APPDATA section upon switch press
  **/
void ApplicationTask(void) 
{
    if (pitInterruptFlag == 1) 
    {
        //clear flag
        pitInterruptFlag = 0;

        StartCrcScan();
        
        //CRC Calculation is successful 
        if (CRCSCAN_STATUS & CRCSCAN_OK_bm) 
        {
            // Turn off LED
            LED_OFF();
            printf("\rCRC matched with Pre-calculated CRC 0x%04X\n", preCalculatedCRC);

        } 
        else 
        {
            // Turn on LED
            LED_ON();
            printf("\rCRC did not match with Pre-calculated CRC 0x%04X\n", preCalculatedCRC);

        }
    }


    if (switchPressFlag == 1) 
    {
        //clear flag
        switchPressFlag = 0;

        //toggle byte between 0x00 and 0xFF
        byteToBeWritten = ~(byteToBeWritten);

        //write byteToBeWritten at appDataAddress
        FLASH_WriteFlashByte(appDataAddress, flashPageBuffer, byteToBeWritten);

        printf("\n\r0x%02X is written at 0x%lX in APPDATA section of Flash\n",
                byteToBeWritten, appDataAddress);

    }
}

/**
  @Param
  none
  @Returns
   none
  @Description
  This function enables crc scan and waits until crc scan is complete.
  **/ 
void StartCrcScan(void) 
{
    printf("\n\rStarted CRC Scan..\n");
    // start CRCSCAN
    CRCSCAN_CTRLA = (1 << CRCSCAN_ENABLE_bp);

    //wait for CRCSCAN to complete 
    while ((CRCSCAN.STATUS & CRCSCAN_BUSY_bm));
}

/**
  @Param
  none
  @Returns
   none
  @Description
  PIT Interrupt handler. Interrupt in every 8 sec
  **/
void PITInterrupt(void)
{
    pitInterruptFlag = 1; 
}

/**
  @Param
  none
  @Returns
   none
  @Description
  IOC handler for pin PC7. Interrupt upon switch press
  **/
void SwitchPressInterrupt(void)
{
    switchPressFlag = 1;
}

/**
    End of File
*/