/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   Conradeus
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// On Board User LEDs

#define LED_ACTIVE          HIGH


// On Board User Switches

#define SWITCH_INPUT        INPUT_PULLUP
#define SWITCH_ACTIVE       LOW

// UART pins(hardware USCI_A)
#define PIN_UART_RX   (1<<1)//DO NOT SET WITH PORTNUM2PIN(1,1)
#define PIN_UART_TX   (1<<2)//DO NOT SET WITH PORTNUM2PIN(1,2)

// I2C pins(hardware USCI_B)
#define PIN_I2C_SCL   (1<<6) //1.6 //DO NOT SET WITH PORTNUM2PIN
#define PIN_I2C_SDA   (1<<7) //1.7 //DO NOT SET WITH PORTNUM2PIN

// SPI pins (bit banging with gpio)
#define PIN_SPI_CLK   PORTNUM2PIN(2,0)// 2.0
#define PIN_SPI_MOSI   PORTNUM2PIN(2,1) //2.1

// Led Status pin
#define LED_STATUS PORTNUM2PIN(2,3) //2.3


// PWM PIN
#define PIN_PWM   (1<<4) //2.4

// onewire pins (bit banging with gpio)
#define PIN_ONEWIRE_DATA    PORTNUM2PIN(1,5)//1.5

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
