#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/fuse.h>

#include "../lib/wdt.h"
#include "../lib/eeprom.h"

#define INPUT_PIN PB2 //PCINT2
#include "../lib/input.h"

#define F_CPU 1200000UL
#define PULLUP_ENABLE_PIN PB4
#define ADC_INPUT_PIN PB3
#include "../lib/battery.h"

#define RED_LED_PIN PB0 //OC0A
#define GREEN_LED_PIN PB1 //OC0B
#include "../lib/led.h"

#define FINISHED 0
#define RUNNING 1

#define DIT 1
#define DAH 3

FUSES = {
    .low = (FUSE_SPIEN & FUSE_EESAVE & FUSE_CKDIV8 & FUSE_SUT0 & FUSE_CKSEL0),
    .high = (FUSE_BODLEVEL0),
};

extern uint16_t battery_level;

static volatile uint8_t color = 0;
static volatile uint8_t morse_code = 0;
static volatile uint8_t morse_code_status = FINISHED;

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
        if(wdt_counter <= wdt_cycles_on) {
            set_led(color);
        } else {
            clear_led(color);
        }
    }
    if(wdt_counter > wdt_cycles_on + wdt_cycles_off) {
        if(wdt_cycles_off == DAH) {
            morse_code_status = FINISHED;
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
    PRR |= (1 << PRTIM0); //Kein Timmer
    battery_init();
    input_init();
    wdt_set(MS500);
    uint16_t counter;
    sei();
    while(1) {
        wait_for_input();
        battery_start_measuring();
        clear_led(GREEN); //Falls Kalibrierung stattgefunden hat
        while(battery_is_busy()) {
            set_sleep_mode(SLEEP_MODE_ADC);
            sleep_enable();
            sleep_cpu();
            sleep_disable();
        }
        if(check_for_calibration() == CALIBRATION_NEEDED) {
            eeprom_write_word(BATTERY_CALIBRATION_LOW_ADRESS, battery_level);
            set_led(GREEN);
        } else {
            counter = eeprom_read_word(COUNTER_LOW_ADRESS);
            if(battery_level <= eeprom_read_word(BATTERY_CALIBRATION_LOW_ADRESS) || counter == COUNTER_MAX) { //Zu geringe Spannung oder Zähler voll
                color = RED;
            } else {
                eeprom_write_word(COUNTER_LOW_ADRESS, counter + 1);
                color = GREEN;
            }
            do {
                morse_code = eeprom_get_morse_code();
                if(morse_code == END_OF_DATA) {
                    break;
                }
                morse_code_status = RUNNING;
                wdt_on();
                while(morse_code_status != FINISHED) {
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