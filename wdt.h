#define MS16 0
#define MS32 1
#define MS64 2
#define S0_125 3
#define S0_250 4
#define S0_5 5
#define S1 6
#define S2 7
#define S4 32
#define S8 33

void wdt_set(uint8_t time) {
    wdt_reset(); //Definierter WDT-Stand
    WDTCR |= (1 << WDCE);
    WDTCR |= time;
}

inline void wdt_on() {
    WDTCR |= (1 << WDTIE); //ISR!
}

inline void wdt_off() {
    WDTCR &= ~(1 << WDTIE);
}