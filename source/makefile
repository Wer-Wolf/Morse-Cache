MCU = attiny13a
#avrdude.conf patch needed
#avrdude with .elf support (version 6.x) required
AVR_PROGRAMMER = usbasp
TARGET = main
PORT = /dev/ttyS0
STANDARD = c11

.PHONY: all fuse programm clean size

all: $(TARGET).hex

fuse: $(TARGET).elf
	avr-objdump -s -j .fuse $(TARGET).elf

programm: $(TARGET).elf
	avrdude -c $(AVR_PROGRAMMER) -P $(PORT) -p $(MCU) -u -U flash:w:$(TARGET).elf
	avrdude -c $(AVR_PROGRAMMER) -P $(PORT) -p $(MCU) -u -U lfuse:w:$(TARGET).elf
	avrdude -c $(AVR_PROGRAMMER) -P $(PORT) -p $(MCU) -u -U hfuse:w:$(TARGET).elf
clean:
	rm -f $(TARGET).o
	rm -f $(TARGET).elf
	rm -f $(TARGET).hex

size: $(TARGET).elf
	avr-size --mcu=$(MCU) $(TARGET).elf

$(TARGET).o: $(TARGET).c
	avr-gcc -Os -c -Wall -std=$(STANDARD) -mmcu=$(MCU) -o $(TARGET).o $(TARGET).c

$(TARGET).elf: $(TARGET).o
	avr-gcc -mmcu=$(MCU) -o $(TARGET).elf $(TARGET).o

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex