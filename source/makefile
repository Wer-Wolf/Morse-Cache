MCU = attiny13a #avrdude.conf patch needed
AVR_PROGRAMMER = usbasp
TARGET = main
INCLUDES = ../lib/battery.h ../lib/eeprom.h ../lib/input.h ../lib/led.h ../lib/morse.h ../lib/wdt.h
INCLUDE += ../util/adc_prescaler.h ../util/address.h
PORT = /dev/ttyS0
STANDARD = c11
ADDITION = -fshort-enums #Important for code-size!

.PHONY: all hex fuse programm clean size

all: $(TARGET).elf

hex: $(TARGET).hex

fuse: $(TARGET).elf
	avr-objdump -s -j .fuse $(TARGET).elf

program: $(TARGET).elf #avrdude with .elf support (version 6.x) required
	avrdude -c $(AVR_PROGRAMMER) -P $(PORT) -p $(MCU) -u -U flash:w:$(TARGET).elf
	avrdude -c $(AVR_PROGRAMMER) -P $(PORT) -p $(MCU) -u -U lfuse:w:$(TARGET).elf
	avrdude -c $(AVR_PROGRAMMER) -P $(PORT) -p $(MCU) -u -U hfuse:w:$(TARGET).elf
clean:
	rm -f $(TARGET).o
	rm -f $(TARGET).elf
	rm -f $(TARGET).hex

size: $(TARGET).elf
	avr-size --mcu=$(MCU) $(TARGET).elf

$(TARGET).o: $(TARGET).c $(INCLUDES)
	avr-gcc -Os -c -Wall -std=$(STANDARD) $(ADDITION) -mmcu=$(MCU) -o $(TARGET).o $(TARGET).c

$(TARGET).elf: $(TARGET).o
	avr-gcc -mmcu=$(MCU) -o $(TARGET).elf $(TARGET).o

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex
