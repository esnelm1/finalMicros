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
uint16_t temp_setpoint;//codificado igual que temp
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
unsigned int timeoutPeriod = 6000;
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

           //char *setpoint = get_setpoint();
           //char *histeresis = get_histeresis();
           //char *intMuestreo = get_intMuestreo();
           //printf("HELOOOOO");
        } else {

            //contador++;
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


  //calefactor_turnOn();
}

void AppRun (void){

  //checkTemp();

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
//#include <msp430.h>
//
//void main(void)
//{
//    // Parar el watchdog timer
//    WDTCTL = WDTPW | WDTHOLD;
//
//    // Configurar pines para UART:
//    // RX en P1.1 y TX en P1.2
//    P1SEL |= BIT1 | BIT2;
//    P1SEL2 |= BIT1 | BIT2;
//
//    // Configurar dirección: TX (P1.2) como salida, RX (P1.1) como entrada
//    P1DIR |= BIT2;       // TX como salida
//    P1DIR &= ~BIT1;      // RX como entrada
//
//    // (Opcional) Si se necesita, configurar resistencias internas para RX
//    // P1REN |= BIT1; // Activar resistencia interna
//    // P1OUT |= BIT1; // Elegir pull-up, por ejemplo
//
//    // Configurar LED en P1.6 (sin conflicto con UART)
//    P1DIR |= BIT6;       // LED como salida
//    P1OUT &= ~BIT6;      // Apagar LED inicialmente
//
//    // Configurar el módulo USCI_A0 para UART
//    UCA0CTL0 = 0;
//    UCA0CTL1 |= UCSWRST;    // Poner el módulo en reset para configurarlo
//    UCA0CTL1 |= UCSSEL_2;   // Seleccionar SMCLK (asumido a 1MHz)
//    UCA0BR0 = 104;          // Divisor para 9600 baudios (1MHz/9600  104)
//    UCA0BR1 = 0;
//    UCA0MCTL = UCBRS0;      // Modulación (valor típico; verifica según tu dispositivo)
//    UCA0CTL1 &= ~UCSWRST;   // Sacar el módulo de reset
//
//    // Bucle infinito de recepción
//    while (1)
//    {
//        // Esperar a que se reciba un dato
//        while (!(IFG2 & UCA0RXIFG));
//        char recibido = UCA0RXBUF;  // Leer el dato recibido
//
//        // Si se recibe el caracter 1 que en ASCII es '0x01'
//        if (recibido == 0x01)
//        {
//            // Encender el LED en P1.6
//            P1OUT |= BIT6;
//        }
//        else
//        {
//            // Apagar el LED
//            P1OUT &= ~BIT6;
//        }
//    }
//}
