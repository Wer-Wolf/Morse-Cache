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
    #define DID_VALUE ADC1D
#else
    #if ADC_INPUT_PIN == PB3
        #define MUX_VALUE 3
        #define DID_VALUE ADC3D
    #else
        #if ADC_INPUT_PIN == PB4
            #define MUX_VALUE 2
            #define DID_VALUE ADC2D
        #else
            #if ADC_INPUT_PIN == PB5
                #define MUX_VALUE 0
                #define DID_VALUE ADC0D
            #else
                #error No ADC Input available on selected Pin
                #define MUX_VALUE 0 //Übersichtlichere Fehlermeldung
            #endif
        #endif
    #endif
#endif

#if (F_CPU / 2) >= 50000 && (F_CPU / 2) <= 200000
    #define ADC_PRESCALER_SELECT 1
    //#pragma message("Prescaler: 2")
#else
    #if (F_CPU / 4) >= 50000 && (F_CPU / 4) <= 200000
        #define ADC_PRESCALER_SELECT 2
        //#pragma message("Prescaler: 4")
    #else
        #if (F_CPU / 8) >= 50000 && (F_CPU / 8) <= 200000
            #define ADC_PRESCALER_SELECT 3
            //#pragma message("Prescaler: 8")
        #else
            #if (F_CPU / 16) >= 50000 && (F_CPU / 16) <= 200000
                #define ADC_PRESCALER_SELECT 4
                //#pragma message("Prescaler: 16")
            #else
                #if (F_CPU / 32) >= 50000 && (F_CPU / 32) <= 200000
                    #define ADC_PRESCALER_SELECT 5
                    //#pragma message("Prescaler: 32")
                #else
                    #if (F_CPU / 64) >= 50000 && (F_CPU / 64) <= 200000
                        #define ADC_PRESCALER_SELECT 6
                        //#pragma message("Prescaler: 64")
                    #else
                        #if (F_CPU / 128) >= 50000 && (F_CPU / 128) <= 200000
                            #define ADC_PRESCALER_SELECT 7
                            //#pragma message("Prescaler: 128")
                        #else
                            #error F_CPU too high or too low
                            #define ADC_PRESCALER_SELECT 0 //Übersichtlichere Fehlermeldung
                        #endif
                    #endif
                #endif
            #endif
        #endif
    #endif
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
    DIDR0 |= (1 << DID_VALUE);
    ADMUX |= (1 << REFS0) | (MUX_VALUE << MUX0); //1,1V
    ADCSRA |= (1 << ADIE) | (ADC_PRESCALER_SELECT << ADPS0);
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
    uint8_t calibration_pin_state = PINB & (1 << PULLUP_ENABLE_PIN);
    DDRB |= (1 << PULLUP_ENABLE_PIN);
    if(calibration_pin_state) {
        return CALIBRATION_NEEDED;
    } else {
        return NO_CALIBRATION;
    }
}