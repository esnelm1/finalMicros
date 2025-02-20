/***************************************************************************//**
  @file     UART.h
  @brief    UART global functions
  @author   Conradeus
 ******************************************************************************/
#ifndef _UART_H_
#define _UART_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "common.h"
#include "cqueue.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE 
 ******************************************************************************/
#define NEW_LINE 10         //ASCII for new line
#define FINISH_MESSAGE 13   //ASCII for finish transmition --> CR (Carriage return)
#define MAX_MESSAGES 15

#define CONECTED  0xff
#define DISCONNECT 0xfe
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
    char txBuffer[QSIZE];   // Tamaño ajustable según necesidades
    unsigned char txBufferLength;   // Array para almacenar los datos recibidos
    unsigned char txBufferIndex;       // ï¿½ndice para rastrear la posiciÃ³n actual en el array
    unsigned char transmitting;
} TX_Buffer;
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void uart_init(void);
void uart_set_message(unsigned long);
void uart_put_char(char message);
unsigned char uart_get_char(void);
void uart_send_message(const char* msg, unsigned char len);
char check_comunicacion();
int recibe_parametros(unsigned char *setPoint, unsigned char *histeresis, unsigned int *intervaloMuestreo);
void print_status(uint16_t temp, char heater);
/*******************************************************************************
 ******************************************************************************/

#endif // _HARDWARE_H_
