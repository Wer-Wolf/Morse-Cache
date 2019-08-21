#define DIRTY_BIT_ADRESS 0x00

#define DATA_START_ADRESS 0x01
#define DATA_END_ADRESS 0x3B

#define COUNTER_LOW_ADRESS 0x3C
#define COUNTER_HIGH_ADRESS 0x3D

#define BATTERY_CALIBRATION_LOW_ADRESS 0x3E
#define BATTERY_CALIBRATION_HIGH_ADRESS 0x3F

#define END_OF_DATA 0xFF
#define ILLEGAL_DATA 0
#define DATA_MAX 9

#define COUNTER_MAX 0xFFFF

#define eeprom_is_busy() (EECR & (1 << EEPE))

volatile uint8_t ee_high_byte = 0;
volatile uint8_t ee_high_adress = 0;

uint8_t ee_data_adress = DATA_START_ADRESS;

uint8_t eeprom_read(uint8_t adress) {
    while(eeprom_is_busy());
    EEARL = adress;
    EECR |= (1 << EERE);
    return EEDR;
}

void eeprom_write(uint8_t adress, uint8_t data) {
    while(eeprom_is_busy());
    EEARL = adress;
    EEDR = data;
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}

ISR(EE_RDY_vect) {
    eeprom_write(ee_high_byte, ee_high_adress);
    EECR &= ~(1 << EERIE); //Power-Down
}

uint16_t eeprom_read_word(uint8_t adress) {
    uint16_t word = eeprom_read(adress++);
    word <<= 8;
    word |= eeprom_read(adress);
    return word;
}

void eeprom_write_word(uint16_t word, uint8_t adress) { //Interrups zuvor abschalten
    ee_high_adress = adress++; //Wort brauch ZWEI Adressen!
    ee_high_byte = (uint8_t) (word >> 8);
    EECR |= (1 << EERIE); //Idle/ADC-Mode
    eeprom_write((uint8_t) word, adress);
} //Interrupts wieder einschalten*/

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
Morse-Code Format:

. = dit (0)
_ = dah (1)

. . _ _ _ _ _ . = 9

- Von rechts lesen, danach um 1 nach rechts schieben

- Ende wenn gleich 1

*/
