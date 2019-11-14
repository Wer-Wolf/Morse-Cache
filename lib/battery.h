#pragma once

#include <avr/interrupt.h>
#include <avr/power.h>

#ifndef ADC_PRESCALER_SELECT_BITS
    #error ADC_PRESCALER_SELECT_BITS not defined
    #define ADC_PRESACLER_SELECT_BITS 0 //Übersichtlichere Fehlermeldung
#endif
#ifndef PULLUP_ENABLE_PIN
    #error PULLUP_ENABLE_PIN not defined
    #define PULLUP_ENABLE_PIN PB4 //Übersichtlichere Fehlermeldung
#endif
#ifndef ADC_INPUT_PIN
    #error ADC_INPUT_PIN not defined
    #define ADC_INPUT_PIN PB3 //Übersichtlichere Fehlermeldung
#endif
#if ADC_INPUT_PIN == PB2
    #define MUX_SELECT_BITS 1
    #define DID_VALUE ADC1D
#else
    #if ADC_INPUT_PIN == PB3
        #define MUX_SELECT_BITS 3
        #define DID_VALUE ADC3D
    #else
        #if ADC_INPUT_PIN == PB4
            #define MUX_SELECT_BITS 2
            #define DID_VALUE ADC2D
        #else
            #if ADC_INPUT_PIN == PB5
                #define MUX_SELECT_BITS 0
                #define DID_VALUE ADC0D
            #else
                #error No ADC Input available on selected Pin
                #define MUX_SELECT_BITS 0 //Übersichtlichere Fehlermeldung
            #endif
        #endif
    #endif
#endif

#define battery_is_busy() (ADCSRA & (1 << ADSC)) //Wahr solange Messung läuft

volatile uint16_t battery_level = 0;

enum check {CALIBRATION_NEEDED, NO_CALIBRATION}; //-fshort-enums

ISR(ADC_vect) {
    PORTB &= ~(1 << PULLUP_ENABLE_PIN);
    battery_level = ADC;
    ADCSRA &= ~(1 << ADEN);
    power_adc_disable(); 
}

inline void battery_init() {
    ACSR |= (1 << ACD); //Stromsparen
    DIDR0 |= (1 << DID_VALUE);
    ADMUX = (1 << REFS0) | (MUX_SELECT_BITS << MUX0); //1,1V
    ADCSRA = (1 << ADIE) | (ADC_PRESCALER_SELECT_BITS << ADPS0);
    power_adc_disable();
}

void battery_start_measuring() {
    if(!battery_is_busy()) { //Messung wird nicht unnötig neugestarted
        power_adc_enable();
        PORTB |= (1 << PULLUP_ENABLE_PIN);
        ADCSRA |= (1 << ADEN) | (1 << ADSC);
    }
}

enum check check_for_calibration() { //Das Ergebnis einer eventuell laufenden Messung sollte verworfen werden
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