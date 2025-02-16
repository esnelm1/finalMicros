/*******************************************************************************
  Description:    Communication with an EEPROM (e.g. 24128) via I2C bus
      The I2C module of the MSP430F2274 is used to communicate with the EEPROM.
      The "Byte Write", "Page Write", "Current Address Read", "Random Read",
      "Sequential Read" and "Acknowledge Polling" commands or the EEPROM are
      realized.

********************************************************************************
    updates
      Jan 2005:
          - updated initialization sequence
      March 2009:
          - updated code for 2xx USCI module
          - added Page Write and Sequential Read functions
      March 2011:
          - Fixed Random and Sequential Reads to Restart
          - Added Page Write to support greater than 256 bytes
********************************************************************************

  W. Goh
  Texas Instruments Inc.
  March 2011
  IAR Embedded Workbench Version: 5.20.1

********************************************************************************

  Copyright 2007-2009 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user who
  downloaded the software, his/her employer (which must be your employer) and
  Texas Instruments Incorporated (the "License"). You may not use this Software
  unless you agree to abide by the terms of the License. The License limits your
  use, and you acknowledge, that the Software may not be modified, copied or
  distributed unless embedded on a Texas Instruments microcontroller or used
  solely and exclusively in conjunction with a Texas Instruments radio frequency
  transceiver, which is integrated into your product. Other than for the
  foregoing purpose, you may not use, reproduce, copy, prepare derivative works
  of, modify, distribute, perform, display or sell this Software and/or its
  documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL TEXAS
  INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL
  EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT
  LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL
  DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS,
  TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT
  LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
*******************************************************************************/

#include "msp430.h"
#include "I2Croutines.h"

#define     SlaveAddress   0x50

unsigned char read_val[150];
unsigned char write_val[150];
unsigned int address;

int main(void)
{
  unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  InitI2C(SlaveAddress);                    // Initialize I2C module

  EEPROM_ByteWrite(0x0000,0x12);
  EEPROM_AckPolling();                      // Wait for EEPROM write cycle
                                            // completion
  EEPROM_ByteWrite(0x0001,0x34);
  EEPROM_AckPolling();                      // Wait for EEPROM write cycle
                                            // completion
  EEPROM_ByteWrite(0x0002,0x56);
  EEPROM_AckPolling();                      // Wait for EEPROM write cycle
                                            // completion
  EEPROM_ByteWrite(0x0003,0x78);
  EEPROM_AckPolling();                      // Wait for EEPROM write cycle
                                            // completion
  EEPROM_ByteWrite(0x0004,0x9A);
  EEPROM_AckPolling();                      // Wait for EEPROM write cycle
                                            // completion
  EEPROM_ByteWrite(0x0005,0xBC);
  EEPROM_AckPolling();                      // Wait for EEPROM write cycle
                                            // completion

  read_val[0] = EEPROM_RandomRead(0x0000);  // Read from address 0x0000
  read_val[1] = EEPROM_CurrentAddressRead();// Read from address 0x0001
  read_val[2] = EEPROM_CurrentAddressRead();// Read from address 0x0002
  read_val[3] = EEPROM_CurrentAddressRead();// Read from address 0x0003
  read_val[4] = EEPROM_CurrentAddressRead();// Read from address 0x0004
  read_val[5] = EEPROM_CurrentAddressRead();// Read from address 0x0005

  // Fill write_val array with counter values
  for(i = 0 ; i <= sizeof(write_val) ; i++)
  {
    write_val[i] = i;
  }

  address = 0x0000;                         // Set starting address at 0
  // Write a sequence of data array
  EEPROM_PageWrite(address , write_val , sizeof(write_val));
  // Read out a sequence of data from EEPROM
  EEPROM_SequentialRead(address, read_val , sizeof(read_val));

  __bis_SR_register(LPM4);
  __no_operation();
}