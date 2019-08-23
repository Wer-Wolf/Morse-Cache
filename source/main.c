#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/fuse.h>

#include "../lib/wdt.h"
#include "../lib/eeprom.h"
#include "../lib/input.h"
#include "../lib/battery.h"
#include "../lib/led.h"

#define FALSE 0
#define TRUE 1

#define DIT 1
#define DAH 3

extern uint16_t battery_level;

static volatile uint8_t color = 0;
static volatile uint8_t morse_code = 0;
static volatile uint8_t morse_code_finished = TRUE;

ISR(WDT_vect) {
    static uint8_t wdt_counter = 0;
    static uint8_t wdt_cycles_on = 0;
    static uint8_t wdt_cycles_off = 0;
    if(wdt_counter == 0) {
        if(morse_code & (1 << 0)) { //dah
            wdt_cycles_on = DAH;
        } else { //dit
            wdt_cycles_on = DIT;
        }
        morse_code >>= 1;
        if(morse_code == 1) { //Ende des Buchstabens
            wdt_cycles_off = DAH;
        } else {
            wdt_cycles_off = DIT;
        }
    } else {
        if(wdt_counter < wdt_cycles_on) {
            set_led(color);
        } else {
            clear_led(color);
        }
    }
    if(wdt_counter > wdt_cycles_on + wdt_cycles_off) {
        if(wdt_cycles_off == DAH) {
            morse_code_finished = TRUE;
            wdt_off();
        } else {
            wdt_counter = 0;
        }
    } else {
        wdt_counter++;
    }
}

int main(void) {
    DDRB |= (1 << PULLUP_ENABLE_PIN) | (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN);
    wdt_set(MS500);
    input_init();
    battery_init();
    uint16_t counter;
    sei();
    while(1) {
        wait_for_input();
        battery_start_measuring();
        while(battery_is_busy()) {
            set_sleep_mode(SLEEP_MODE_ADC);
            sleep_enable();
            sleep_cpu();
            sleep_disable();
        }
        if(check_for_calibration() == CALIBRATION_NEEDED) {
            cli();
            eeprom_write_word(battery_level, BATTERY_CALIBRATION_LOW_ADRESS);
            sei();
            set_led(GREEN);
        } else {
            counter = eeprom_read_word(COUNTER_LOW_ADRESS);
            if(battery_level <= eeprom_read_word(BATTERY_CALIBRATION_LOW_ADRESS) || counter == COUNTER_MAX) { //Zu geringe Spannung oder Zähler voll
                color = RED;
            } else {
                cli();
                eeprom_write_word(counter++, COUNTER_LOW_ADRESS);
                sei();
                color = GREEN;
            }
            do {
                morse_code = eeprom_get_morse_code();
                if(morse_code == END_OF_DATA) {
                    break;
                }
                morse_code_finished = FALSE;
                wdt_on();
                while(morse_code_finished != TRUE) {
                    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
                    sleep_enable();
                    sleep_cpu();
                    sleep_disable();
                }
            } while(1);
        } 
    }
    return 0;
}