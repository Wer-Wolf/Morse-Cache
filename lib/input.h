#include <avr/interrupt.h>

#define INPUT_PIN PB2 //PCINT2

EMPTY_INTERRUPT(PCINT0_vect);

#define input_init() GIMSK |= (1 << PCIE)

void wait_for_input() {
    PORTB |= (1 << INPUT_PIN); //Kein externer Pullup
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_bod_disable();
    PCMSK |= (1 << PCINT2); //Keine Race-Condition
    sleep_cpu();
    sleep_disable();
    PCMSK &= ~(1 << PCINT2);
    PORTB &= ~(1 << INPUT_PIN);
}