/*
 @file     i2c.c
 @brief    i2c for .c file
 @author   Esneleus
 ******************************************************************************/


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "i2c.h"
#include <msp430.h>

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

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
 static unsigned char adr_hi;
 static unsigned char adr_lo;
 static int PtrTransmit;
 static unsigned char I2CBufferArray[66];
 static unsigned char I2CBuffer;

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

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

 void AckPolling(void)
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

 void EpromByteWrite(unsigned int Address, unsigned char Data)
 {

     while (UCB0STAT & UCBUSY)
         ;                // Wait until I2C module has finished all operations.

     adr_hi = Address >> 8;                    // Calculate high byte
     adr_lo = Address & 0xFF;                  // and low byte of address

     I2CBufferArray[2] = adr_hi;               // Store high byte address.
     I2CBufferArray[1] = adr_lo;               // Store low byte address.
     I2CBufferArray[0] = Data;
     PtrTransmit = 2;                          // Set I2CBufferArray Pointer

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

 unsigned char EepromRandomRead(unsigned int Address)
 {

     while (UCB0STAT & UCBUSY)
         ;                // Wait until I2C module has finished all operations

     adr_hi = Address >> 8;                    // Calculate high byte
     adr_lo = Address & 0xFF;                  // and low byte of address

     I2CBufferArray[1] = adr_hi;               // Store high byte address.
     I2CBufferArray[0] = adr_lo;               // Store low byte address.
     PtrTransmit = 1;                          // Set I2CBufferArray Pointer

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



//
