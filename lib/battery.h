#include <avr/interrupt.h>

#ifndef PULLUP_ENABLE_PIN
    #warning PULLUP_ENABLE_PIN automatically defined as PB4
    #define PULLUP_ENABLE_PIN PB4
#endif
#ifndef ADC_INPUT_PIN
    #warning ADC_INPUT_PIN automatically defined as PB3
    #define ADC_INPUT_PIN PB3
#endif
#if ADC_INPUT_PIN == PB2
    #define MUX_VALUE 1
#endif
#if ADC_INPUT_PIN == PB3
    #define MUX_VALUE 3
#endif
#if ADC_INPUT_PIN == PB4
    #define MUX_VALUE 2
#endif
#if ADC_INPUT_PIN == PB5
    #define MUX_VALUE 0
#endif
#if ADC_INPUT_PIN == PB1 || ADC_INPUT_PIN == PB0
    #error No ADC Input available on selected Pin (PB0 or PB1)
    #define MUX_VALUE 0
#endif

#define CALIBRATION_NEEDED 0
#define NO_CALIBRATION 1

#define battery_is_busy() (ADCSRA & (1 << ADSC)) //Wahr solange Messung läuft

uint16_t battery_level = 0;

ISR(ADC_vect) {
    PORTB &= ~(1 << PULLUP_ENABLE_PIN);
    battery_level = ADCL;
    battery_level |= ((uint16_t) ADCH << 8);
    ADCSRA &= ~(1 << ADEN);
    PRR |= (1 << PRADC);
}

inline void battery_init() {
    ACSR |= (1 << ACD); //Stromsparen
    DIDR0 |= (1 << ADC3D);
    ADMUX |= (1 << REFS0) | (MUX_VALUE << MUX0); //1,1V, PB3
    ADCSRA |= (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1); //150 kHz
    PRR |= (1 << PRADC);
}

void battery_start_measuring() {
    if(!battery_is_busy()) { //Messung wird nicht unnötig neugestarted
        PRR &= ~(1 << PRADC);
        PORTB |= (1 << PULLUP_ENABLE_PIN);
        ADCSRA |= (1 << ADEN) | (1 << ADSC);
    }
}

uint8_t check_for_calibration() { //Das Ergebnis einer eventuell laufenden Messung sollte verworfen werden
    PORTB &= ~(1 << PULLUP_ENABLE_PIN); //Falls gerade eine Messung läuft
    DDRB &= ~(1 << PULLUP_ENABLE_PIN);
    uint8_t temp = PINB & (1 << PULLUP_ENABLE_PIN);
    DDRB |= (1 << PULLUP_ENABLE_PIN);
    if(temp) {
        return CALIBRATION_NEEDED;
    } else {
        return NO_CALIBRATION;
    }
}