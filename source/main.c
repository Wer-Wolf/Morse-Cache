#include <stdint.h> //c99
#include <stdbool.h>

#include <avr/io.h>
#include <avr/signature.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
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

#define reset_occured() (mcusr_mirror & ((1 << WDRF) | (1 << BORF) | (1 << EXTRF) | (1 << PORF)))
//mcusr_mirror von wdt.h

FUSES = {
	.low = (FUSE_SPIEN & FUSE_EESAVE & FUSE_CKDIV8 & FUSE_SUT0 & FUSE_CKSEL0),
	.high = (FUSE_BODLEVEL0),
};

static volatile uint8_t color = 0;

static volatile struct morse_code {
	uint8_t value;
	bool running;
} morse_code = {0, false};

struct watchdog {
	uint8_t counter;
	struct cycles {
		uint8_t active;
		uint8_t idle;
	} cycles;
};

static inline void sleep() {
	sleep_enable();
	NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
		sleep_cpu();
	}
	sleep_disable();
}

static inline void wait() { //Abhängig vom Watchdog-Timeout und setzt morse_code auf 0!
	wdt_reset(); //Definierter Ausgangszustand
	morse_code.value = 0;
	//morse_code.running muss zuvor false sein
	wdt_on(); //Beended sich durch morse_code = 0 sofort nach einem Wachtdog-Timeout
	sleep();
}

static inline void morse_code_end() {
	morse_code.running = false;
	wdt_off();
	wdt_reset(); //Definierter Ausgangszustand
}

ISR(WDT_vect) {
	static struct watchdog wdt = {0, {0, 0}};
	if(wdt.counter == 0) {
		if(morse_code.value <= 1) { //Illegale Daten
			morse_code_end();
			return;
		} else {
			morse_code.running = true; //Start
			//Benötigt einen Durchlauf zum aktualisieren!
			if(morse_code.value & (1 << 0)) { //dah
				wdt.cycles.active = DAH;
			} else { //dit
				wdt.cycles.active = DIT;
			}
			morse_code.value >>= 1;
			if(morse_code.value == 1) { //Ende des Buchstabens
				wdt.cycles.idle = DAH;
			} else {
				wdt.cycles.idle = DIT;
			}
		}
	} else {
		if(wdt.counter <= wdt.cycles.active) {
			set_led(color);
		} else {
			clear_led(color);
		}
	}
	if(wdt.counter >= wdt.cycles.active + wdt.cycles.idle) { //Periode beendet
		if(wdt.cycles.idle == DAH) {
			morse_code_end();
		}
		wdt.counter = 0;
	} else {
		wdt.counter++;
	}
}

int main(void) {
	DDRB = (1 << PULLUP_ENABLE_PIN) | (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN);
	power_timer0_disable();
	battery_init();
	input_init();
	wdt_set(MS500);
	while(1) {
		battery_start_measuring();
		set_sleep_mode(SLEEP_MODE_ADC);
		sleep();
		if(battery_level <= eeprom_read_word(BATTERY_CALIBRATION_LOW_ADRESS)) { //Zu geringe Spannung
			color = RED;
		} else {
			color = GREEN;
		}
		if(reset_occured()) { //EEPROM checken
			mcusr_mirror = 0x00; //Check wird nur einmal ausgeführt
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			uint8_t eeprom_data;
			uint8_t eeprom_adress = DATA_START_ADRESS;
			do {
				eeprom_data = eeprom_read(eeprom_adress);
				if(is_illegal_data(eeprom_data)) { //Ende oder Fehler
					if(eeprom_data == END_OF_DATA) {
						set_led(color); //Check erfolgreich (Ende)
						wait();
						clear_led(color);
					}
					break;
				} else {
					eeprom_adress++;
				}
			} while(eeprom_adress <= DATA_END_ADRESS);
		} else { //Morsecode
			if(check_for_calibration() == CALIBRATION_NEEDED) {
				eeprom_write_word(BATTERY_CALIBRATION_LOW_ADRESS, battery_level);
				sleep(); //Interrupt wird benötigt
				set_led(color);
				wait();
				clear_led(color);
			} else {
				set_sleep_mode(SLEEP_MODE_PWR_DOWN);
				do {
					morse_code.value = eeprom_get_morse_code();
					if(morse_code.value == END_OF_DATA) {
						break;
					}
					wdt_on();
					do {
						sleep(); //morse_code.running aktualisieren
					} while(morse_code.running != false);
				} while(1);
			}
		}
		wait_for_input();
	}
	return 0;
}
