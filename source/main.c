#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/fuse.h>
#include <util/atomic.h>

#include "../lib/wdt.h"
#include "../lib/eeprom.h"
#include "../util/adress.h"
#include "../lib/morse.h"

#define INPUT_PIN PB2 //PCINT2
#include "../lib/input.h"

#define F_CPU 1200000UL
#define PULLUP_ENABLE_PIN PB4
#define ADC_INPUT_PIN PB3
#include "../util/adc_prescaler.h"
#include "../lib/battery.h"

#define RED_LED_PIN PB0 //OC0A
#define GREEN_LED_PIN PB1 //OC0B
#include "../lib/led.h"

#define EXTERNAL_RESET_OCCURED (reset_source & (1 << EXTRF))

#define FINISHED 0
#define RUNNING 1

FUSES = {
    .low = (FUSE_SPIEN & FUSE_EESAVE & FUSE_CKDIV8 & FUSE_SUT0 & FUSE_CKSEL0),
    .high = (FUSE_BODLEVEL0),
};

static volatile uint8_t color = 0;
static volatile uint8_t morse_code = 0;
static volatile uint8_t morse_code_status = FINISHED;

ISR(WDT_vect) {
    static uint8_t wdt_counter = 0;
    static uint8_t wdt_cycles_on = 0;
    static uint8_t wdt_cycles_off = 0;
    if(wdt_counter == 0) {
        if(morse_code <= 1) { //Illegale Daten
            morse_code_status = FINISHED;
            wdt_off();
        } else {
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
        }
    } else {
        if(wdt_counter <= wdt_cycles_on) {
            set_led(color);
        } else {
            clear_led(color);
        }
    }
    if(wdt_counter > wdt_cycles_on + wdt_cycles_off) { //Periode beendet
        if(wdt_cycles_off == DAH) {
            morse_code_status = FINISHED;
            wdt_off();
        }
        wdt_counter = 0;
    } else {
        wdt_counter++;
    }
}

void sleep() {
    sleep_enable();
    NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
        sleep_cpu();
    }
    sleep_disable();
}

int main(void) {
    uint8_t reset_source = MCUSR;
    MCUSR = 0;
    DDRB |= (1 << PULLUP_ENABLE_PIN) | (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN);
    PRR |= (1 << PRTIM0); //Kein Timer
    battery_init();
    input_init();
    wdt_set(MS500);
    if(EXTERNAL_RESET_OCCURED) { //EEPROM checken
        color = RED;
        uint8_t eeprom_data;
        uint8_t eeprom_adress = DATA_START_ADRESS;
        do {
            eeprom_data = eeprom_read(eeprom_adress);
            if(eeprom_data > DATA_MAX) { //Ende oder Fehler
                if(eeprom_data == END_OF_DATA) {
                    color = GREEN;
                } //Sonst Fehler (RED)
                break;
            } else {
                eeprom_adress++;
            }
        } while(eeprom_adress <= DATA_END_ADRESS);
        set_led(color);
        morse_code_status = RUNNING;
        wdt_on(); //Beended sich durch morse_code = 0 sofort
        sleep();
        wdt_reset();
        clear_led(color);
    }
    while(1) {
        wait_for_input();
        battery_start_measuring();
        clear_led(GREEN); //Falls Kalibrierung stattgefunden hat
        set_sleep_mode(SLEEP_MODE_ADC);
        sleep();
        if(check_for_calibration() == CALIBRATION_NEEDED) {
            eeprom_write_word(BATTERY_CALIBRATION_LOW_ADRESS, battery_level);
            sleep();
            set_led(GREEN);
        } else {
            if(battery_level <= eeprom_read_word(BATTERY_CALIBRATION_LOW_ADRESS)) { //Zu geringe Spannung
                color = RED;
            } else {
                color = GREEN;
            }
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            do {
                morse_code = eeprom_get_morse_code();
                if(morse_code == END_OF_DATA) {
                    break;
                }
                morse_code_status = RUNNING;
                wdt_on();
                while(morse_code_status != FINISHED) {
                    sleep();
                }
            } while(1);
        } 
    }
    return 0;
}