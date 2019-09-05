#include <avr/interrupt.h>
#include <util/atomic.h>

#ifndef INPUT_PIN
    #warning INPUT_PIN automatically defined as PB2
    #define INPUT_PIN PB2 //PCINT2
#endif

ISR(PCINT0_vect) {
    PCMSK &= ~(1 << INPUT_PIN);
    PORTB &= ~(1 << INPUT_PIN);
}

#define input_pending() (PCMSK & (1 << INPUT_PIN))

#define input_init() GIMSK |= (1 << PCIE)

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