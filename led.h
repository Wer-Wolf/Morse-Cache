#define RED_LED_PIN PB0 //OC0A
#define GREEN_LED_PIN PB1 //OC0B

#define RED 1
#define GREEN 2
#define BOTH 3

#define set_led(color) PORTB |= (color << RED_LED_PIN);
#define clear_led(color) PORTB &= ~(color << RED_LED_PIN);