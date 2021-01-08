#pragma once

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>

#ifndef INPUT_PIN
    #error INPUT_PIN not defined
    #define INPUT_PIN PB2 //Übersichtlichere Fehlermeldung
#endif

#define input_pending() (PCMSK & (1 << INPUT_PIN))

#define input_init() GIMSK |= (1 << PCIE)

ISR(PCINT0_vect) {
    PCMSK &= ~(1 << INPUT_PIN);
    PORTB &= ~(1 << INPUT_PIN);
    GIFR = (1 << PCIF); //Eventuelles Prellen kompensieren
}

inline void wait_for_input() { //Interrupts müssen zuvor deaktiviert sein
    PORTB |= (1 << INPUT_PIN); //Kein externer Pullup
    PCMSK |= (1 << INPUT_PIN);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        while(input_pending()) {
            sleep_enable();
            sleep_bod_disable();
            NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
                sleep_cpu();
            }
            sleep_disable();
        }
    }
}