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
#include "../util/address.h"
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

FUSES = {
	.low = (FUSE_SPIEN & FUSE_EESAVE & FUSE_CKDIV8 & FUSE_SUT0 & FUSE_CKSEL0),
	.high = (FUSE_BODLEVEL0),
};

static volatile uint8_t color = 0;

static volatile uint8_t morse_code = 0;

static inline void sleep() {
	sleep_enable();
	NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
		sleep_cpu();
	}
	sleep_disable();
}

static inline void wait() { //Abhängig vom Watchdog-Timeout und setzt morse_code auf 0!
	wdt_reset(); //Definierter Ausgangszustand
	morse_code = 0;
	//wdt_active() muss zuvor false sein
	wdt_on(); //Beended sich durch morse_code = 0 sofort nach einem Wachtdog-Timeout
	sleep();
}

static inline void morse_code_end() {
	wdt_off();
	wdt_reset(); //Definierter Ausgangszustand
}

ISR(WDT_vect) {
	static uint8_t counter = 0, duty_cycle = 0, counter_max = 0;

	if(counter == 0) {
		if(morse_code <= 1) { //Illegale Daten
			morse_code_end();
			return;
		} else {
			//Benötigt einen Durchlauf zum aktualisieren!
			if(morse_code & (1 << 0)) { //dah
				duty_cycle = DAH;
			} else { //dit
				duty_cycle = DIT;
			}
			morse_code >>= 1;
			if(morse_code == 1) { //Ende des Buchstabens
				counter_max = duty_cycle + DAH;
			} else {
				counter_max = duty_cycle + DIT;
			}
		}
	}
	if(counter < duty_cycle) {
		set_led(color);
	} else {
		clear_led(color);
	}
	counter++;
	if(counter >= counter_max) { //Periode beendet
		if(counter_max - duty_cycle == DAH) {
			morse_code_end();
		}
		counter = 0;
	}
}

int main(void) {
	uint8_t eeprom_data = 0;

	DDRB = (1 << PULLUP_ENABLE_PIN) | (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN);
	power_timer0_disable();
	battery_init();
	input_init();
	wdt_set(MS500);
	while(1) {
		set_sleep_mode(SLEEP_MODE_ADC); //ADC benötigt ADC-Sleep
		battery_start_measuring(); //Benötigt Sleep
		sleep();
		if(battery_level <= eeprom_read_word(BATTERY_CALIBRATION_LOW_ADDRESS)) { //Zu geringe Spannung
			color = RED;
		} else {
			color = GREEN;
		}
		if(calibration_needed()) { //Batteriewarner aktualisieren
			eeprom_write_word(BATTERY_CALIBRATION_LOW_ADDRESS, battery_level);
			sleep(); //Interrupt wird benötigt
			set_led(color);
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			wait();
			clear_led(color);
		} else {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			for(uint8_t eeprom_address = DATA_START_ADDRESS; eeprom_address <= DATA_END_ADDRESS; eeprom_address++) {
				eeprom_data = eeprom_read(eeprom_address);
				if(mcusr_mirror) {//MCU wurde zurückgesetzt, EEPROM checken
					if(is_illegal_data(eeprom_data)) { //Ende oder Fehler
						if(eeprom_data == END_OF_DATA) { //Check erfolgreich
							set_led(color);
							wait();
							clear_led(color);
						}
						break;
					}
				} else { //Morsecode anzeigen
					if(is_illegal_data(eeprom_data)) {
						if(eeprom_data == END_OF_DATA) { //Ende
							break;
						} else {
							continue; //Überspringen bei Fehler
						}
					}
					morse_code = convert_to_morse(eeprom_data - DATA_OFFSET);
					wdt_on();
					do {
						sleep();
					} while(wdt_active());
				}
			}
			mcusr_mirror = 0x00; //Check wird nur einmal durchlaufen
		}
		wait_for_input();
	}
	return 0;
}
