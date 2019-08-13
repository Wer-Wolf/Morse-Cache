#define RED_LED_PIN PB0 //OC0A
#define GREEN_LED_PIN PB1 //OC0B

#define OFF 0
#define OFF_RED 1
#define OFF_GREEN 2
#define ON_RED 3
#define ON_GREEN 4
#define RED 5
#define GREEN 6

const __flash uint8_t brightness_table[8] = {0, 4, 8, 16, 32, 64, 128, 255}; //Tabelle (const!) im Flash gespeichert --> -std=gnu99

inline void led_init() {
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); //Fast-PWM, SET on TOP
    TCCR0B |= (1 << CSO1) | (1 << CS00); //18750 MHz
    PRR |= (1 << PRTIM0);
}

void set_led(uin8_t color, unit8_t brightness) {
    switch(color) {
        case OFF_RED: //Power-Down
            OC0A = 0;
            DDRB &= ~(1 << RED_LED_PIN);
        break;
        case OFF_GREEN:
            OC0B = 0;
            DDRB &= ~(1 << GREEN_LED_PIN);
        break;
        case ON_RED: //Idle-Mode
            DDRB |= (1 << RED_LED_PIN);
            PRR &= ~(1 << PRTIM0);
        break;
        case ON_GREEN:
            DDRB |= (1 << GREEN_LED_PIN);
            PRR &= ~(1 << PRTIM0);
        break;
        case RED:
            OC0A = brightness;
        break;
        case GREEN:
            OC0B = brightness;
        break;
        default: //OFF, Power-Down
            OC0A = 0;
            OC0B = 0;
            DDRB &= ~((1 << RED_LED_PIN) | (1 << GREEN_LED_PIN));
            PRR |= (1 << PRTIM0);
    }
}