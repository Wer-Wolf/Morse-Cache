#pragma once

#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#if ADC_INPUT_PIN == PB2
	#define MUX_SELECT_BITS 1
	#define DID_VALUE ADC1D
#elif ADC_INPUT_PIN == PB3
	#define MUX_SELECT_BITS 3
	#define DID_VALUE ADC3D
#elif ADC_INPUT_PIN == PB4
	#define MUX_SELECT_BITS 2
	#define DID_VALUE ADC2D
#elif ADC_INPUT_PIN == PB5
	#define MUX_SELECT_BITS 0
	#define DID_VALUE ADC0D
#else
	#error No ADC Input available on selected Pin
	#define MUX_SELECT_BITS 0 //Übersichtlichere Fehlermeldung
#endif

#define battery_is_busy() (ADCSRA & (1 << ADSC)) //Wahr solange Messung läuft

volatile uint16_t battery_level = 0;

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

bool calibration_needed() { //Das Ergebnis einer eventuell laufenden Messung sollte verworfen werden
	PORTB &= ~(1 << PULLUP_ENABLE_PIN); //Falls gerade eine Messung läuft
	DDRB &= ~(1 << PULLUP_ENABLE_PIN);
	uint8_t calibration_pin_state = PINB & (1 << PULLUP_ENABLE_PIN);
	DDRB |= (1 << PULLUP_ENABLE_PIN);
	if(calibration_pin_state) {
		return true;
	} else {
		return false;
	}
}
