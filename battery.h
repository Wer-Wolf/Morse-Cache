#define PULLUP_ENABLE_PIN PB4
#define ADC_INPUT_PIN PB3

#define START_MEASURING 0
#define GET_RESULT 1

#define SUCCESS 2
#define ERROR 3

uint16_t battery_level = 0;

ISR(ADC_vect) {
    PORTB &= ~(1 << PULLUP_ENABLE_PIN);
    battery_level = ADCL;
    battery_level |= (ADCH << 8);
    ADCSRA &= ~(1 << ADEN);
    PRR |= (1 << PRADC);
}

inline void battery_init() {
    ACSR |= (1 << ACD); //Stromsparen
    DIDR0 |= (1 << ADC3D);
    ADMUX |= (1 << REFS0) | (1 << MUX0) | (1 << MUX1); //1,1V, PB3
    ADCSRA |= (1 << ADCIE) | (1 << ADPS0) | (1 << ADPS1); //150 kHz
    PRR |= (1 << PRADC);
}

uint8_t battery(uint8_t action) {
    switch(action) {
        case START_MEASURING:
            PRR &= ~(1 << PRADC);
            PORTB |= (1 << PULLUP_ENABLE_PIN);
            ADCSRA |= (1 << ADEN) | (1 << ADSC);
            return SUCCES;
        break;
        default: //GET_RESULT
            if(ADCSRA & (1 << ADSC)) { //Busy
                return ERROR;
            } else {
                return SUCCES; //battery_level kann gelesen werden
            }
    }
}