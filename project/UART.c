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

#define HANDSHAKE_CMD 'H'
#define TEMP_CMD 'T'
#define HEATER_CMD 'S'


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void uart_send_temperature(void)
void uart_send_heater_state(void)
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

    // micellaneos 
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

void uart_send_string(const char *str) {
    while (*str) {
        while (!(IFG2 & TX_EMPTY));
        TX_BUFFER = *str++;
    }
}


static void uart_handle_command(char *cmd) {
    if (strncmp(cmd, "SET:", 4) == 0) {
        int sp, h, si;
        if (sscanf(cmd, "SET: SP = %d, H = %d, SI = %d", &sp, &h, &si) == 3) {
            // Guardamos los valores en variables globales o estructuras
            setPoint = sp;
            histeresis = h;
            intervalo_muestreo = si;
            
            uart_send_string("SET_OK\n");
        } else {
            uart_send_string("SET_ERR\n");
        }
    } else {
        uart_send_string("CMD_ERR\n");
    }
}
/*******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void uart_send_temperature(void) {
    char buffer[20];
    float temp = temp_ReadTemperature();
    int state = calefactor_status();
    snprintf(buffer, sizeof(buffer), "%.2f,%d\n", temp,state);
    uart_send_string(buffer);
}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void uart_rx_isr(void) {
    if (IFG2 & RX_FULL) {
        char cmd = RX_BUFFER;
        PushQueue(cmd);
        uart_handle_command(cmd);
        IFG2 &= ~RX_FULL;
    }
}

/******************************************************************************/

