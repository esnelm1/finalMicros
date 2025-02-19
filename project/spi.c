/***************************************************************************//**
 @file     spi.c
 @brief    spi for .c file
 @author   Conradeus
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "spi.h"
#include "hardware.h"
#include "board.h"
#include "gpio.h"
#include "timer.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define DELAY_REFRESH_LED 50 //MS

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void spi_write_temp(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
unsigned char nivel_temp_led;

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void spi_init(void){

    P2DIR |= PIN_SPI_MOSI + PIN_SPI_CLK + PIN_LATCH_ENABLE; // Configurar MOSI y CLK como salida
    //P2DIR &= ~PIN_SPI_MISO; // not used
    P2OUT &= ~(PIN_SPI_MOSI + PIN_SPI_CLK + PIN_LATCH_ENABLE); // Inicializar líneas en bajo

    timerInit(); //timer initialize
    uint8_t spi_timer_id = timerGetId();
    uint8_t mode = TIM_MODE_PERIODIC;
    timerStart(spi_timer_id, TIMER_MS2TICKS(DELAY_REFRESH_LED), mode, &spi_write_temp);
}

void get_nivel(unsigned char data) {
    nivel_temp_led = data;
}

//void spi_write(unsigned char data) {
//    nivel_temp_led = data;
//    int i;
//    P2OUT &= ~PIN_LATCH_ENABLE;
//    for (i = 0; i < 8; i++) {
//        P2OUT &= ~PIN_SPI_CLK;
//        if (nivel_temp_led & 0x80) {  // Enviar bit mas significativo
//            P2OUT |= PIN_SPI_MOSI;
//        } else {
//            P2OUT &= ~PIN_SPI_MOSI;
//        }
//
//        P2OUT |= PIN_SPI_CLK;  // Generar pulso de reloj
//
//        nivel_temp_led <<= 1;          // Desplazar siguiente bit
//    }
//    P2OUT |= PIN_LATCH_ENABLE; // habilito el latch para que se prendan los leds
//}

/*******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void spi_write_temp(void){
    P2OUT &= ~PIN_LATCH_ENABLE;
    int i;
    for (i=0; i < 8; i++) {
        P2OUT &= ~PIN_SPI_CLK;
        if (nivel_temp_led & 0x80) {  // Enviar bit más significativo
            P2OUT |= PIN_SPI_MOSI;
        } else {
            P2OUT &= ~PIN_SPI_MOSI;
        }
        P2OUT |= PIN_SPI_CLK;  // Generar pulso de reloj

        nivel_temp_led <<= 1;          // Desplazar siguiente bit
    }
    P2OUT |= PIN_LATCH_ENABLE; // habilito el latch para que se prendan los leds
}




/******************************************************************************/

