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

inline void wdt_set(uint8_t time) {
    wdt_reset(); //Definierter WDT-Stand
    WDTCR |= (1 << WDCE); //Muss atomar sein
    WDTCR |= time;
}