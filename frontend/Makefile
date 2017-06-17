build:
	mkdir build
	avr-gcc -Os -mmcu=attiny13a main.c -o build/firmware.elf
	avr-objcopy -j .text -O ihex build/firmware.elf build/firmware.hex
clean:
	rm -r build
burn:
	avrdude -c usbasp -p t13 -U flash:w:build/firmware.hex
all:
	make clean
	make build
	make burn
