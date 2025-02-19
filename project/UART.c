/***************************************************************************//**
  @file     UART.h
  @brief    UART global functions
  @author   Conradeus
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "UART.h"
#include "board.h"
#include "timer.h"
#include "hardware.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define RX_FULL   UCA0RXIFG
#define TX_EMPTY  UCA0TXIFG
#define RX_BUFFER UCA0RXBUF
#define TX_BUFFER UCA0TXBUF
#define UART_DELAY_MS 10
#define MAX_SCALE 255
#define MAX_VOLTAJE 3300
#define END_BYTE 0x46 //una F
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
TX_Buffer Tx_buffer = {.txBufferLength = 0, .txBufferIndex = 0, .transmitting = 0};  // Buffer de transmisiï¿½n inicializado
/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static unsigned char rxdata;
static void get_setpoint(unsigned char* sp);
static void get_histeresis(unsigned char* h);
static void get_intMuestra(unsigned int* im);
static int getRXStatus(void);
static int getTXStatus(void);


static char rx_datosFlag = 0;
/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void uart_init(void)
{
    QueueInit();  // Inicializa el buffer circular para RX

    // Configuración de pines
    P1SEL  |= PIN_UART_RX | PIN_UART_TX;
    P1SEL2 |= PIN_UART_RX | PIN_UART_TX;
    P1DIR |= 1 << 2;
    P1REN |= 1 << 1;
    P1OUT |= (1 << 1) + (1 << 2);

    // Configuración del módulo USCI_A0 para UART
    UCA0CTL0 = 0;
    UCA0CTL1 |= UCSWRST;      // Módulo en reset para configuración
    UCA0CTL1 |= UCSSEL_2;     // Selecciona SMCLK (asumido 8MHz)
    UCA0BR0 = 104;            // Divisor para 9600 baudios
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;        // Configuración de modulación
    UCA0CTL1 &= ~UCSWRST;     // Sale del reset
    IE2 |= UCA0RXIE;          // Habilita la interrupción de recepción
}

void uart_send_message(const char* msg, unsigned char len) // Función para enviar un mensaje completo
{
    // Si ya se está transmitiendo otro mensaje, se descarta este
    if(Tx_buffer.transmitting) return;
    memcpy(Tx_buffer.txBuffer, msg, len); //Si no se está transmitiendo, copia el mensaje recibido
    Tx_buffer.txBufferLength = len;
    Tx_buffer.txBufferIndex = 0;
    Tx_buffer.transmitting = 1;
    IE2 |= UCA0TXIE;

    if(getTXStatus() == TX_EMPTY)
    {
        TX_BUFFER = Tx_buffer.txBuffer[Tx_buffer.txBufferIndex++];
    }
}

void uart_put_char(char message)
{
    while(getTXStatus() != TX_EMPTY);
        TX_BUFFER = message;
}

unsigned char uart_get_char(void)
{
    if (getRXStatus()){
        PullQueue(&rxdata);
        return rxdata;
    }
    return 0;
}

char check_comunicacion(void)
{
    return rx_datosFlag;  // Establece comunicacion con Matlab
}

void LED_status_cases(int value){
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

int recibe_parametros(unsigned char *sp, unsigned char *h, unsigned int *im)
{
    // Se esperan4 bytes en el buffer
    if (rx_datosFlag)
    {
        get_setpoint(sp);
        get_histeresis(h);
        get_intMuestra(im);
        rx_datosFlag = 0;
        return 1;
    }
    return 0;
}

void print_status(uint16_t temp, char heater)
{
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
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void get_setpoint(unsigned char* sp)
{
    *sp = uart_get_char();
}

static void get_histeresis(unsigned char* h)
{
    *h = uart_get_char();
}

static void get_intMuestra(unsigned int* im)
{
    unsigned char lowByte = uart_get_char();   // Lee el byte bajo
    unsigned char highByte = uart_get_char();  // Lee el byte alto

    // Combina los dos bytes
    *im = ((unsigned int)highByte << 8) | lowByte;
}

static int getRXStatus(void)
{
    return QueueStatus();
}

static int getTXStatus(void)
{
    return (IFG2 & TX_EMPTY);
}

//INTERRUPT TX
#pragma vector = USCIAB0TX_VECTOR
__interrupt void uart_tx_isr(void)
{
    if(Tx_buffer.transmitting)
    {
         if(Tx_buffer.txBufferIndex < Tx_buffer.txBufferLength)
         {
             TX_BUFFER = Tx_buffer.txBuffer[Tx_buffer.txBufferIndex++];
         }
         else
         {
             Tx_buffer.transmitting = 0;  // Mensaje completado
             IE2 &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
         }
    }
}

//INTERRUPT RX
#pragma vector = USCIAB0RX_VECTOR
__interrupt void uart_rx_isr(void)
{
    if (IFG2 & RX_FULL)
    {
        if (RX_BUFFER == END_BYTE){
            rx_datosFlag = 1;
        } else {
            PushQueue(RX_BUFFER);
            rx_datosFlag = 0;
        }
        IFG2 &= ~RX_FULL; //  se limpia el flag de RX_FULL
    }
}
