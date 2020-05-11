<!-- Please do not change this html logo with link -->
<a href="https://www.microchip.com" rel="nofollow"><img src="images/microchip.png" alt="MCHP" width="300"/></a>

# CRC SCAN ON FLASH MEMORY

This example demonstrates usage of Cyclic Redundancy Check Memory Scan (CRCSCAN) peripheral to perform CRC operation on the Flash memory and Nonvolatile Memory Controller (NVMCTRL) peripheral to perform read and write operations on the Flash memory in runtime.

In this example, the CRCSCAN peripheral periodically performs CRC scan on Flash memory by calculating CRC checksum of the whole Flash and this is compared to a pre-calculated checksum. Hexmate tool which comes with MPLAB X IDE is used to pre-calculate CRC checksum and the calculated checksum is stored at a specific address in the Flash.

The result of CRC scan is displayed on a terminal window of data visualizer as well as indicated by on-board LED (LED0) of AVR128DA48 Curiosity Nano board. Upon switch (SW0) press event, one-byte of data in the Flash memory is modified using NVMCTRL peripheral. This results in CRC failure during the subsequent CRC scan.

**This example contains two firmware projects, one for CRC-16 and other for CRC-32. Both the firmwares are independent and showcase the usage of CRC-16 and CRC-32, respectively.**

**Note:** *This example is valid for all the AVR DA family MCUs*

## Useful Links
- [AVR128DA48 Product Page](https://www.microchip.com/wwwproducts/en/AVR128DA28 "AVR128DA48 Product Page")
- [AVR128DA48 Code Examples on GitHub](https://github.com/microchip-pic-avr-examples?q=avr128da48 "AVR128DA48 Code Examples on GitHub")
- [AVR128DA48 Project Examples in START](https://start.atmel.com/#examples/AVR128DA48CuriosityNano "AVR128DA48 Project Examples in START")
- [CRC SCAN ON FLASH MEMORY - Studio Example](avr128da48-crc-scan-on-flash-memory-mplab "CRC SCAN ON FLASH MEMORY - Studio Example")
- [Hexmate](https://microchipdeveloper.com/xc8:hexmate-cli "Hexmate")

## Application Code Flow Diagram
<br><img src="images/firmware-flowchart.png" width="400">

## Software Used
- [MPLAB X IDE v5.40](https://www.microchip.com/mplab/mplab-x-ide "MPLAB X IDE v5.40")
- [XC8 Compiler v2.20](https://www.microchip.com/mplab/compilers "XC8 Compiler v2.20")
- [MPLAB Code Configurator (MCC) v3.95.0](https://www.microchip.com/mplab/mplab-code-configurator "MPLAB Code Configurator (MCC) v3.95.0")
- AVR8bit Lib v2.2.2
- AVR-Dx DFP 1.1.40
- [Standalone Data Visualizer v2.20.674](https://www.microchip.com/mplab/avr-support/data-visualizer "Standalone Data Visualizer v2.20.674")

**Note:** *For running the demo, the installed tool versions should be same or later.*

## Hardware used
[AVR128DA48 Curiosity Nano Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM164151 "AVR128DA48 Curiosity Nano Evaluation Kit")
<br><img src="images/curiosity-nano.png" width="500">

## Setup
Connect AVR128DA48 Curiosity Nano Board to Host Computer (PC) using standard Micro B USB cable.

## Operation
1. Open Standalone Data Visualizer. Select Virtual COM Port to which AVR DA nano board is connected. Click **Connect**.

    **Note:** *Alternately, open the MPLAB Data Visualizer plugin extension available to MPLAB X IDE.*
  <br><img src="images/data-visualizer.png" width="500">

2. Open the crc16/crc32 project in MPLAB X IDE. (File -> Open Project)
3. Clean the project. (Right click on project name and click Clean)
4. Click **Make and Program Device**.
5. Once programming is completed, terminal window should start displaying the messages.
<br><img src="images/dv1.png" width="500">

6. Scan starts after 8 sec. Result should be printed on terminal.
<br><img src="images/dv2.png" width="500">

7. Press Switch (SW0) to change a byte at location “0x1FC01” in the Flash memory to 0x00.
<br><img src="images/dv3.png" width="500">

8. Subsequent CRC scan will result in a failure and LED0 on Curiosity nano board turns ON.
<br><img src="images/dv4.png" width="500">

9. Press switch again to change data back to 0xFF.
<br><img src="images/dv5.png" width="500">

10. Subsequent scan will result in CRC success. LED turns OFF.
<br><img src="images/dv6.png" width="500">

**Note:** *Clean the project and then program the device by clicking* **Make and Program Device** ![](images/program-device-icon.png)

## Summary
This example demonstrates how CRCSCAN peripheral can be used to perform integrity check on Flash memory. It also demonstrates use of MNVCTRL peripheral to perform read and write operations on Flash memory in runtime.
The Below sections give brief explanation of Flash memory structure and CRC peripheral of AVR DA family for better understanding of the demo. Firmware generation section explains firmware generation using MCC and MPLAB X Linker options and Hexmate tools options used for this demo.

## Appendix:
### Flash Memory in AVR DA
The AVR128DA28/32/48/64 devices contain 128 KB on-chip in-system reprogrammable Flash memory for program storage. For write protection, the Flash program memory space can be divided into three sections: Boot Code section, Application Code section, and Application Data section.
Refer below table for details of Flash section write access.
<br><img src="images/table.png" width="500">

**In this example, program is placed in APPCODE section and a byte from APPDATA section is being modified upon a switch press event.**

**Note:** *Refer* **add linker command** *subsection in* **Firmware Generation** *section to know about how to place program at a specific memory address*

#### Section Sizes

The sizes of these sections are set by the Boot Size (FUSE.BOOTSIZE) fuse and the Code Size (FUSE.CODESIZE) fuse. The fuses select the section sizes in blocks of 512 bytes. The BOOT section stretches from FLASHSTART to BOOTEND. The APPCODE section spreads from BOOTEND until APPEND. The remaining area is the APPDATA section.

**In this example, FUSE.BOOTSIZE is set to 2 and FUSE.CODESIZE is set to 254. Thus, first 2*512 bytes (1KB) is BOOT, the next 250*2 bytes (126 KB) is APPCODE and remaining 2*512 bytes (1KB) is APPDATA.**



### CRCSCAN in AVR DA
#### CRC SCAN Options

The CRC scan can be set up to scan the entire Flash, only the boot section, or both the boot and application code sections.  **In this example, it is set to scan entire Flash.**

The CRCSCAN can be configured to perform a code memory scan before the device leaves Reset. If this check fails, the CPU is not allowed to start normal code execution. This option is enabled for CRC-16 firmware.

**Note:** *For CRC-32, XOR operation needs to be performed on the generated checksum.  This option is not available with Hexmate. The Final XOR operation is performed by the MCU. So, this option cannot be enabled for CRC-32.*



#### CRC Polynomials Options

The CRC generator supports CRC-16-CCITT and CRC-32 (IEEE 802.3).

The polynomial options are:
- CRC-16-CCITT: x16 + x12 + x5 + 1
- CRC-32: x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1


**This example contains two firmware projects, one for CRC-16 and other for CRC-32.**


#### Location to store pre-calculated CRC

The pre-calculated checksum must be present in the last location of the section to be checked. **As this example checks CRC for entire Flash, CRC is placed in last location of Flash. (which is happened to be in APPDATA section)**

**Note:** *Refer* **add post build command** *subsection in* **Firmware Generation** *section to know how to pre-calculate CRC Flash memory and place in last location of Flash using Hexmate.*
<br><img src="images/memory.png" width="500">


### Firmware Generation
This section explains how to generate firmware using MCC.
#### 1. Create a New MPLAB X Project and Open MCC
- Open the MPLAB X IDE
- Create New project: File -> New Project
- Select Microchip Embedded -> Standalone Project
- Select AVR128DA48 as the target device
- Select XC8 as compiler
- Open MCC by clicking the MCC icon in the toolbar. ![](images/mcc-icon.png)

#### 2. Configure MCC
##### o	Add Peripherals to project
- Add CRCSCAN, NVMCTRL, RTC, USART1 to the project.
<br><img src="images/add-peripherals.png" width="450">

#### o	Set Fuses
Fuse setting options are available under Registers tab of System Module.
- Set **BOOTSIZE** Register to **0x2**
- Set **CODESIZE** Register to **0xFE**
- Configure **CRCSEL** in **SYSCFG0** register to **Enable CRC 16**

  **Note:** *In case of CRC-32, configure* **CRCSEL** *to* **Enable CRC 32**
- Configure **CRCSRC** in **SYSCFG0** register to **CRC OF FULL FLASH**

  **Note:** *In case of CRC-32, Configure* **CRCSEL** *to* **NOCRC** *because Hexmate cannot generate CRC-32 checksum.*
<br><img src="images/size-fuse.png" width="500">
<br><img src="images/crc-fuse.png" width="500">

#### o	Configure CRCSCAN
- Configure the **CRC Source** as **CRC on entire flash**
<br><img src="images/crcscan.png" width="500">

#### o	Configure USART
In this example, USART1 is used to transmit data to the terminal window to display the stored CRC value as well as the end result message.
- Check the **Printf support** option in the **Software settings section**
<br><img src="images/uart.png" width="500">

#### o	Configure PIT
In this example, PIT is used to generate periodic event for every 8 seconds.  The PIT interrupt is used to check CRC of the program memory. PIT is part of RTC module, thus PIT configuration options are available under RTC configuration window.
- Uncheck the **Enable RTC option**
- Select **RTC Clock Source Selection** option as **Internal 1.024 kHz oscillator**
- Check the **PIT Enable** option
- Select the **Period Selection** as **RTC Clock Cycles 8192**
- Check the **PIT interrupt Enable** option
- Uncheck **Overflow Interrupt Enable** option
<br><img src="images/rtc.png" width="500">
<br><img src="images/pit.png" width="500">

#### o	Configure the Pins used
#### In Pin Manager: Grid View,
- Set pin PC6 as output for LED indication
- Set pin PC7 as input for Switch press event
<br><img src="images/pin-grid.png" width="650">

#### In Pin Module,
- Give custom name LED0 to the pin PC6. Set pin PC6 (LED0) output default status to **HIGH**, by checking **START HIGH** option for pin PC6 in pin module section. Thereby, LED0 turn off after the pin initialization.
- Give custom name SW0 to the pin PC7. Check **PULLUPEN** option for SW0 and configure  **ISC** to **Sense Rising Edge**
<br><img src="images/pin-module.png" width="500">

#### o	Enable Global Interrupt
- Check **Global Interrupt Enable** option in **Interrupt Manager** window
<br><img src="images/interrupt-manager.png" width="500">

#### o	Generate the project file
Click Generate button next to the project Resources heading under **Tree view** section to generate code according to the configuration. Add custom code to the project present in *main.c* file.

#### 3. Add Linker Command
- Open Project Properties by right clicking **project name -> Properties**
- Add a linker command to the project under **XC8 Global Options -> XC8 Linker -> Additional options**, as shown below
<br><img src="images/linker.png" width="500">

**Command:**
`-Wl,-Ttext=0x400`

This command keeps the application code at 0x400 location in the Flash memory. It is the start of APPCODE section of Flash as discussed in the previous section. For details of other linker commands refer MPLAB XC8 C Compiler User Guide for AVR MCU.

#### 4. Add Post Build Command
- Open Project Properties by right clicking **project name -> Properties**
- Add a post-build command to the project under **Conf -> Building**, as shown below
- Also check the **Execute this line after build** option as shown below
<br><img src="images/post-build.png" width="500">

**Command:**
`"C:\Program Files (x86)\Microchip\MPLABX\v5.35\mplab_platform\bin\hexmate"  ${ImagePath} -o${ImagePath} -FILL=0xFFFF@0x0000:0x1FFFD -CK=0x0000-0x1FFFD@0x1FFFE+0xFFFFw2g5p0x1021`

This post build command takes the generated HEX file by linker, fill the remaining space of Flash with 0xFFFF (-FILL=0xFFFF@0x0000:0x1FFFD), calculate CRC, and keep the CRC checksum at the last 2 bytes of Flash. (-CK=0x0000-0x1FFFD@0x1FFFE+0xFFFFw2g5p0x1021).

CRC-16 example calculates a CRC (g5), using an initial value of 0xFFFF (+0xFFFF) and a polynomial value of 0x1021 (p0x1021) from the HEX file data at addresses 0 through to 0x1FFFD (0x0000-0x1FFFD), placing the big-endian, 2-byte-wide result (w2) at address 0x1FFFE (@0x1FFFE) in the final HEX file.

For other Hexmate options, see the Utilities Chapter in the MPLAB XC8 C Compiler User Guide.

**For CRC-32 use following command**
`"C:\Program Files (x86)\Microchip\MPLABX\v5.35\mplab_platform\bin\hexmate"  ${ImagePath} -o${ImagePath}  -FILL=0xFFFF@0x0000:0x1FFFB -CK=0x0000-0x1FFFB@0x1FFFC+0xFFFFFFFFw-4g-5p0x04C11DB7`

**Note:** *For CRC-32, XOR operation needs to be performed on the generated checksum. This option is not available with Hexmate. Thus, the final XOR operation is performed by the MCU.*
