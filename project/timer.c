/***************************************************************************//**
  @file     timer.c
  @brief    Timer driver. Advance implementation
  @author   Conradeus
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "timer.h"

#include "rti.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#if TIMER_TICK_MS != RTI_PERIOD_MS
#error Las frecuencias no coinciden!!
#endif // TIMER_TICK_MS != RTI_PERIOD_MS

#define TIMER_DEVELOPMENT_MODE    1

#define TIMER_ID_INTERNAL   0


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {
    uint16_t            period;
    uint16_t            cnt;
    timer_callback_t    callback;
    uint8_t             mode        : 1;
    uint8_t             running     : 1;
    uint8_t             expired     : 1;
    uint8_t             unused      : 5;
} timer_t;


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief Periodic service
 */
static void timer_isr(void);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static timer_t timers[TIMERS_MAX_CANT];
static uint8_t timers_cant = TIMER_ID_INTERNAL+1;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void timerInit(void)
{
    static char yaInit = 0;
    if (yaInit)
        return;
    
    rtiInit(timer_isr); // init peripheral
    
    yaInit = 1;
}


uint8_t timerGetId(void)
{
#ifdef TIMER_DEVELOPMENT_MODE
    if (timers_cant >= TIMERS_MAX_CANT)
    {
        return TIMER_INVALID_ID;
    }
    else
#endif // TIMER_DEVELOPMENT_MODE
    {
        return timers_cant++;
    }
}


void timerStart(uint8_t id, ticks_t ticks, uint8_t mode, timer_callback_t callback)
{
#ifdef TIMER_DEVELOPMENT_MODE
    if ((id < timers_cant) && ticks && (mode < CANT_TIM_MODES))
#endif // TIMER_DEVELOPMENT_MODE
    {
        // ****** COMPLETAR ******
        /*    uint16_t            period;
    uint16_t            cnt;
    timer_callback_t    callback;
    uint8_t             mode        : 1;
    uint8_t             running     : 1;
    uint8_t             expired     : 1;
    uint8_t             unused      : 5;*/
        timers[id].running = 0;
        timers[id].period = ticks; 
        timers[id].cnt = ticks; // descendente 
        timers[id].callback = callback;
        timers[id].mode = mode;
        timers[id].expired = 0;
        timers[id].running = 1;
    }
}


void timerStop(uint8_t id)
{
    if(timers[id].running){
        timers[id].running = 0;
    }
}


uint8_t timerExpired(uint8_t id)
{
    if(timers[id].expired){
        return 1;
    }
    else{
        return 0;
    }
}


void timerDelay(ticks_t ms)
{
    timerStart(TIMER_ID_INTERNAL, TIMER_MS2TICKS(ms), TIM_MODE_SINGLESHOT, NULL);
    while (!timerExpired(TIMER_ID_INTERNAL))
    {
        // wait...
    }
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void timer_isr(void)
{
    // ****** COMPLETAR ******
    // decremento los timers activos
    // si hubo timeout!
    // 1) execute action: callback or set flag
    // 2) update state

    volatile int i;
    for (i = 0; i < timers_cant; i++)
    {
        if (timers[i].running)
        {
            timers[i].cnt--;
            if (timers[i].cnt==0)
            {
                if (timers[i].mode == TIM_MODE_SINGLESHOT)
                {
                    timers[i].running = 0;
                    timers[i].expired = 1;
                    if(timers[i].callback != NULL){
                        timers[i].callback(); //call the function
                    }

                }
                else if (timers[i].mode == TIM_MODE_PERIODIC)
                {
                    timers[i].cnt =timers[i].period;
                    if(timers[i].callback != NULL){
                        timers[i].callback(); //call the function
                    }

                     
                }    
            }
            
            
            
        }
        
    }
    
    
}

/******************************************************************************/
