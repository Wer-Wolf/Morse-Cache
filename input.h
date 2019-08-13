#define INPUT_PIN PB2 //PCINT2

EMPTY_INTERRUPT(PCINT0_vect);

inline void input_init() {
    GIMSK |= (1 << PCIE);
}

void wait_for_input() {
    PCMSK |= (1 << PCINT2);
    set_sleep_mode(SLEEP_MODE_PWR_DWN)
    sleep_enable();
    sleep_bod_disable();
    sei();
    sleep_cpu();
    sleep_disable();
    PCMSK &= ~(1 << PCINT2);
}