#pragma once

#include <avr/wdt.h>
#include <util/atomic.h> //-std=gnu99

#define MS16 0
#define MS32 1
#define MS64 2
#define MS125 3
#define MS250 4
#define MS500 5
#define S1 6
#define S2 7
#define S4 32
#define S8 33

#define wdt_on()  WDTCR |= (1 << WDTIE); //ISR!
#define wdt_off() WDTCR &= ~(1 << WDTIE);

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));

void wdt_reset_handler(void) __attribute__((naked)) __attribute__((section(".init3")));

void wdt_reset_handler(void) {
    mcusr_mirror = MCUSR;
    MCUSR = 0x00;
    WDTCR = (1 << WDCE) | (1 << WDE);
    WDTCR = 0x00;
}

inline void wdt_set(uint8_t time) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        wdt_reset(); //Definierter WDT-Stand
        WDTCR |= (1 << WDCE);
        WDTCR |= time;
    }
}