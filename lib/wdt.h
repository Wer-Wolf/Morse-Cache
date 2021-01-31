#pragma once

#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/atomic.h> //c99

enum watchdog_timeout_values {S8 = 33, S4 = 32, S2 = 7, S1 = 6,
	MS500 = 5, MS250 = 4, MS125 = 3, MS64 = 2, MS32 = 1, MS16 = 0}; //-fshort-enums

#define wdt_on()  WDTCR |= (1 << WDTIE)
#define wdt_off() WDTCR &= ~(1 << WDTIE)

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));

void wdt_reset_handler(void) __attribute__((naked)) __attribute__((section(".init3")));

void wdt_reset_handler(void) { //Empfehlung des Datenblatts
	mcusr_mirror = MCUSR;
	MCUSR = 0x00;
	WDTCR = (1 << WDCE) | (1 << WDE);
	WDTCR = 0x00;
}

extern ISR(WDT_vect); //ISR wird benötigt!

inline void wdt_set(enum watchdog_timeout_values time) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		wdt_reset(); //Definierter WDT-Stand
		WDTCR |= (1 << WDCE);
		WDTCR |= time;
	}
}
