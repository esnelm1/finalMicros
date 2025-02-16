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

static int getTXStatus(void);
static int getRXStatus(void);
static void uart_finish(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

//unsigned char uart_message;
//char hexString[3]
unsigned long uart_message;

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

void uart_put_char(char message)
{
    while (getTXStatus() != TX_EMPTY) // USCI_A0 TX buffer ready?
        TX_BUFFER = message;
        uart_finish();
}

char uart_get_char(void)
{
    unsigned char rxdata;
    //while (getRXStatus()) {
        PullQueue(&rxdata);          //pulls data from buffer and assigns it to rxdata pointer
    //}
    return rxdata;
}



/*******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void uart_finish(void)
{
    while (getTXStatus() != TX_EMPTY) // USCI_A0 TX buffer ready?
        TX_BUFFER = FINISH_MESSAGE;
}


static int getTXStatus(void)
{
    return (IFG2 & TX_EMPTY);         //checks that TX buffer is empty and UART flag is set

}


static int getRXStatus(void)
{
    return (QueueStatus());          //returns buffer status

}


#pragma vector = USCIAB0RX_VECTOR
__interrupt void uart_rx_isr(void)
{
    if (IFG2 &RX_FULL)
    {
        PushQueue (RX_BUFFER);       //checks dedicated flag
        IFG2 &= ~RX_FULL;            //sets local flag
    }
}

/******************************************************************************/

