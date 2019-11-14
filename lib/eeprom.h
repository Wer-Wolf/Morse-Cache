#pragma once

#include <util/atomic.h> //c99
#include <avr/interrupt.h>

#define eeprom_is_busy() (EECR & (1 << EEPE))
#define ee_interrupt_is_pending() (EECR & (1 << EERIE))

static volatile struct eeprom {
    uint8_t high_byte;
    uint8_t high_adress;
} eeprom = {0, 0};

//Das Aufrufen einer der Funktionen bei deaktivierten Interrupts darf nur erfolgen, wenn kein EE-Interrupt aussteht

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
    eeprom_write(eeprom.high_adress, eeprom.high_byte);
}

uint16_t eeprom_read_word(uint8_t adress) {
    uint16_t word = eeprom_read(adress + 1);
    word <<= 8;
    word |= eeprom_read(adress);
    return word;
}

void eeprom_write_word(uint8_t adress, uint16_t word) {
    while(ee_interrupt_is_pending()); //Vermeidet Race-Condition
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        eeprom.high_adress = adress + 1; //Wort brauch ZWEI Adressen!
        eeprom.high_byte = (uint8_t) (word >> 8);
        eeprom_write(adress, (uint8_t) word);
        EECR |= (1 << EERIE); //Idle/ADC-Mode
    } //Interrupts müssen eingeschalten werden, um Operation zu beenden
}