#pragma once

#ifndef RED_LED_PIN
    #error RED_LED_PIN not defined
    #define RED_LED_PIN PB0 //Übersichtlichere Fehlermeldung
#endif
#ifndef GREEN_LED_PIN
    #error GREEN_LED_PIN not defined
    #define GREEN_LED_PIN PB1 //Übersichtlichere Fehlermeldung
#endif

#define RED 1
#define GREEN 2
#define BOTH 3

#define set_led(color) PORTB |= (color << RED_LED_PIN)
#define clear_led(color) PORTB &= ~(color << RED_LED_PIN)