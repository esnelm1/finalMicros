/***************************************************************************//**
  @file     system.c
  @brief    MCU system driver
  @author   Conradeus
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "system.h"
#include "board.h"
#include "hardware.h"
#include "timer.h"
#include "gpio.h"
#include "pwm.h"
#include "UART.h"
#include "onewire.h"
#include "i2c.h"
#include "spi.h"



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define STEP_DELAY_MS 4000
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void step_on(void);
/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void systemInitFirst(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

//     Configure DCO
    DCOCTL = 0;
#if DCOCLK_FREQ_HZ == 1000000UL
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
#elif DCOCLK_FREQ_HZ == 8000000UL
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;
#elif DCOCLK_FREQ_HZ == 12000000UL
    BCSCTL1 = CALBC1_12MHZ;
    DCOCTL = CALDCO_12MHZ;
#elif DCOCLK_FREQ_HZ == 16000000UL
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
#else
#error Mal definido el clock del sistema
#endif

    // Configure MCLK and SMCLK
    BCSCTL2 = SELM_0 | (MCLK_PRESCALER_2POW<<4) | (SMCLK_PRESCALER_2POW<<1);
}


void boardInit(void)
{
    uart_init();
    LED_conectionStatus_init();
    //oneWire_init();
    //i2c_init();
    //spi_init();
    //pwm_init();
 
}

void timer_set_init(void){
    timerInit();
    uint8_t step_id = timerGetId();
    uint8_t step_mode = TIM_MODE_SINGLESHOT;
    timerStart(step_id, TIMER_MS2TICKS(STEP_DELAY_MS), step_mode, &step_on);
}


void systemInitLast(void)
{
    //WDTCTL = WDT_MDLY_0_5; // Watchdog timer interval mode; intervalo seleccionado, 0.5ms entre interrupciones del watchdog
    //IE1 |= WDTIE; // Habilitar el bit WDTIE, habilitar interrupciones del watchdog
    enable_interrupts(); // Habilitar interrupciones generales
}

void LED_conectionStatus_init(void)
{
    gpioMode(LED_STATUS,OUTPUT);
    gpioWrite(LED_STATUS,LOW);
}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void step_on(void){
    gpioWrite(PIN_LED_GREEN, LED_ACTIVE);
}

/******************************************************************************/

