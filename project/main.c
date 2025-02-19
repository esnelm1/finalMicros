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
#include "statusLed.h"
#include "barraLed.h"
#include "calefactor.h"
#include "temperatura.h"
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

    //check_comunicacion()  //Para volver a conectar a Matlab
    recibe_parametros(&setPoint, &histeresis, &intervaloMuestreo);
    checkTemp();
}


void checkTemp(void){
    // Lectura de temperatura (se debe reemplazar readTemperatura() por la función real)
    temp = readTemperatura();
    //temp = 445;  // Ejemplo: 445 se muestra como "0445" en el formato de 4 dígitos
    temp_setpoint = 1000;
    barraled_write_temp(temp);
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
    print_status(temp,calefactor_status());
}


