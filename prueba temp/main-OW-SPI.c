#include <msp430.h>
#include <stdint.h>





#define DELAY_REFRESH_LED 16000000 // 1 segundo (16 MHz)
#define PIN_SPI_CLK       (1 << 0) // P2.0
#define PIN_SPI_MOSI      (1 << 1) // P2.1
#define PIN_LATCH_ENABLE  (1 << 2) // P2.2
//------------------------------------------------------------------------------
// Comandos del protocolo Single-wire
//------------------------------------------------------------------------------
#define READ_ROM            0x33  // Leer ROM
#define MATCH_ROM           0x55  // Coincidir con ROM
#define SKIP_ROM            0xCC  // Omitir ROM

#define WRITE_MEM           0x0F  // Escribir memoria
#define VERIFY_MEM          0xAA  // Verificar memoria
#define COPY_MEM            0x55  // Copiar memoria
#define READ_MEM            0xF0  // Leer memoria

#define CONVERT_T           0x44  // Comando para que el sensor convierta la temperatura a un valor digital
#define READ_SCRATCHPAD     0xBE  // Leer la temperatura digital
//------------------------------------------------------------------------------
// Definiciones para el tiempo
//------------------------------------------------------------------------------
#define CLKFREQ     16u          // Frecuencia del reloj en MHz
#define RESET_LOW    (500 * CLKFREQ)
#define RESET_DELAY  (360 * CLKFREQ)
#define WRITE_ONE    (6 * CLKFREQ)
#define DELAY_ONE    (64 * CLKFREQ)
#define WRITE_ZERO   (60 * CLKFREQ)
#define DELAY_ZERO   (10 * CLKFREQ)
#define READ_LOW     (6 * CLKFREQ)
#define READ_DETECT  (9 * CLKFREQ)
#define READ_DELAY   (55 * CLKFREQ)

//------------------------------------------------------------------------------
// Variables globales
int16_t temp_read = 0; // Variable para almacenar la temperatura (en 0.0625 C)
uint16_t temp_scaled;
//------------------------------------------------------------------------------
// Declaracion de funciones
//------------------------------------------------------------------------------
void Single_reset(void);
void Single_write(uint8_t);
uint8_t Single_read(void);
void barraled_write_temp(uint16_t);
void spi_write_temp(char);
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Detener el watchdog

    // Configurar el DCO a 16 MHz
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    // Configuracion GPIO
    P1OUT = BIT5;  // P1.5 salida alta
    P1DIR |= BIT5; // P1.5 como salida
     // Configurar pines como salida
    P2DIR |= PIN_SPI_MOSI | PIN_SPI_CLK | PIN_LATCH_ENABLE;
    P2OUT &= ~(PIN_SPI_MOSI | PIN_SPI_CLK | PIN_LATCH_ENABLE);

    uint8_t identification[8] = {0};
    uint8_t byte;

    Single_reset();
    Single_write(READ_ROM);
    for (byte = 0; byte < 8; byte++) {
        identification[byte] = Single_read();
    }

    while (1) {
            // Paso 1: Reiniciar
            Single_reset();

            // Paso 2: Enviar comando SKIP_ROM
            Single_write(SKIP_ROM);

            // Paso 3: Enviar comando CONVERT_T
            Single_write(CONVERT_T);

            // Esperar el tiempo necesario para la conversion (750 ms para 12 bits)
            __delay_cycles(16000000 * 0.75); // Asumiendo DCO a 16 MHz

            // Paso 4: Reiniciar y enviar comando READ_SCRATCHPAD
            Single_reset();
            Single_write(SKIP_ROM);           // Comando SKIP_ROM
            Single_write(READ_SCRATCHPAD);    // Comando READ_SCRATCHPAD
            // Paso 5: Leer los dos primeros bytes (LSB y MSB) del Scratchpad
            uint8_t temp_lsb = Single_read();
            uint8_t temp_msb = Single_read();

            // Paso 6: Combinar bytes para formar el valor en bruto
            temp_read = (temp_msb << 8) | temp_lsb;// Variable para almacenar la temperatura (en 0.0625 C)

            temp_scaled = (temp_read * 10) / 16;//convierto a 4 cifras. 0445 es 44.5.C
            barraled_write_temp(temp_scaled);



    }
}

//------------------------------------------------------------------------------
// Implementacion de Single_reset()
//------------------------------------------------------------------------------
void Single_reset(void) {
    P1OUT &= ~BIT5;  // P1.5 a bajo
    __delay_cycles(RESET_LOW);
    P1DIR &= ~BIT5;  // P1.5 como entrada
    __delay_cycles(RESET_DELAY);
    P1OUT |= BIT5;   // P1.5 en alto
    P1DIR |= BIT5;   // P1.5 como salida
}

//------------------------------------------------------------------------------
// Implementacion de Single_write()
//------------------------------------------------------------------------------
void Single_write(uint8_t data) {
    uint8_t ibit;
    for (ibit = 0; ibit < 8; ibit++) {
        P1OUT &= ~BIT5;
        if ((data >> ibit) & 0x01) {
            __delay_cycles(WRITE_ONE);
            P1OUT |= BIT5;
            __delay_cycles(DELAY_ONE);
        } else {
            __delay_cycles(WRITE_ZERO);
            P1OUT |= BIT5;
            __delay_cycles(DELAY_ZERO);
        }
    }
}

//------------------------------------------------------------------------------
// Implementacion de Single_read()
//------------------------------------------------------------------------------
uint8_t Single_read(void) {
    uint8_t data = 0;
    uint8_t ibit;
    for (ibit = 0; ibit < 8; ibit++) {
        P1OUT &= ~BIT5;      // Llevar linea a bajo
        __delay_cycles(READ_LOW);
        P1DIR &= ~BIT5;      // P1.5 como entrada
        __delay_cycles(READ_DETECT);
        if (P1IN & BIT5) {
            data |= (1 << ibit);
        }
        __delay_cycles(READ_DELAY);
        P1OUT |= BIT5;       // Llevar linea a alto
        P1DIR |= BIT5;       // P1.5 como salida
    }
    return data;
}


void barraled_write_temp(uint16_t data){
    uint16_t temp = data/10;

    int bits_en_1 = (temp * 8) / 50; // Proporcional a la temperatura (0-8 bits)
    if (bits_en_1>8) {
        bits_en_1 = 8;
    }

    // Generar el valor de nivel con los bits más significativos en 1
    unsigned char nivel = (1 << bits_en_1) - 1;

    spi_write_temp(nivel);

}

void spi_write_temp(char n) {
    int i;
    P2OUT &= ~PIN_LATCH_ENABLE;
    for (i = 0; i < 8; i++) {
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
    //__delay_cycles(10); // Pequenio retardo
    //P2OUT &= ~PIN_LATCH_ENABLE;

    // Retardo de 1 segundo para visualizar el cambio
    __delay_cycles(DELAY_REFRESH_LED);
}
