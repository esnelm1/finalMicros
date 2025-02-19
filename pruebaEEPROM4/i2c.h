/*
  @file     i2c.h
  @brief    i2c for .h file
  @author   Esneleus
 ******************************************************************************/

#ifndef I2C_H
#define I2C_H

#define SDA_PIN       BIT7                  // USCI_B0SDA pin on P1.7
#define SCL_PIN       BIT6                  // USCI_B0SCL pin on P1.6
#define SCL_CLOCK_DIV 0x12                  // SCL clock divider
#define SlaveAddress  0x50

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

 int PtrTransmit;
 unsigned char I2CBufferArray[66];
 unsigned char I2CBuffer;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

 void I2cInit(void);
 void AckPolling(void);
 void EpromByteWrite(unsigned int Address, unsigned char Data);
 unsigned char EepromRandomRead(unsigned int Address);



/*******************************************************************************
 ******************************************************************************/

#endif // HARDWARE_H
