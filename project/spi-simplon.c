#include <msp430.h>

#define DELAY_REFRESH_LED 16000000 // 1 segundo (16 MHz)
#define PIN_SPI_CLK       (1 << 0) // P2.0
#define PIN_SPI_MOSI      (1 << 1) // P2.1
#define PIN_LATCH_ENABLE  (1 << 2) // P2.2

void spi_write_temp(char n);

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Detener el watchdog

    // Configurar el DCO a 16 MHz
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    // Configurar pines como salida
    P2DIR |= PIN_SPI_MOSI | PIN_SPI_CLK | PIN_LATCH_ENABLE;
    P2OUT &= ~(PIN_SPI_MOSI | PIN_SPI_CLK | PIN_LATCH_ENABLE);

    char patterns[] = {0b10101010, 0b01010101}; // Alternar LEDs
    int index = 0;

    while (1) {
        spi_write_temp(patterns[index]);
        index ^= 1; // Alternar entre 0 y 1
    }
}

void spi_write_temp(char n) {
    int i = 0;
    P2OUT &= ~PIN_LATCH_ENABLE;
    for (i; i < 8; i++) {
        P2OUT &= ~PIN_SPI_CLK;

        if (n & 0x80) {
            P2OUT |= PIN_SPI_MOSI;
        } else {
            P2OUT &= ~PIN_SPI_MOSI;
        }

        P2OUT |= PIN_SPI_CLK;  // Pulso de reloj
        n <<= 1;
    }

    // Activar el latch
    P2OUT |= PIN_LATCH_ENABLE;
    //__delay_cycles(10); // Pequeño retardo
    //P2OUT &= ~PIN_LATCH_ENABLE;

    // Retardo de 1 segundo para visualizar el cambio
    __delay_cycles(DELAY_REFRESH_LED);
}
