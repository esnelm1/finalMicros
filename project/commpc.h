/***************************************************************************//**
  @file     commpc.h
  @brief    commpc for .h file
  @author   Conradeus
 ******************************************************************************/

#ifndef _COMMPC_H_
#define _COMMPC_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "common.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE 
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
char check_comm();
void print_temperature();
void print_heater_state();
void get_setpoint(unsigned char* setpoint);
void get_histeresis(unsigned char* histeresis);
void get_intMuestra(unsigned int* int_muestra);
int recibe_parametros(unsigned char *setPoint, unsigned char *histeresis, unsigned int *intervaloMuestreo);
void LED_status(int value);
void uart_put_string(const char *str);



/*******************************************************************************
 ******************************************************************************/

#endif // _HARDWARE_H_
