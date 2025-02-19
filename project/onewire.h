/***************************************************************************//**
  @file     onewire.h
  @brief    onewire header for msp430g2553
  @author   Conradeus
 ******************************************************************************/

#ifndef _ONEWIRE_H_
#define _ONEWIRE_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "common.h"
#include "machine/_types.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define READ_ROM            0x33  // Leer ROM
#define MATCH_ROM           0x55  // Coincidir con ROM
#define SKIP_ROM            0xCC  // Omitir ROM

#define WRITE_MEM           0x0F  // Escribir memoria
#define VERIFY_MEM          0xAA  // Verificar memoria
#define COPY_MEM            0x55  // Copiar memoria
#define READ_MEM            0xF0  // Leer memoria

#define CONVERT_T           0x44  // Comando para que el sensor convierta la temperatura a un valor digital
#define READ_SCRATCHPAD     0xBE  // Leer la temperatura digital



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void oneWire_init(void);
void oneWire_singleWrite(__uint8_t data);
void oneWire_singleReset(void);
__uint8_t oneWire_singleRead(void);



/*******************************************************************************
 ******************************************************************************/

#endif // _ONEWIRE_H_
