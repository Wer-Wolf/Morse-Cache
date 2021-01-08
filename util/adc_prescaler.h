#pragma once

#if (F_CPU / 2) >= 50000 && (F_CPU / 2) <= 200000
	#define ADC_PRESCALER_SELECT_BITS 1
	//#pragma message("Prescaler: 2")
#elif (F_CPU / 4) >= 50000 && (F_CPU / 4) <= 200000
	#define ADC_PRESCALER_SELECT_BITS 2
	//#pragma message("Prescaler: 4")
#elif (F_CPU / 8) >= 50000 && (F_CPU / 8) <= 200000
        #define ADC_PRESCALER_SELECT_BITS 3
        //#pragma message("Prescaler: 8")
#elif (F_CPU / 16) >= 50000 && (F_CPU / 16) <= 200000
        #define ADC_PRESCALER_SELECT_BITS 4
        //#pragma message("Prescaler: 16")
#elif (F_CPU / 32) >= 50000 && (F_CPU / 32) <= 200000
        #define ADC_PRESCALER_SELECT_BITS 5
	//#pragma message("Prescaler: 32")
#elif (F_CPU / 64) >= 50000 && (F_CPU / 64) <= 200000
	#define ADC_PRESCALER_SELECT_BITS 6
        //#pragma message("Prescaler: 64")
#elif (F_CPU / 128) >= 50000 && (F_CPU / 128) <= 200000
        #define ADC_PRESCALER_SELECT_BITS 7
        //#pragma message("Prescaler: 128")
#else
        #error F_CPU too high or too low
        #define ADC_PRESCALER_SELECT_BITS 0 //Übersichtlichere Fehlermeldung
#endif
