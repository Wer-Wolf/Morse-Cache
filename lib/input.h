#include <avr/interrupt.h>

#ifndef INPUT_PIN
    #warning INPUT_PIN automatically defined as PB2
    #define INPUT_PIN PB2 //PCINT2
#endif

EMPTY_INTERRUPT(PCINT0_vect);

#define input_init() GIMSK |= (1 << PCIE)

void wait_for_input() {
    PORTB |= (1 << INPUT_PIN); //Kein externer Pullup
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_bod_disable();
    PCMSK |= (1 << INPUT_PIN); //Keine Race-Condition
    sleep_cpu();
    sleep_disable();
    PCMSK &= ~(1 << INPUT_PIN);
    PORTB &= ~(1 << INPUT_PIN);
}