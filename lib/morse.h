#pragma once

#include "../lib/eeprom.h"
#include "../util/adress.h"

#define END_OF_DATA 0x00
#define ILLEGAL_DATA 0xFF
#define DATA_MIN 0x30 //"0"
#define DATA_MAX 0x39 //"9"
#define DATA_OFFSET 0x30

#define is_illegal_data(data) ((data) > DATA_MAX || (data) < DATA_MIN)

#define DIT 1
#define DAH 3

#define convert_to_morse(data) (((uint8_t) (((0xFE0F << (data)) & 0x3E00) >> 9)) | (1 << 5))

uint8_t eeprom_get_morse_code() {
    static uint8_t ee_data_adress = DATA_START_ADRESS;
    uint8_t raw_data;
    uint8_t morse_data;
    do {
        raw_data = eeprom_read(ee_data_adress);
        if(raw_data == END_OF_DATA || ee_data_adress >= DATA_END_ADRESS) { //Ende der Daten
            morse_data = END_OF_DATA;
            ee_data_adress = DATA_START_ADRESS;
        } else {
            if(is_illegal_data(raw_data)) { //Invalid ASCII character
                morse_data = ILLEGAL_DATA;
            } else {
                morse_data = convert_to_morse(raw_data - DATA_OFFSET);
            }
            ee_data_adress++;
        }
    } while(morse_data == ILLEGAL_DATA);
    return morse_data;
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