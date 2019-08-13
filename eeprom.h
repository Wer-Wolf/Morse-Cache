#define DIRTY_BIT_ADRESS 0x00

#define COUNTER_LOW_ADRESS 0x01
#define COUNTER_HIGH_ADRESS 0x02

#define BATTERY_CALIBRATION_LOW_ADRESS 0x03
#define BATTERY_CALIBRATION_HIGH_ADRESS 0x04

volatile uint8_t ee_high_byte = 0;
volatile uint8_t ee_high_adress = 0;

ISR(EE_RDY_vect) {
    eeprom_write(ee_high_byte, ee_high_adress);
    EECR &= ~(1 << EERIE); //Power-Down
}

uint8_t eeprom_read(uint8_t adress) {
    while(EECR & (1 << EEPE));
    EEARL = adress;
    EECR |= (1 << EERE);
    return EEDR;
}

void eeprom_write(uint8_t adress, uint8_t data) {
    while(EECR & (1 << EEPE));
    EEARL = adress;
    EEDR = data;
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}

uint16_t eeprom_read_word(uint8_t adress) {
    uint16_t word = eeprom_read(adress++);
    word <<= 8;
    word |= eeprom_read(adress);
    return word;
}

void eeprom_write_word(uint16_t word, uint8_t adress) { //Interrups zuvor abschalten
    eeprom_write((uint8_t) word, adress);
    ee_high_byte = (uint8_t) (word >> 8);
    if(ee_high_byte != eeprom_read(high_adress)) { //Oberes Byte wird geupdated
        ee_high_adress = adress++; //Wort brauch ZWEI Adressen!
        EECR |= (1 << EERIE); //Idle-Mode
    }
} //Interrupts wieder einschalten*/