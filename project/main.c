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
#include "commpc.h"
#include "UART.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
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

void AppInit(void){

    while (conexion == DESCONECTADO) //primer comunicarse con la compu por uart
    {
        if (check_comm()) //chequea conexion con la PC por UARt
        {
           LED_status(LED_NORMAL);
           conexion = CONECTADO;

           while (contador < TIMEOUT_COUNT) {
               if (recibe_parametros(&setPoint, &histeresis, &intervaloMuestreo)) { //Si recibe todos los parametros sale del while
                   break;
               }
               contador++;
           }
        }
    }   // PROBLEMA --> TIENE Q TENER UN TIEMPO EN EL QUE LEE EL UART --> SINO Q ENTRE EN EEPROM

    // SI no se conecta, lectura de eeprom
    if (!check_comm())
    {
        //LED_status(LED_ERROR);


        //si falla la comunicacion leer la eprom
    }



  //calefactor_turnOn();
}

void AppRun (void){

  //checkTemp();

    recibe_parametros(&setPoint, &histeresis, &intervaloMuestreo);

    //intento mandar informacion




    uart_put_string("1C");



}


void checkTemp(void){
  temp = readTemperatura();
  if (temp>temp_setpoint)
  {
    calefactor_turnOff();
  }
  else if (temp<temp_setpoint)
  {
    if (!calefactor_status()) //si el calefactor esta apagado lo prendo
    {
      calefactor_turnOn();
    }

  }


}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/******************************************************************************/
