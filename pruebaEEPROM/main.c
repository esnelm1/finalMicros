/*******************************************************************************
// main.c - Adaptado para MSP430G2553
// Comunicación con una EEPROM vía I2C utilizando el MSP430G2553.
// Se utilizan las rutinas adaptadas en I2Croutines.c e I2Croutines.h
//
// W. Goh (adaptado)
// Texas Instruments, 2011 (adaptación MSP430G2553)
//*******************************************************************************/

#include "msp430.h"
#include "I2Croutines.h"

#define SlaveAddress   0x50

#define TEST_ADDRESS         0x0000 // Dato de prueba
#define TEST_DATA            0x55

unsigned char read_val[150];
unsigned char write_val[150];
unsigned int address;

int main(void)
{
  unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Detener el watchdog timer

  InitI2C(SlaveAddress);                    // Inicializar el módulo I2C

  EEPROM_ByteWrite(0x0000,0x12);
  EEPROM_AckPolling();                      // Espera final del ciclo de escritura
  EEPROM_ByteWrite(0x0001,0x34);
  EEPROM_AckPolling();
  EEPROM_ByteWrite(0x0002,0x56);
  EEPROM_AckPolling();
  EEPROM_ByteWrite(0x0003,0x78);
  EEPROM_AckPolling();
  EEPROM_ByteWrite(0x0004,0x9A);
  EEPROM_AckPolling();
  EEPROM_ByteWrite(0x0005,0xBC);
  EEPROM_AckPolling();

  read_val[0] = EEPROM_RandomRead(0x0000);
  read_val[1] = EEPROM_CurrentAddressRead();
  read_val[2] = EEPROM_CurrentAddressRead();
  read_val[3] = EEPROM_CurrentAddressRead();
  read_val[4] = EEPROM_CurrentAddressRead();
  read_val[5] = EEPROM_CurrentAddressRead();

  // Llenar el arreglo write_val con valores crecientes
  for(i = 0 ; i < sizeof(write_val); i++)
  {
    write_val[i] = i;
  }

  address = 0x0000;                         // Dirección de inicio
  EEPROM_PageWrite(address, write_val, sizeof(write_val));
  EEPROM_SequentialRead(address, read_val, sizeof(read_val));

  __bis_SR_register(LPM4_bits);             // Entrar en LPM4 (bajo consumo)
  __no_operation();
}
