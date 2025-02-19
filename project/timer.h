/***************************************************************************//**
  @file     timer.h
  @brief    Timer driver. Advance implementation
  @author   Conradeus
 ******************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define TIMER_TICK_MS       1
#define TIMER_MS2TICKS(ms)  ((ms)/TIMER_TICK_MS)

#define TIMERS_MAX_CANT     8
#define TIMER_INVALID_ID    255


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

// Timer Modes
enum { TIM_MODE_SINGLESHOT, TIM_MODE_PERIODIC, CANT_TIM_MODES };

// Timer alias
typedef uint16_t ticks_t;
typedef void (*timer_callback_t)(void);


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialice timer and corresponding peripheral
 */
void timerInit(void);


// Non-Blocking services ////////////////////////////////////////////////

/**
 * @brief Request an timer
 * @return ID of the timer to use
 */
uint8_t timerGetId(void);


/**
 * @brief Begin to run a new timer
 * @param id ID of the timer to start
 * @param ticks time until timer expires, in ticks
 * @param mode SINGLESHOT or PERIODIC
 * @param callback Function to be call when timer expires
 */
void timerStart(uint8_t id, ticks_t ticks, uint8_t mode, timer_callback_t callback);


/**
 * @brief Finish to run a timer
 * @param id ID of the timer to stop
 */
void timerStop(uint8_t id);


/**
 * @brief Verify if a timer has run timeout
 * @param id ID of the timer to check for expiration
 * @return 1 = timer expired!
 */
uint8_t timerExpired(uint8_t id);


// Blocking services ////////////////////////////////////////////////

/**
 * @brief Wait the specified time. Use internal timer
 * @param ticks time to wait in ticks
 */
void timerDelay(ticks_t ticks);


/*******************************************************************************
 ******************************************************************************/

#endif // _TIMER_H_
