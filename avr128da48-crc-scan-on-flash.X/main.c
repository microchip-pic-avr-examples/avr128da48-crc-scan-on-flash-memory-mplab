 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"

/**
    Uncomment line 44 (#define CRC32) to use CRC32 firmware.
 
    Other changes required for CRC32:
    1)  Change SYSCFG0 fuse (MCC Generated Files/device_config.c/->line 44) to
            .SYSCFG0 = CRCSEL_CRC32_gc | CRCSRC_NOCRC_gc | RSTPINCFG_GPIO_gc,
    2) Change post build command as mentioned here:
 https://github.com/microchip-pic-avr-examples/avr128da48-crc-scan-on-flash-memory-mplab
 **/
//#define CRC32

#define PAGE_SIZE   (512)
#define LED_ON()  LED0_SetLow()
#define LED_OFF() LED0_SetHigh()
#define CRC_LENGTH  (4)

#ifdef CRC32
#define FORMAT "0x%08lX"
#else
#define FORMAT "0x%04lX"
#endif

void PerformCrcFinalXOR(void);
void StartCrcScan(void);
void ApplicationTask(void);
void PITInterrupt(void);
void SwitchPressInterrupt(void);
void FLASH_WriteFlashBlock(flash_address_t flash_adr, uint8_t *data, size_t size);
extern void (*RTC_PIT_isr_cb)(void);


volatile uint8_t pitInterruptFlag = 1; //setting default value as 1 to perform crc at the start
volatile uint8_t switchPressFlag = 0;
const flash_address_t appDataAddress = 0x1FC01; //Address at which data will be toggled
flash_address_t crcStoredAddress = 0x1FFFF; //Initializing  with last address of flash 
uint32_t preCalculatedCRC = 0; //variable to store crc 
uint8_t byteToBeWritten = 0xFF; //data to be written in the flash in runtime  
uint8_t flashPageBuffer[PAGE_SIZE] = {0}; //A buffer in memory the size of a flash page, used as a scratchpad

/*
    Main application
*/

int main(void)
{
    SYSTEM_Initialize();
    
    printf("\rSystem Initialized\n");

    /* Set interrupt handler for PIT interrupt */
    RTC_PIT_isr_cb = &PITInterrupt;
    /* Set interrupt handler for switch press interrupt */
    PC7_SetInterruptHandler(SwitchPressInterrupt);

    /* Read "Pre-calculated crc" from flash */
    preCalculatedCRC = (uint32_t) FLASH_Read(crcStoredAddress);
    preCalculatedCRC |= (uint32_t) FLASH_Read(--crcStoredAddress) << 8;
 
#ifdef CRC32    
    preCalculatedCRC |= (uint32_t)FLASH_Read(--crcStoredAddress) << 16;
    preCalculatedCRC |= (uint32_t)FLASH_Read(--crcStoredAddress) << 24;
#endif
    
    printf("\n\rPre-calculated CRC read from Flash is complete\n");
    printf("\rPre-calculated CRC : " FORMAT "\n", preCalculatedCRC);

#ifdef CRC32     
    /* Perform XOR on CRC by calling function */
    PerformCrcFinalXOR();
#endif


    while(1)
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
  This function reads the store CRC from flash, XOR the CRC with 0xFFFF
  and store the CRC in same the address.
	 
  Note: CRC32 requires Final XOR with checksum. This is required to perform by 
  MCU as, in HEXMATE there is no option to perform final XOR with checksum.
 **/
void PerformCrcFinalXOR(void) {
    uint8_t crcTempArray[CRC_LENGTH] = {0};  

    /* XOR CRC with 0xFFFFFFFF */
    preCalculatedCRC ^= 0xFFFFFFFF;

    /* Storing CRC in Big Endian order */ 
    crcTempArray[0] = (uint8_t) (preCalculatedCRC >> 24);
    crcTempArray[1] = (uint8_t) (preCalculatedCRC >> 16);
    crcTempArray[2] = (uint8_t) (preCalculatedCRC >> 8);
    crcTempArray[3] = (uint8_t) (preCalculatedCRC);

    printf("\n\rUpdated CRC: 0x%08lX\n", preCalculatedCRC);

    /* Update CRC in the same address */
    FLASH_WriteFlashBlock(crcStoredAddress, crcTempArray, CRC_LENGTH);

    printf("\n\rStored Updated CRC in Flash\n");

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
        /* Clear flag */
        pitInterruptFlag = 0;

        StartCrcScan();

        /* CRC Calculation is successful */
        if (CRCSCAN_STATUS & CRCSCAN_OK_bm) 
        {
            /* Turn off LED */
            LED_OFF();
            printf("\rCRC matched with Pre-calculated CRC " FORMAT "\n", preCalculatedCRC);

        }
        else 
        {
            /* Turn on LED */
            LED_ON();
            printf("\rCRC did not match with Pre-calculated CRC " FORMAT "\n", preCalculatedCRC);

        }
    }

    if (switchPressFlag == 1) 
    {
        /* Clear flag */
        switchPressFlag = 0;

        /* Toggle byte between 0x00 and 0xFF */
        byteToBeWritten = ~(byteToBeWritten);

        /* Write byteToBeWritten at appDataAddress */
        FLASH_WriteFlashBlock(appDataAddress, &byteToBeWritten,1);

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
    /* Start CRCSCAN */
    CRCSCAN_CTRLA = (1 << CRCSCAN_ENABLE_bp);

    /* Wait for CRCSCAN to complete */
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
  @Param
   flash_address The byte-address of the flash to write to
   data The data to write to the flash
   size The size of the data (in bytes) to write to the flash
  @Returns
   none
  @Description
   Update the specific locations in a given Flash page without disturbing the other locations
 **/
void FLASH_WriteFlashBlock(flash_address_t flash_address, uint8_t *data, size_t size)
{
    flash_address_t flashStartPageAddress;
    uint16_t flashAddressOffset;
    flash_data_t flashWriteData[PROGMEM_PAGE_SIZE];

    //Get the starting address of the page containing the given address
    flashStartPageAddress = FLASH_PageAddressGet(flash_address);

    //Read entire row
    for (flashAddressOffset = 0; flashAddressOffset < PROGMEM_PAGE_SIZE; flashAddressOffset++)
    {
        flashWriteData[flashAddressOffset] = FLASH_Read(flashStartPageAddress + flashAddressOffset);
    }

    //Get offset from the starting address of the page
    flashAddressOffset = FLASH_PageOffsetGet(flash_address);

    //Update data of required size
    for (uint8_t i=0; i<size; i++)
    {
        flashWriteData[flashAddressOffset + i] = *(data+ i);
    }

    //Erase the entire Flash page
    FLASH_PageErase(flashStartPageAddress);
 
    //Write data to the Flash row
    FLASH_RowWrite(flashStartPageAddress, flashWriteData);  
}
/**
    End of File
 */