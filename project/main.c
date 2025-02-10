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

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define UMBRAL_PORCENTAGE 100
#define TIMEOUT_COUNT 5000
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
char temp_setpoint;//codificado igual que temp
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void AppInit(void);
void AppRun(void);
void checkTemp(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
uint8_t setPoint, histeresis, calefactor = 0;
uint16_t intervaloMuestreo;
unsigned int timeout = 5000;
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
    systemInitLast();

    uart_init();
    conectionStatus_init();

    for(;;)
        AppRun();
} // NO TOCAR

void appInit(void){
    int conexion = DESCONECTADO;
    int contador = 0;
    while (contador < TIMEOUT_COUNT || conexion)   //primer comunicarse con la compu por uart
    {
        contador++;
        if (check_comm())
        {
           LED_status(LED_NORMAL);

           char *setpoint = get_setpoint();
           char *histeresis = get_histeresis();
           char *intMuestreo = get_intMuestreo();
           conexion = CONECTADO;
        }
    }
    // SI no se conecta, lectura de eeprom
    if (!check_comm())
    {
        LED_status(LED_ERROR);


        //si falla la comunicacion leer la eprom
    }


  //primer comunicarse con la compu por uart
  //si falla la comunicacion leer la eprom
  // de la compu o de la eprom obtengo el setpoint

  
  calefactor_turnOn();
}

void AppRun (void){

  checkTemp();

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
