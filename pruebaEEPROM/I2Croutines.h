/******************************************************************************/
/*  Adaptado para MSP430G2553                                                 */
/******************************************************************************/

#ifndef I2C_ROUTINES_H
#define I2C_ROUTINES_H

// Configuración de pines para el MSP430G2553:
// Los pines de I2C se encuentran en el puerto 1: P1.6 (SCL) y P1.7 (SDA)
#define I2C_PORT_SEL  P1SEL
#define I2C_PORT_OUT  P1OUT
#define I2C_PORT_REN  P1REN
#define I2C_PORT_DIR  P1DIR
#define SDA_PIN       BIT7   // SDA en P1.7
#define SCL_PIN       BIT6   // SCL en P1.6
#define SCL_CLOCK_DIV 0x12   // Divisor del reloj SCL (ajustable según se requiera)

// Prototipos de funciones
void InitI2C(unsigned char eeprom_i2c_address);
void EEPROM_ByteWrite(unsigned int Address , unsigned char Data);
void EEPROM_PageWrite(unsigned int StartAddress , unsigned char * Data , unsigned int Size);
unsigned char EEPROM_RandomRead(unsigned int Address);
unsigned char EEPROM_CurrentAddressRead(void);
void EEPROM_SequentialRead(unsigned int Address , unsigned char * Data , unsigned int Size);
void EEPROM_AckPolling(void);

#endif
