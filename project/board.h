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
#define PIN_UART_RX   (1<<1) //DO NOT SET WITH PORTNUM2PIN(1,1)
#define PIN_UART_TX   (1<<2) //DO NOT SET WITH PORTNUM2PIN(1,2)

// I2C pins(hardware USCI_B)
#define SCL_PIN   (1<<6) // USCI_B0SCL pin on P1.6
#define SDA_PIN   (1<<7) // USCI_B0SCL pin on P1.7

// SPI pins (bit banging with gpio)
#define PIN_SPI_CLK       (1 << 0) // 2.0
#define PIN_SPI_MOSI      (1 << 1) //2.1
#define PIN_LATCH_ENABLE  (1 << 2) //2.2

// Led Status pin
#define LED_STATUS PORTNUM2PIN(2,3) //2.3

// Pin Green LED Timer
//#define PIN_LED_GREEN PORTNUM2PIN(2,2)  // Lo DEFINI RECIEN

// PWM PIN
#define PIN_PWM   (1 << 5) //1.5

// onewire pins (bit banging with gpio)
#define PIN_ONEWIRE_DATA    PORTNUM2PIN(1,4)//1.4

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
