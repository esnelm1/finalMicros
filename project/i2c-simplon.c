#include <msp430.h>

#define EEPROM_ADDR  0x0000  // Dirección base de la EEPROM 24LC512
#define I2C_SCL      BIT6  // P1.6
#define I2C_SDA      BIT7  // P1.7
#define DATA_ADDR    0x0000 // Dirección de memoria donde se escribirá el dato

void I2C_Init();
void I2C_WriteByte(unsigned int mem_addr, unsigned char data);
unsigned char I2C_ReadByte(unsigned int mem_addr);
void delay_ms(unsigned int ms);

int main() {
    WDTCTL = WDTPW | WDTHOLD;  // Detener el watchdog timer
    I2C_Init();


    char patterns[] = {47,48};


    int index = 0;

        while (1) {
            I2C_WriteByte(DATA_ADDR, patterns[index]); // Escribir 47 en la EEPROM
            delay_ms(1000); // Esperar 1 segundo
            unsigned char read_data = I2C_ReadByte(DATA_ADDR); // Leer el dato

            index ^= 1; // Alternar entre 0 y 1
        }
}

void I2C_Init() {
    P1SEL |= I2C_SCL | I2C_SDA;
    P1SEL2 |= I2C_SCL | I2C_SDA;

    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;  // Modo maestro, I2C síncrono
    UCB0CTL1 = UCSSEL_2 | UCSWRST;         // SMCLK, seguir en reset

    UCB0BR0 = 160;  // Para 100 kHz
    UCB0BR1 = 0;

    UCB0CTL1 &= ~UCSWRST;  // Salir del reset
}


void I2C_WriteByte(unsigned int mem_addr, unsigned char data) {
    while (UCB0CTL1 & UCTXSTP);
    UCB0CTL1 |= UCTR | UCTXSTT; // Modo de transmisión y condición de inicio

    while (!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = EEPROM_ADDR << 1;
    while (!(IFG2 & UCB0TXIFG));

    UCB0TXBUF = (mem_addr >> 8) & 0xFF;
    while (!(IFG2 & UCB0TXIFG));

    UCB0TXBUF = mem_addr & 0xFF;
    while (!(IFG2 & UCB0TXIFG));

    UCB0TXBUF = data;
    while (!(IFG2 & UCB0TXIFG));

    UCB0CTL1 |= UCTXSTP; // Condición de parada
    while (UCB0CTL1 & UCTXSTP);
}

unsigned char I2C_ReadByte(unsigned int mem_addr) {
    unsigned char data;

    while (UCB0CTL1 & UCTXSTP);
    UCB0CTL1 |= UCTR | UCTXSTT;

    while (!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = EEPROM_ADDR << 1;
    while (!(IFG2 & UCB0TXIFG));

    UCB0TXBUF = (mem_addr >> 8) & 0xFF;
    while (!(IFG2 & UCB0TXIFG));

    UCB0TXBUF = mem_addr & 0xFF;
    while (!(IFG2 & UCB0TXIFG));

    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;
    while (UCB0CTL1 & UCTXSTT);

    UCB0CTL1 |= UCTXSTP;
    while (!(IFG2 & UCB0RXIFG));
    data = UCB0RXBUF;

    return data;
}

void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_cycles(1000);
    }
}

