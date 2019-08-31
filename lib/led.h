#ifndef RED_LED_PIN
    #warning RED_LED_PIN automatically defined as PB0
    #define RED_LED_PIN PB0 //OC0A
#endif
#ifndef GREEN_LED_PIN
    #warning GREEN_LED_PIN automatically defined as PB1
    #define GREEN_LED_PIN PB1 //OC0B
#endif

#define RED 1
#define GREEN 2
#define BOTH 3

#define set_led(color) PORTB |= (color << RED_LED_PIN)
#define clear_led(color) PORTB &= ~(color << RED_LED_PIN)