#include <msp430.h> 

#define I2C_PORT_SEL  P1SEL
#define I2C_PORT_OUT  P1OUT
#define I2C_PORT_REN  P1REN
#define I2C_PORT_DIR  P1DIR
#define SDA_PIN       BIT7                  // USCI_B0SDA pin on P1.7
#define SCL_PIN       BIT6                  // USCI_B0SCL pin on P1.6
#define SCL_CLOCK_DIV 0x12                  // SCL clock divider
#define SlaveAddress  0x50

int PtrTransmit;
unsigned char I2CBufferArray[66];
unsigned char I2CBuffer;

int main(void)
{

      P1SEL |= SDA_PIN + SCL_PIN;        // Assign I2C pins to USCI_B0
      P1SEL2 |= SDA_PIN + SCL_PIN;

      WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

      // Recommended initialization steps of I2C module as shown in User Guide:
      UCB0CTL1 |= UCSWRST;                      // Enable SW reset
      UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;       // I2C Master, synchronous mode
      UCB0CTL1 = UCSSEL_2 | UCTR | UCSWRST;       // Use SMCLK, TX mode, keep SW reset
      UCB0BR0 = SCL_CLOCK_DIV;                   // fSCL = SMCLK/12 = ~100kHz
      UCB0BR1 = 0;
      UCB0I2CSA  = SlaveAddress;                 // Define Slave Address
      UCB0I2COA = 0x01A5;                        // Own address.
      UCB0CTL1 &= ~UCSWRST;                      // Clear SW reset, resume operation


      // Escribo

      unsigned int Address = 0x00;
      unsigned char Data = 1;
      unsigned char adr_hi;
      unsigned char adr_lo;

    while(1){

      while (UCB0STAT & UCBUSY);                // Wait until I2C module has finished all operations.

      adr_hi = Address >> 8;                    // Calculate high byte
      adr_lo = Address & 0xFF;                  // and low byte of address

      I2CBufferArray[2] = adr_hi;               // Store high byte address.
      I2CBufferArray[1] = adr_lo;               // Store low byte address.
      I2CBufferArray[0] = Data;
      PtrTransmit = 2;                          // Set I2CBufferArray Pointer

      UCB0CTL1 |= UCTR;                         // UCTR=1 => Transmit Mode (R/W bit = 0)
      UCB0CTL1 |= UCTXSTT;                      // Start condition generation => I2C communication is started


         while(PtrTransmit >= 0)
         {
             while (UCB0STAT & UCBUSY); // Espera hasta que el módulo termine sus operaciones
             UCB0TXBUF = I2CBufferArray[PtrTransmit]; // Load TX buffer
             PtrTransmit--;                          // Decrement TX byte counter
             while (UCB0STAT & UCNACKIFG);

         }

      UCB0CTL1 |= UCTXSTP;                      // I2C stop condition
      while(UCB0CTL1 & UCTXSTP);                // Ensure stop condition got sent


      // ACK Polling

      while (UCB0STAT & UCBUSY);                // Wait until I2C module has finished all operations
      do
      {
        UCB0STAT = 0x00;                        // Clear I2C interrupt flags
        UCB0CTL1 |= UCTR;                       // UCTR=1 => Transmit Mode (R/W bit = 0)
        UCB0CTL1 &= ~UCTXSTT;
        UCB0CTL1 |= UCTXSTT;                    // Start condition is generated
        while(UCB0CTL1 & UCTXSTT)               // Wait until I2CSTT bit is cleared
        {
          if(!(UCNACKIFG & UCB0STAT))           // Break out if ACK received
            break;
        }
        UCB0CTL1 |= UCTXSTP;                    // Stop condition is generated after slave address was sent
        while (UCB0CTL1 & UCTXSTP);             // Wait until stop bit is reset
        __delay_cycles(40000);                    // Software delay
      }while(UCNACKIFG & UCB0STAT);



      // Lectura


      while (UCB0STAT & UCBUSY);                // Wait until I2C module has finished all operations

/*
      adr_hi = Address >> 8;                    // Calculate high byte
      adr_lo = Address & 0xFF;                  // and low byte of address
*/

      I2CBufferArray[1] = adr_hi;               // Store high byte address.
      I2CBufferArray[0] = adr_lo;               // Store low byte address.
      PtrTransmit = 1;                          // Set I2CBufferArray Pointer

      // Write Address first
      UCB0CTL1 |= UCTR;                         // UCTR=1 => Transmit Mode (R/W bit = 0)
      UCB0CTL1 |= UCTXSTT;                      // Start condition generation => I2C communication is started
      while(PtrTransmit >= 0)
         {
             while (UCB0STAT & UCBUSY); // Espera hasta que el módulo termine sus operaciones
             UCB0TXBUF = I2CBufferArray[PtrTransmit]; // Load TX buffer
             PtrTransmit--;                          // Decrement TX byte counter
             while (UCB0STAT & UCNACKIFG);
         }
      UCB0CTL1 |= UCTXSTP;                      // I2C stop condition
      while (UCB0CTL1 & UCTXSTP);             // Wait until stop bit is reset


      // Read Data byte
//      UCB0CTL1 &= ~UCTR;                        // UCTR=0 => Receive Mode (R/W bit = 1)
//
//      UCB0CTL1 |= UCTXSTT; // Inicio de la lectura
//      while (!(IFG2 & UCB0RXIFG));  // Espera a que se reciba el dato
//      I2CBuffer = UCB0RXBUF;        // Lee el dato recibido
//
//
//      UCB0CTL1 |= UCTXSTP;                      // I2C stop condition
//
//      while(UCB0CTL1 & UCTXSTP);                // Ensure stop condition got sent

      // Configuración para la recepción de un solo byte:
      UCB0CTL1 &= ~UCTR;            // Modo recepción (R/W = 1)
      UCB0CTL1 |= UCTXSTT;          // Genera condición de inicio
      while (UCB0CTL1 & UCTXSTT);    // Espera a que se envíe la dirección
      UCB0CTL1 |= UCTXSTP;          // Genera condición de parada para un solo byte
      while (UCB0CTL1 & UCTXSTP);             // Wait until stop bit is reset
      while (!(IFG2 & UCB0RXIFG));   // Espera a que se reciba el dato
      I2CBuffer = UCB0RXBUF;        // Lee el dato



    };
}
