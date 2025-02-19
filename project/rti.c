/***************************************************************************//**
  @file     rti.c
  @brief    Real Time Interrupt (RTI) driver
  @author   Conradeus
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "rti.h"

#include "hardware.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// SysTic configuration
#if SMCLK_FREQ_HZ == 1000000UL
#if RTI_PERIOD_MS == 32
#define WDTISX              0
#elif RTI_PERIOD_MS == 8
#define WDTISX              1
#else
#error Valor de RTI_PERIOD_MS no valido!
#endif // RTI_PERIOD_MS == xx
#elif SMCLK_FREQ_HZ == 8000000UL
#if RTI_PERIOD_MS == 4
#define WDTISX              0
#elif RTI_PERIOD_MS == 1
#define WDTISX              1
#else
#error Valor de RTI_PERIOD_MS no valido!
#endif // RTI_PERIOD_MS == xx
#elif SMCLK_FREQ_HZ == 16000000UL
#if RTI_PERIOD_MS == 2
#define WDTISX              0
#else
#error Valor de RTI_PERIOD_MS no valido!
#endif // RTI_PERIOD_MS == xx
#else
#error Valor de RTI_PERIOD_MS no valido!
#endif // SMCLK_FREQ_HZ == xx

#define WDTCTL_INIT         (WDTPW|WDTTMSEL|WDTCNTCL|WDTISX)


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
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

rti_callback_t rtiCallback = NULL;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

char rtiInit (rti_callback_t fun)
{
	static char yaInit = 0;
	if (yaInit)
        return false;
    
    if (fun != NULL)
    {
        rtiCallback = fun; // Register callback
        
        WDTCTL= WDTCTL_INIT; // Interval timer mode ; according interval selection
        IE1 |=  WDTIE; // Enable the WDTIE bit

       	yaInit = 1;
    }
    
    return true;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#pragma vector = WDT_VECTOR             //Interval timer vector location
__interrupt void rti_isr(void)
{
    if (rtiCallback != NULL)
        rtiCallback();
}


/******************************************************************************/

