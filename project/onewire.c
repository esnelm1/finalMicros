/***************************************************************************//**
 @file     template.c
 @brief    template for .c file
 @author   Conradeus
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "onewire.h"
#include "common.h"
#include "board.h"
#include "hardware.h"
#include "machine/_types.h"
#include "timer.h"
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define RESET_LOW    500/1000           // 500 us
#define RESET_DELAY  360/1000           // 360 us
#define WRITE_ONE    6/1000             // 6 us
#define DELAY_ONE    64/1000            // 64 us
#define WRITE_ZERO   60/1000            // 60 us
#define DELAY_ZERO   10/1000            // 10 us
#define READ_LOW     6/1000             // 6 us
#define READ_DETECT  9/1000             // 9 us
#define READ_DELAY   55/1000            // 55 us

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void oneWire_init(void){
    // Configuracion GPIO
    gpioMode(PIN_ONEWIRE_DATA, OUTPUT); //P1DIR |= BIT5; // P1.5 como salida
    gpioWrite(PIN_ONEWIRE_DATA, HIGH); //P1OUT = BIT5;  // P1.5 HIGH
}
void oneWire_singleWrite(__uint8_t data){

    uint8_t ibit;
    for (ibit=0; ibit < 8; ibit++) {
        gpioWrite(PIN_ONEWIRE_DATA, LOW); //P1OUT &= ~BIT5;
        if ((data >> ibit) & 0x01) {
            timerDelay(WRITE_ONE);
            gpioWrite(PIN_ONEWIRE_DATA, HIGH); //P1OUT |= BIT5;
            timerDelay(DELAY_ONE);
        } else {
            timerDelay( WRITE_ZERO);
            gpioWrite(PIN_ONEWIRE_DATA, HIGH); //P1OUT |= BIT5;
            timerDelay( DELAY_ZERO);
        }
    }

}

__uint8_t oneWire_singleRead(void){

    uint8_t data = 0;
    uint8_t ibit;
    for (ibit=0; ibit < 8; ibit++) {
        gpioWrite(PIN_ONEWIRE_DATA, LOW); //P1OUT &= ~BIT5;
        timerDelay(READ_LOW);
        gpioMode(PIN_ONEWIRE_DATA, INPUT); //P1DIR &= ~BIT5;      // P1.5 como entrada
        timerDelay(READ_DETECT);
        if (P1IN & BIT5) {
            data |= (1 << ibit);
        }
        timerDelay(READ_DELAY);
        gpioMode(PIN_ONEWIRE_DATA,OUTPUT);
        gpioWrite(PIN_ONEWIRE_DATA, HIGH);
    }
    return data;
}

void oneWire_singleReset(void){
    gpioWrite(PIN_ONEWIRE_DATA, LOW); //P1OUT &= ~BIT5;
    timerDelay(RESET_LOW);
    gpioMode(PIN_ONEWIRE_DATA, INPUT); //P1DIR &= ~BIT5;  // P1.5 input
    timerDelay(RESET_DELAY);
    gpioMode(PIN_ONEWIRE_DATA,OUTPUT);
    gpioWrite(PIN_ONEWIRE_DATA, HIGH);
}

/*******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/******************************************************************************/

