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
static char* get_numeros(char *var, char *sf);
void uart_read_line(char *buffer, size_t size);

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
    unsigned char message = uart_get_char();
    return (message == 0x01);  //Recibe dato en ascii
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

char* get_setpoint()
{
    char *sp = get_numeros(uart_get_char(),"SP");
    return sp;
}

char* get_histeresis()
{
    char *h = get_numeros(uart_get_char(),"H");
    return h;
}

char* get_intMuestreo()
{
    char *im = get_numeros(uart_get_char(),"IM");
    return im;
}


void LED_conectionStatus_init(void)
{
    gpioMode(LED_STATUS,OUTPUT);
    gpioWrite(LED_STATUS,LOW);
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

/*******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static char* get_numeros(char *var, char *sf) {
    // Verificar que los punteros no sean nulos
    if (var == NULL || sf == NULL) {
        return NULL;
    }

    // Buscar el sufijo en la cadena
    char *pos = strstr(var, sf);
    if (pos != NULL) {
        // Extraer los números antes del sufijo
        size_t len = pos - var; // Longitud de los números

        // Reservar memoria dinámica
        char *numeros = (char*)malloc((len + 1) * sizeof(char));
        if (numeros == NULL) {
            return NULL; // Si falla la asignación de memoria
        }

        strncpy(numeros, var, len);
        numeros[len] = '\0'; // Agregar terminador nulo
        return numeros; // Retornar puntero a memoria dinámica
    } else {
        printf("No coinciden con el sufijo %s\n", sf);
        return NULL; // Devolver NULL si no hay coincidencia
    }
}

/******************************************************************************/

