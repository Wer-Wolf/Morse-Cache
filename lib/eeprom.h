#include <util/atomic.h> //-std=gnu99
#include <avr/interrupt.h>

#define DIRTY_BIT_ADRESS 0x00

#define DATA_START_ADRESS 0x01
#define DATA_END_ADRESS 0x3C

#define COUNTER_ADRESS 0x3D

#define BATTERY_CALIBRATION_LOW_ADRESS 0x3E
#define BATTERY_CALIBRATION_HIGH_ADRESS 0x3F

#define END_OF_DATA 0xFF
#define ILLEGAL_DATA 0
#define DATA_MAX 9

#define COUNTER_MAX 0xFFFF

#define FALSE 0
#define TRUE 1

#define eeprom_is_busy() (EECR & (1 << EEPE))
#define ee_interrupt_is_pending() (EECR & (1 << EERIE))

static volatile uint8_t ee_high_byte = 0;
static volatile uint8_t ee_high_adress = 0;

//Das Aufrufen einer der Funktionen darf nur erfolgen, wenn kein EE-Interrupt aussteht

uint8_t eeprom_read(uint8_t adress) {
    while(ee_interrupt_is_pending());
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        while(eeprom_is_busy());
        EEARL = adress;
        EECR |= (1 << EERE);
    }
    return EEDR;
}

void eeprom_write(uint8_t adress, uint8_t data) {
    while(ee_interrupt_is_pending());
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        while(eeprom_is_busy());
        EEARL = adress;
        EEDR = data;
        EECR |= (1 << EEMPE);
        EECR |= (1 << EEPE);
    }
}

ISR(EE_RDY_vect) {
    EECR &= ~(1 << EERIE); //Power-Down
    eeprom_write(ee_high_adress, ee_high_byte);
}

uint16_t eeprom_read_word(uint8_t adress) {
    uint16_t word = eeprom_read(adress + 1);
    word <<= 8;
    word |= eeprom_read(adress);
    return word;
}

void eeprom_write_word(uint8_t adress, uint16_t word) {
    while(ee_interrupt_is_pending()); //Vermeidet Race-Condition
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        ee_high_adress = adress + 1; //Wort brauch ZWEI Adressen!
        ee_high_byte = (uint8_t) (word >> 8);
        eeprom_write(adress, (uint8_t) word);
        EECR |= (1 << EERIE); //Idle/ADC-Mode
    } //Interrupts werden immer eingeschaltet!
}

uint8_t convert_data_to_morse(uint8_t data) {
    if(data > DATA_MAX) {
        return ILLEGAL_DATA;
    } else {
        return ((uint8_t) ((0xFC1F >> data) & 0x1F)) | (1 << 5);
    }
}

uint8_t eeprom_parse_data(uint8_t adress) {
    uint8_t data = eeprom_read(adress);
    if(data == END_OF_DATA || adress == DATA_END_ADRESS) {
        return END_OF_DATA;
    } else {
        return convert_data_to_morse(data);
    }
}

uint8_t eeprom_get_morse_code() {
    static uint8_t ee_data_adress = DATA_START_ADRESS;
    uint8_t data;
    do {
        data = eeprom_parse_data(ee_data_adress);
        ee_data_adress++;
    } while(data == ILLEGAL_DATA);
    if(data == END_OF_DATA) {
        ee_data_adress = DATA_START_ADRESS;
    }
    return data;
}

/*
Format:

0 = dit (.)
1 = dah (_)

Beispiel:
0 0 1 1 1 1 1 0 = 9
    ^
    Morsecode (. . . _ _ _ _ .)
         ^
         Abschluss des Zeichens (gehöhrt nicht mehr zum Morsecode)

- Von rechts lesen, danach um 1 nach rechts schieben

- Ende wenn gleich 1
*/
