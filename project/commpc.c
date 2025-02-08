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

char check_comm()
{
    char checker = uart_get_char(); //Matlab manda un 1 para establecer comm.
    char message = 1;
    if(checker == 1)
    {
        return message;
    } else   // No llega el mensaje
    {
        return !(message);
    }
}

void print_temperature()
{
    char value = readTemperatura();
    uart_put_char(value);
    uart_put_char('T');
    uart_put_char(FINISH_MESSAGE); //con CR
}

void print_heater_state()
{
    char value = calefactor_status();
    uart_put_char(value);
    uart_put_char('C');
    uart_put_char(FINISH_MESSAGE); //con CR
}

char get_setpoint()
{
    char sp[] = get_numeros(uart_get_char(),"SP")
    return sp;
}

char get_histeresis()
{
    char h[] = get_numeros(uart_get_char(),"H")
    return h;
}

char get_intMuestreo()
{
    char im[] = get_numeros(uart_get_char(),"IM")
    return im;
}



char get_numeros(char *var, char *sf) {
    // Buffer estático para almacenar los números
    char numeros[10];

    // Buscar el sufijo en la cadena
    char *pos = strstr(var, sf);
    if (pos != NULL) {
        // Extraer los números antes del sufijo
        size_t len = pos - var; // Longitud de los números
        strncpy(numeros, var, len);
        numeros[len] = '\0'; // Agregar terminador nulo
    } else {
        // Si no se encuentra el sufijo, considerar todo como números
        strcpy(numeros, var);
        printf("No coinciden con el sufijo %s",sf);
        return;
    }
    return numeros; // Devolver los números
}


void statusLed_init(void){
    gpioMode(LED_STATUS,OUTPUT);
    gpioWrite(LED_STATUS,LOW);
}


void statusLed(int value){
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

//void uart_handle_command() {
//    if (strncmp(cmd, "SET:", 4) == 0) {
//        int sp, h, si;
//        if (sscanf(cmd, "SET: SP = %d, H = %d, SI = %d", &sp, &h, &si) == 3) {
//            // Guardamos los valores en variables globales o estructuras
//            setPoint = sp;
//            histeresis = h;
//            intervalo_muestreo = si;
//
//            uart_put_char("SET_OK\n");
//        } else {
//            uart_put_char("SET_ERR\n");
//        }
//    } else {
//        uart_put_char("CMD_ERR\n");
//    }
//}





/******************************************************************************/

