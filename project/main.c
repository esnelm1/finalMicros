/***************************************************************************//**
  @file     main.c
  @brief    Main file
  @author   Conradeus
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "system.h"
#include "board.h"
#include "eprom.h"
#include "commpc.h"
#include "UART.h"

//*******************************************************************************
// * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
// ******************************************************************************/
#define UMBRAL_PORCENTAGE 100
#define TIMEOUT_COUNT 3000
#define LED_NORMAL 1
#define LED_ERROR 0
#define CONECTADO 1
#define DESCONECTADO 0

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

uint16_t temp; //temperatura codificada con 4 digitos. 3 para grados y 1 para la decima
uint16_t temp_setpoint; //codificado igual que temp
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void AppInit(void);
void AppRun(void);
void checkTemp(void);
void print_status(void);
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
unsigned char setPoint, histeresis;
unsigned int intervaloMuestreo;

unsigned char calefactor = 0;

char conexion = DESCONECTADO;
unsigned int contador = 0;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void main (void)
{ // NO TOCAR
    systemInitFirst();
    boardInit();
    timer_set_init();
    LED_conectionStatus_init();
    systemInitLast();
    AppInit();

    for(;;)
        AppRun();
} // NO TOCAR

void AppInit(void){ //PRIMERA COMUNICACION CON EL UART

    while (conexion == DESCONECTADO) //primer comunicarse con la compu por uart
    {
        if (check_comunicacion()) //chequea conexion con la PC por UARt
        {
            LED_status_cases(LED_NORMAL);
            conexion = CONECTADO;
            recibe_parametros(&setPoint, &histeresis, &intervaloMuestreo);
        }
    }
}
void AppRun (void){

    // Para volver a conectar a Matlab
    //check_comunicacion();
    recibe_parametros(&setPoint, &histeresis, &intervaloMuestreo);
    checkTemp();
}


void checkTemp(void){
    // Lectura de temperatura (se debe reemplazar readTemperatura() por la función real)
    // temp = readTemperatura();
    temp = 445;  // Ejemplo: 445 se muestra como "0445" en el formato de 4 dígitos
    temp_setpoint = 1000;

    if (temp > temp_setpoint)
    {
        calefactor_turnOff();
    }
    else if (temp < temp_setpoint)
    {
        if (!calefactor_status()) // Si el calefactor está apagado, se enciende
        {
            calefactor_turnOn();
        }
    }
    // Se envía la información por TX: temperatura (4 dígitos) y estado del calefactor (1 dígito)
    print_status();
}

void print_status(void)
{
    int heater = calefactor_status();
    char msg[6];  // 4 dígitos para la temperatura, 1 dígito para el estado y 1 carácter CR

    // Conversión manual a 4 dígitos (se añaden ceros a la izquierda si es necesario)
    msg[0] = '0' + ((temp / 1000) % 10);
    msg[1] = '0' + ((temp / 100) % 10);
    msg[2] = '0' + ((temp / 10) % 10);
    msg[3] = '0' + (temp % 10);

    // Estado del calefactor (asumido que es 0 o 1)
    msg[4] = '0' + (heater & 0xF);

    // Caracter de finalización
    msg[5] = FINISH_MESSAGE;

    uart_send_message(msg, 6);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/******************************************************************************/

//#include "system.h"
//#include "board.h"
//#include "eprom.h"
//#include "commpc.h"
//#include "UART.h"
//
//unsigned char setPoint, histeresis;
//unsigned int intervaloMuestreo;
//
//void main (void){
//
//    systemInitFirst();
//    boardInit();
//    timer_set_init();
//    LED_conectionStatus_init();
//    systemInitLast();
//
//    while (1) //primer comunicarse con la compu por uart
//    {
//        if (check_comunicacion()) //chequea conexion con la PC por UARt
//        {
//            LED_status(LED_NORMAL);
//            conexion = CONECTADO;
//            recibe_parametros(&setPoint, &histeresis, &intervaloMuestreo)
//        }
//    }
//}
