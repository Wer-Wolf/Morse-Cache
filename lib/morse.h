#pragma once

#include "../lib/eeprom.h"
#include "../util/adress.h"

#define END_OF_DATA 0x00
#define DATA_MIN 0x30 //"0"
#define DATA_MAX 0x39 //"9"
#define DATA_OFFSET 0x30

#define is_illegal_data(data) ((data) > DATA_MAX || (data) < DATA_MIN)

#define DIT 1
#define DAH 3

#define convert_to_morse(data) (((uint8_t) (((0xFE0F << (data)) & 0x3E00) >> 9)) | (1 << 5))

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
