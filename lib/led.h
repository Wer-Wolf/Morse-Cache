#pragma once

#define RED 1
#define GREEN 2
#define BOTH 3

#define set_led(color) PORTB |= (color << RED_LED_PIN)
#define clear_led(color) PORTB &= ~(color << RED_LED_PIN)
