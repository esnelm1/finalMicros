#include "msp430.h"
#include "I2Croutines.h"


#define MAXPAGEWRITE 64

int PtrTransmit;
unsigned char I2CBufferArray[66];
unsigned char I2CBuffer;

/*----------------------------------------------------------------------------*/
// Inicialización del módulo I2C adaptada para MSP430G2553
/*----------------------------------------------------------------------------*/
void InitI2C(unsigned char eeprom_i2c_address)
{
  // Configurar pines para MSP430G2553: P1.6 (SCL) y P1.7 (SDA)
  P1SEL |= SDA_PIN + SCL_PIN;
  P1SEL2 |= SDA_PIN + SCL_PIN;  // Esta línea es necesaria para seleccionar la función I2C

  // Inicialización del módulo I2C:
  UCB0CTL1 |= UCSWRST;                        // Activa reset por software
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;       // Modo Maestro I2C, síncrono
  UCB0CTL1 = UCSSEL_2 + UCTR + UCSWRST;       // Usa SMCLK, modo transmisión, mantiene reset
  UCB0BR0 = SCL_CLOCK_DIV;                    // fSCL = SMCLK/12 100kHz
  UCB0BR1 = 0;
  UCB0I2CSA  = eeprom_i2c_address;            // Dirección del esclavo (EEPROM)
  UCB0I2COA = 0x01A5;                         // Dirección propia (no esencial en modo maestro)
  UCB0CTL1 &= ~UCSWRST;                       // Sale del reset, reanuda operación

  // Si el bus está ocupado, se genera un pulso manual en SCL para liberarlo
  if (UCB0STAT & UCBBUSY)
  {
    I2C_PORT_SEL &= ~SCL_PIN;
    I2C_PORT_OUT &= ~SCL_PIN;
    I2C_PORT_DIR |= SCL_PIN;
    I2C_PORT_SEL |= SDA_PIN + SCL_PIN;
  }
}

void I2CWriteInit(void)
{
  UCB0CTL1 |= UCTR;                         // Modo transmisión
  IFG2 &= ~UCB0TXIFG;
  IE2 &= ~UCB0RXIE;                         // Deshabilita interrupción de recepción
  IE2 |= UCB0TXIE;                          // Habilita interrupción de transmisión
}

void I2CReadInit(void)
{
  UCB0CTL1 &= ~UCTR;                        // Modo recepción
  IFG2 &= ~UCB0RXIFG;
  IE2 &= ~UCB0TXIE;                         // Deshabilita interrupción de transmisión
  IE2 |= UCB0RXIE;                          // Habilita interrupción de recepción
}

void EEPROM_ByteWrite(unsigned int Address, unsigned char Data)
{
  unsigned char adr_hi;
  unsigned char adr_lo;

  while (UCB0STAT & UCBUSY);

  adr_hi = Address >> 8;
  adr_lo = Address & 0xFF;

  I2CBufferArray[2] = adr_hi;
  I2CBufferArray[1] = adr_lo;
  I2CBufferArray[0] = Data;
  PtrTransmit = 2;

  I2CWriteInit();
  UCB0CTL1 |= UCTXSTT;

  __bis_SR_register(LPM0_bits + GIE);
  UCB0CTL1 |= UCTXSTP;

  unsigned int timeout = 10000;  // Límite de tiempo para la espera del STOP
  while ((UCB0CTL1 & UCTXSTP) && --timeout);

  if (timeout == 0) {
      //ERROR: El STOP nunca se completó, reiniciar el módulo I2C
      UCB0CTL1 |= UCSWRST;  // Reset del módulo I2C
      UCB0CTL1 &= ~UCSWRST; // Salir del reset
  }

//  while(UCB0CTL1 & UCTXSTP);
}

void EEPROM_PageWrite(unsigned int StartAddress, unsigned char * Data, unsigned int Size)
{
  volatile unsigned int i = 0;
  volatile unsigned char counterI2cBuffer;
  unsigned char adr_hi;
  unsigned char adr_lo;
  unsigned int currentAddress = StartAddress;
  unsigned int currentSize = Size;
  unsigned int bufferPtr = 0;
  unsigned char moreDataToRead = 1;

  while (UCB0STAT & UCBUSY);

  while(moreDataToRead)
  {
    adr_hi = currentAddress >> 8;
    adr_lo = currentAddress & 0xFF;

    if(currentSize > MAXPAGEWRITE)
    {
      bufferPtr = bufferPtr + MAXPAGEWRITE;
      counterI2cBuffer = MAXPAGEWRITE - 1;
      PtrTransmit = MAXPAGEWRITE + 1;
      currentSize = currentSize - MAXPAGEWRITE;
      currentAddress = currentAddress + MAXPAGEWRITE;

      I2CBufferArray[MAXPAGEWRITE + 1] = adr_hi;
      I2CBufferArray[MAXPAGEWRITE] = adr_lo;
    }
    else
    {
      bufferPtr = bufferPtr + currentSize;
      counterI2cBuffer = currentSize - 1;
      PtrTransmit = currentSize + 1;
      moreDataToRead = 0;
      currentAddress = currentAddress + currentSize;

      I2CBufferArray[currentSize + 1] = adr_hi;
      I2CBufferArray[currentSize] = adr_lo;
    }

    unsigned char temp;
    for(i; i < bufferPtr; i++)
    {
      temp = Data[i];
      I2CBufferArray[counterI2cBuffer] = temp;
      counterI2cBuffer--;
    }

    I2CWriteInit();
    UCB0CTL1 |= UCTXSTT;
    __bis_SR_register(LPM0_bits + GIE);
    UCB0CTL1 |= UCTXSTP;
    while(UCB0CTL1 & UCTXSTP);

    EEPROM_AckPolling();
  }
}

unsigned char EEPROM_CurrentAddressRead(void)
{
  while(UCB0STAT & UCBUSY);
  I2CReadInit();

  UCB0CTL1 |= UCTXSTT;
  while(UCB0CTL1 & UCTXSTT);
  UCB0CTL1 |= UCTXSTP;
  __bis_SR_register(LPM0_bits + GIE);
  while(UCB0CTL1 & UCTXSTP);
  return I2CBuffer;
}

unsigned char EEPROM_RandomRead(unsigned int Address)
{
  unsigned char adr_hi;
  unsigned char adr_lo;

  while (UCB0STAT & UCBUSY);

  adr_hi = Address >> 8;
  adr_lo = Address & 0xFF;

  I2CBufferArray[1] = adr_hi;
  I2CBufferArray[0] = adr_lo;
  PtrTransmit = 1;

  I2CWriteInit();
  UCB0CTL1 |= UCTXSTT;
  __bis_SR_register(LPM0_bits + GIE);

  I2CReadInit();

  UCB0CTL1 |= UCTXSTT;
  while(UCB0CTL1 & UCTXSTT);
  UCB0CTL1 |= UCTXSTP;
  __bis_SR_register(LPM0_bits + GIE);
  while(UCB0CTL1 & UCTXSTP);
  return I2CBuffer;
}

void EEPROM_SequentialRead(unsigned int Address, unsigned char * Data, unsigned int Size)
{
  unsigned char adr_hi;
  unsigned char adr_lo;
  unsigned int counterSize;

  while (UCB0STAT & UCBUSY);

  adr_hi = Address >> 8;
  adr_lo = Address & 0xFF;

  I2CBufferArray[1] = adr_hi;
  I2CBufferArray[0] = adr_lo;
  PtrTransmit = 1;

  I2CWriteInit();
  UCB0CTL1 |= UCTXSTT;
  __bis_SR_register(LPM0_bits + GIE);

  I2CReadInit();

  UCB0CTL1 |= UCTXSTT;
  while(UCB0CTL1 & UCTXSTT);

  for(counterSize = 0 ; counterSize < Size ; counterSize++)
  {
    __bis_SR_register(LPM0_bits + GIE);
    Data[counterSize] = I2CBuffer;
  }
  UCB0CTL1 |= UCTXSTP;
  __bis_SR_register(LPM0_bits + GIE);
  while(UCB0CTL1 & UCTXSTP);
}

void EEPROM_AckPolling(void)
{
  while (UCB0STAT & UCBUSY);
  do
  {
    UCB0STAT = 0x00;
    UCB0CTL1 |= UCTR;
    UCB0CTL1 &= ~UCTXSTT;
    UCB0CTL1 |= UCTXSTT;
    while(UCB0CTL1 & UCTXSTT)
    {
      if(!(UCNACKIFG & UCB0STAT))
        break;
    }
    UCB0CTL1 |= UCTXSTP;
    //while (UCB0CTL1 & UCTXSTP);
    unsigned int timeout = 10000;
    while ((UCB0CTL1 & UCTXSTP) && --timeout);

    if (timeout == 0) {
        // ERROR: El STOP no se completó. Reiniciamos el módulo I2C.
        UCB0CTL1 |= UCSWRST;  // Poner en reset
        UCB0CTL1 &= ~UCSWRST; // Salir del reset
    }

    __delay_cycles(500);
  } while(UCNACKIFG & UCB0STAT);
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void TX_ISR_I2C(void)
{
  if(UCB0TXIFG & IFG2)
  {
    UCB0TXBUF = I2CBufferArray[PtrTransmit]; // Cargar el buffer de transmisión

    while (UCB0STAT & UCNACKIFG);


    PtrTransmit--;
    IFG2 |= UCB0TXIFG;
    if(PtrTransmit < 0)
    {
      while(!(IFG2 & UCB0TXIFG));
      IE2 &= ~UCB0TXIE;        // Deshabilitar interrupción de transmisión
      IFG2 &= ~UCB0TXIFG;      // Limpiar la bandera de transmisión
      __bic_SR_register_on_exit(LPM0_bits); // Salir del modo LPM0
    }
  }
  else if(UCB0RXIFG & IFG2)
  {
    I2CBuffer = UCB0RXBUF;      // Leer dato recibido
    __bic_SR_register_on_exit(LPM0_bits); // Salir del modo LPM0
  }
}
