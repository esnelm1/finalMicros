/***************************************************************************//**
 @file     commpc.c
 @brief    commpc for .c file
 @author   Conradeus
 ******************************************************************************/

/*Este archivo es para armar la comunicacion con la computadora. La comunicacion es
por uart. Hay que ver de hacer funciones para enviar y transmitir datos, y leerlas
con una interfaz de matlab. Hay que ver como hacer que sea robusta y no pierda datos*/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "commpc.h"
#include "cqueue.h"
#include "UART.h"
#include "calefactor.h"
#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
//static char* get_numeros(char *var, char *sf);


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// FUNCIONES DE LECTURA DE VOLTAJE DEL SENSOR DE TEMPERATURA - LA TRANSFORMACION DEL VALOR
// DE TEMPERATURA SE PUEDE HACER ACA O EN MATLAB. SI LO HACEMOS ACA ES MAS ROBUSTO PERO
// TENEMOS MENOS HERRAMIENTAS MATEMATICAS. YO LO HARIA CACA

// FUNCION DE LECTURA DE ESTADO DE CALEFACTOR. SE DEBE USAR calefactor_status

/*******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


char check_comm(void)
{
    return (uart_get_char() == 1); // Establece comunicacion con Matlab
}

void print_temperature()
{
    //char value = readTemperatura();
    //uart_put_char(value);
    uart_put_char('T');
    uart_put_char(FINISH_MESSAGE); //con CR
}

void print_heater_state()
{
    //char value = calefactor_status();
    //uart_put_char(value);
    uart_put_char('C');
    uart_put_char(FINISH_MESSAGE); //con CR
}

void get_setpoint(unsigned char* sp) {*sp = uart_get_char();}

void get_histeresis(unsigned char* h) {*h = uart_get_char();}

void get_intMuestra(unsigned int* im)
{
    unsigned char lowByte = uart_get_char();   // Lee el byte bajo
    unsigned char highByte = uart_get_char();  // Lee el byte alto

    // Combina los dos bytes
    *im = ((unsigned int)highByte << 8) | lowByte;
}


void LED_status(int value){
    switch (value)
        {
        case 0: //funcionamiento error
            gpioWrite(LED_STATUS,LOW);
            break;

        case 1: //funcionamiento normal
            gpioWrite(LED_STATUS,HIGH);
            break;
        }
}

int recibe_parametros(unsigned char *setPoint, unsigned char *histeresis, unsigned int *intervaloMuestreo)
{
    // Se esperan al menos 4 bytes en el buffer
    if (getRXStatus() > 3)
    {
        get_setpoint(setPoint);
        get_histeresis(histeresis);
        get_intMuestra(intervaloMuestreo);
        return 1;
    }
    return 0;
}

void uart_put_string(const char *str)
{
    while (*str) {
        uart_put_char(*str++);
    }
}

/*******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/******************************************************************************/

