#ifndef I2CROUTINES_H
#define I2CROUTINES_H

/******************************************************************************/
/*  Communication with an EEPROM (e.g. 2465) via I2C bus                      */
/*  The I2C module of the MSP430G2553 is used to communicate with the EEPROM. */
/*  The "Byte Write", "Page Write", "Current Address Read", "Random Read",    */
/*  "Sequential Read" and "Acknowledge Polling" commands of the EEPROM are     */
/*  realized.                                                                 */
/*                                                                            */
/*  developed with IAR Embedded Workbench V5.20.1                             */
/*                                                                            */
/*  Texas Instruments                                                         */
/*  William Goh                                                               */
/*  March 2011                                                                */
/*                                                                            */
/*  Adapted for MSP430G2553 (Code Composer Studio)                            */
/******************************************************************************/

#define I2C_PORT_SEL  P1SEL
#define I2C_PORT_OUT  P1OUT
#define I2C_PORT_REN  P1REN
#define I2C_PORT_DIR  P1DIR
#define SDA_PIN       BIT7                  // USCI_B0SDA pin on P1.7
#define SCL_PIN       BIT6                  // USCI_B0SCL pin on P1.6
#define SCL_CLOCK_DIV 0x12                  // SCL clock divider

void InitI2C(unsigned char eeprom_i2c_address);
void EEPROM_ByteWrite(unsigned int Address , unsigned char Data);
void EEPROM_PageWrite(unsigned int StartAddress , unsigned char * Data , unsigned int Size);
unsigned char EEPROM_RandomRead(unsigned int Address);
unsigned char EEPROM_CurrentAddressRead(void);
void EEPROM_SequentialRead(unsigned int Address , unsigned char * Data , unsigned int Size);
void EEPROM_AckPolling(void);

#endif // I2CROUTINES_H
