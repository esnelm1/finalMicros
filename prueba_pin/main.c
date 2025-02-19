/**
 * Simple blink
 * For MSP-EXP430G2 LaunchPad boards to run on a MSP430G2553 MCU
 */

#include <msp430.h>

// On Board User Leds
#define LED_GREEN_MSK  (1<<0)          // P1.0 (00000001)
#define LED_RED_MSK    (1<<6)          // P1.6 (00100000)
#define LED_RED_MSK2   (1<<7)          // P1.6 (00100000)
#define LED_RGB_MSK    (1<<5) | (1<<1) // P2.5 | P2.1


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    P1DIR |= LED_GREEN_MSK | LED_RED_MSK | LED_RED_MSK2; // configure Pin as output (00100001)
    P1OUT =  LED_GREEN_MSK | LED_RED_MSK | LED_RED_MSK2; // initial state (ESTOS SON LOS REGISTROS)
    P2DIR |= LED_RGB_MSK; // configure Pin as output
    P2OUT =  LED_RGB_MSK; // initial state


    volatile unsigned int i;

    while(1)
    {
        P1OUT ^= LED_GREEN_MSK; // Toggle PIN
        for(i=16000; i>0; i--) {} //  Some delay
        P1OUT ^= LED_RED_MSK; // Toggle PIN
        for(i=16000; i>0; i--) {} //  Some delay
        P1OUT ^= LED_RED_MSK2; // Toggle PIN
        for(i=16000; i>0; i--) {} //  Some delay
        P2OUT ^= LED_RGB_MSK; // Toggle PIN
        for(i=16000; i>0; i--) {} //  Some delay
    }
}
