/*
 @file     i2c.c
 @brief    i2c for .c file
 @author   Esneleus
 ******************************************************************************/


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "i2c.h"
#include "board.h"
#include "hardware.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
 static void I2cWriteInit(void);
 static void I2cReadInit(void);
 static void AckPolling(void);
 static void EpromByteWrite(unsigned int Address, unsigned char Data);
 static unsigned char EepromRandomRead(unsigned int Address);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
 static unsigned char adr_hi;
 static unsigned char adr_lo;
 static unsigned char I2CBuffer;

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


 // Función para escribir los parámetros en la EEPROM
 void eprom_write_parameters(unsigned char setPoint, unsigned char histeresis, unsigned int intervaloMuestreo)
 {
     // Escribe setPoint en la dirección 0x0000
     EpromByteWrite(0x0000, setPoint);
     AckPolling();

     // Escribe histeresis en la dirección 0x0001
     EpromByteWrite(0x0001, histeresis);
     AckPolling();

     // Escribe intervaloMuestreo en dos bytes:
     // LSB en la dirección 0x0002
     EpromByteWrite(0x0002, (unsigned char)(intervaloMuestreo & 0xFF));  // LSB
     AckPolling();
     // y MSB en la dirección 0x0003
     EpromByteWrite(0x0003, (unsigned char)((intervaloMuestreo >> 8) & 0xFF));  // MSB
     AckPolling();
 }

 // Función para leer los parámetros de la EEPROM
 void eprom_read_parameters(unsigned char *setPoint, unsigned char *histeresis, unsigned int *intervaloMuestreo)
 {
     // Lee setPoint de la dirección 0x0000
     *setPoint = EepromRandomRead(0x0000);

     // Lee histeresis de la dirección 0x0001
     *histeresis = EepromRandomRead(0x0001);

     // Lee intervaloMuestreo desde dos direcciones:
     // LSB de la dirección 0x0002 y MSB de la dirección 0x0003
     unsigned char lsb = EepromRandomRead(0x0002);
     unsigned char msb = EepromRandomRead(0x0003);
     *intervaloMuestreo = (unsigned int)lsb | ((unsigned int)msb << 8);
 }

 void I2cInit(void)
 {
     P1SEL |= SDA_PIN + SCL_PIN;        // Assign I2C pins to USCI_B0
     P1SEL2 |= SDA_PIN + SCL_PIN;

     WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

     // Recommended initialization steps of I2C module as shown in User Guide:
     UCB0CTL1 |= UCSWRST;                      // Enable SW reset
     UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;       // I2C Master, synchronous mode
     UCB0CTL1 = UCSSEL_2 | UCTR | UCSWRST;   // Use SMCLK, TX mode, keep SW reset
     UCB0BR0 = 10;                   // fSCL = SMCLK/12 = ~100kHz
     UCB0BR1 = 0;
     UCB0I2CSA = SlaveAddress;                 // Define Slave Address
     UCB0I2COA = 0x01A5;                        // Own address.
     UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
 }




/*******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


 static void I2cWriteInit(void)
 {
     UCB0CTL1 |= UCTR;                   // UCTR=1 => Transmit Mode (R/W bit = 0)
     IFG2 &= ~UCB0TXIFG;

 }


 static void I2cReadInit(void)
 {
     UCB0CTL1 &= ~UCTR;                       // Modo recepción (R/W = 1)
     IFG2 &= ~UCB0RXIFG;

 }

 static void AckPolling(void)
  {
      while (UCB0STAT & UCBUSY)
          ;                // Wait until I2C module has finished all operations
      do
      {
          UCB0STAT = 0x00;                        // Clear I2C interrupt flags
          UCB0CTL1 |= UCTR;               // UCTR=1 => Transmit Mode (R/W bit = 0)
          UCB0CTL1 &= ~UCTXSTT;
          UCB0CTL1 |= UCTXSTT;                    // Start condition is generated
          while (UCB0CTL1 & UCTXSTT)           // Wait until I2CSTT bit is cleared
          {
              if (!(UCNACKIFG & UCB0STAT))           // Break out if ACK received
                  break;
          }
          UCB0CTL1 |= UCTXSTP; // Stop condition is generated after slave address was sent
          while (UCB0CTL1 & UCTXSTP)
              ;             // Wait until stop bit is reset
          __delay_cycles(40000);                    // Software delay
      }
      while (UCNACKIFG & UCB0STAT);
  }

  static void EpromByteWrite(unsigned int Address, unsigned char Data)
  {

      while (UCB0STAT & UCBUSY)
          ;                // Wait until I2C module has finished all operations.

      adr_hi = Address >> 8;                    // Calculate high byte
      adr_lo = Address & 0xFF;                  // and low byte of address

      I2cWriteInit();                           // write init

      UCB0CTL1 |= UCTXSTT; // Start condition generation => I2C communication is started
      while (UCB0STAT & UCBUSY)
          ; // Espera hasta que el módulo termine sus operaciones
      while (!(IFG2 & UCB0TXIFG))
          ;
      UCB0TXBUF = adr_hi; // Load TX buffer
      while (UCB0STAT & UCNACKIFG)
          ;

      while (UCB0STAT & UCBUSY)
          ; // Espera hasta que el módulo termine sus operaciones
      while (!(IFG2 & UCB0TXIFG))
          ;
      UCB0TXBUF = adr_lo; // Load TX buffer
      while (UCB0STAT & UCNACKIFG)
          ;

      while (UCB0STAT & UCBUSY)
          ; // Espera hasta que el módulo termine sus operaciones
      while (!(IFG2 & UCB0TXIFG))
          ;
      UCB0TXBUF = Data; // Load TX buffer
      while (UCB0STAT & UCNACKIFG)
          ;

      while (!(IFG2 & UCB0TXIFG))
          ;
      IFG2 &= ~UCB0TXIFG;                   // Clear USCI_B0 TX int flag
      UCB0CTL1 |= UCTXSTP;                      // I2C stop condition
      while (UCB0CTL1 & UCTXSTP)
          ;                // Ensure stop condition got sent

  }

 static unsigned char EepromRandomRead(unsigned int Address)
  {

      while (UCB0STAT & UCBUSY)
          ;                // Wait until I2C module has finished all operations

      adr_hi = Address >> 8;                    // Calculate high byte
      adr_lo = Address & 0xFF;                  // and low byte of address

      // Write Address first

      I2cWriteInit();
      UCB0CTL1 |= UCTXSTT; // Start condition generation => I2C communication is started
      while (UCB0STAT & UCBUSY)
          ; // Espera hasta que el módulo termine sus operaciones
      while (!(IFG2 & UCB0TXIFG))
          ;
      UCB0TXBUF = adr_hi; // Load TX buffer
      while (UCB0STAT & UCNACKIFG)
          ;

      while (UCB0STAT & UCBUSY)
          ; // Espera hasta que el módulo termine sus operaciones
      while (!(IFG2 & UCB0TXIFG))
          ;
      UCB0TXBUF = adr_lo; // Load TX buffer
      while (UCB0STAT & UCNACKIFG)
          ;

      while (!(IFG2 & UCB0TXIFG))
          ;
      IFG2 &= ~UCB0TXIFG;                   // Clear USCI_B0 TX int flag

      // Read Data byte

      I2cReadInit();

      UCB0CTL1 |= UCTXSTT;          // Genera condición de inicio
      while (UCB0CTL1 & UCTXSTT)
          ;
      while (UCB0STAT & UCNACKIFG)
          ;
      UCB0CTL1 |= UCTXSTP;          // Emite inmediatamente la condición de parada
      while (!(IFG2 & UCB0RXIFG))
          ;   // Espera a que se reciba el dato
      I2CBuffer = UCB0RXBUF;        // Lee el dato

      return I2CBuffer;

  }

//
