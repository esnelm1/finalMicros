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
#define RX_FULL  UCA0RXIFG  //UCA0RXIFG is set when UCA0RXBUF has received a complete character
#define TX_EMPTY UCA0TXIFG  //UCA0TXIFG is set when UCA0TXBUF is empty
#define RX_BUFFER UCA0RXBUF //receive buffer
#define TX_BUFFER UCA0TXBUF //transmit buffer

#define UART_DELAY_MS 10
#define MAX_SCALE 255
#define MAX_VOLTAJE 3300

#define NEW_LINE 10         //ASCII for new line
#define FINISH_MESSAGE 13   //ASCII for finish transmition --> CR (Carriage return)
#define MAX_MESSAGES 15

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
void uart_put_char(char message);
char uart_get_char(void);

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
    P1SEL |= PIN_UART_TX;
    P1SEL2 |= PIN_UART_TX;

    // micellaneos - VERIFICAR, ES DISTINTO A LO DE JACOBY
    UCA0CTL1 = UCSWRST;   //Reset state
    UCA0CTL0 &= ~UCPEN;   //parity not enabled
    UCA0CTL0 &= ~UCMSB;   //LSB first
    UCA0CTL0 &= ~UC7BIT;  //8 bits character
    UCA0CTL0 &= ~UCSPB;   //one stop bit
    UCA0CTL1 |= UCSSEL_2; //SMCLK
    UCA0CTL1 &= ~UCSWRST; //disable reset start
    IE2 |= UCA0RXIE;      //enable rx interrupts

    //baud rate definition 
    UCA0BR0 = 65;
    UCA0BR1 = 3;          // (UCA0BR0+256*UCA0BR1) = 833
    UCA0MCTL = UCBRS1;    // USCI Second Stage Modulation Select 1

}


void uart_put_char(char message)
{
    while (getTXStatus() != TX_EMPTY) // USCI_A0 TX buffer ready?
        TX_BUFFER = message;
    uart_finish(message);
}

char uart_get_char(void)
{
    unsigned char rxdata;
    while (getRXStatus())
     {
        PullQueue(&rxdata);          //pulls data from buffer and assigns it to rxdata pointer
     }
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
    return (IFG2 & TX_EMPTY);         //check s that TX buffer is empty and UART flag is set
}


static int getRXStatus(void)
{
    return (QueueStatus());          //returns buffer status

}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void uart_rx_isr(void)
{
    if (IFG2 &RX_FULL)
    {   
        PushQueue (RX_BUFFER);       //checks dedicated flag   
        IFG2 &= ~RX_FULL;            //sets local flag
    }
}

/******************************************************************************/

