/***************************************************************************//**
 @file     display.c
 @brief    display control
 @author   Conradeus
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "UART.h"
#include "cqueue.h"
#include "common.h"
#include "board.h"
#include "timer.h"
#include "hardware.h"
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define RX_FULL   UCA0RXIFG  //UCA0RXIFG is set when UCA0RXBUF has received a complete character
#define TX_EMPTY  UCA0TXIFG  //UCA0TXIFG is set when UCA0TXBUF is empty
#define RX_BUFFER UCA0RXBUF //receive buffer
#define TX_BUFFER UCA0TXBUF //transmit buffer

#define UART_DELAY_MS 10
#define MAX_SCALE 255
#define MAX_VOLTAJE 3300


//type of messages
enum
{
    RAW_MESSAGE, VOLTAGE_MESSAGE
};

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

unsigned long uart_message;
unsigned char rxdata;

volatile unsigned char txBuffer[QSIZE];
volatile unsigned int txHead = 0;
volatile unsigned int txTail = 0;

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void uart_init(void)
{
    
    QueueInit();          //buffer init
    
    // port init
    P1SEL  |= PIN_UART_RX | PIN_UART_TX;
    P1SEL2 |= PIN_UART_RX | PIN_UART_TX;
    P1DIR |= 1<<2;
    P1REN |= 1<<1; /* Place UCA0 in Reset to be configured */
    P1OUT |= (1<<1) + (1<<2);

    // Configurar el módulo USCI_A0 para UART
    UCA0CTL0 = 0;
    UCA0CTL1 |= UCSWRST;    // Poner el módulo en reset para configurarlo
    UCA0CTL1 |= UCSSEL_2;   // Seleccionar SMCLK (asumido a 8MHz)
    UCA0BR0 = 104;          // Divisor para 9600 baudios (8MHz/9600 = 51)
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;      // Modulación (valor típico; verifica según tu dispositivo)
    UCA0CTL1 &= ~UCSWRST;   // Sacar el módulo de reset
    // Habilita la interrupción de recepción
    IE2 |= UCA0RXIE;

}

#include "UART.h"
#include "cqueue.h"
// ... otras inclusiones

// Eliminamos las variables locales de TX que antes estaban definidas, ya que usaremos las de cqueue:
// volatile unsigned char txBuffer[QSIZE];
// volatile unsigned int txHead = 0;
// volatile unsigned int txTail = 0;

void uart_put_char(char message)
{
    __disable_interrupt(); // Protege la sección crítica

    // Intenta insertar el byte en la cola de transmisión.
    // Si la cola está llena, espera (busy-wait). Podrías implementar un timeout si es necesario.
    while(PushQueue_TX(message) == QFULL)
    {
        ; // Espera hasta que haya espacio en el buffer TX
    }

    // Habilita la interrupción de TX para que se inicie la transmisión de datos.
    IE2 |= UCA0TXIE;

    __enable_interrupt();
}


char uart_get_char(void)
{
    //while (getRXStatus()) {
    PullQueue(&rxdata);          //pulls data from buffer and assigns it to rxdata pointer
    //}
    return rxdata;
}

int getRXStatus(void)
{
    return (QueueStatus());          //returns buffer status

}

/*******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// INTERRUPT RX
#pragma vector = USCIAB0RX_VECTOR
__interrupt void uart_rx_isr(void)
{
    if (IFG2 & RX_FULL)
    {
        PushQueue (RX_BUFFER);       //checks dedicated flag
        IFG2 &= ~RX_FULL;            //sets local flag
    }
}

// Ver de poner interrupt para mandar
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    unsigned char byte;
    // Extrae el siguiente byte de la cola TX y envíalo
    if(PullQueue_TX(&byte) == QOK)
    {
         TX_BUFFER = byte;
    }
    else
    {
         // No quedan datos, deshabilita la interrupción de TX
         IE2 &= ~UCA0TXIE;
    }
}

