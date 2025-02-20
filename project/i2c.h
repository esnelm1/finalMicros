/*
  @file     i2c.h
  @brief    i2c for .h file
  @author   Esneleus
 ******************************************************************************/

#ifndef I2C_H
#define I2C_H

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

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

 void I2cInit(void);
 void eprom_write_parameters(unsigned char setPoint, unsigned char histeresis, unsigned int intervaloMuestreo);
 void eprom_read_parameters(unsigned char *setPoint, unsigned char *histeresis, unsigned int *intervaloMuestreo);
/*******************************************************************************
 ******************************************************************************/

#endif // HARDWARE_H
