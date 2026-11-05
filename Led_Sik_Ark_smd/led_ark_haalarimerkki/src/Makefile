BIN=main
OBJS=main.o ctrl_functions.o
MCU=atmega328pb

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -DF_CPU=8000000UL -mmcu=${MCU} -Wall -std=c99
PORT=/dev/ttyUSB0

${BIN}.hex: ${BIN}.elf
	${OBJCOPY} -O ihex $< $@

${BIN}.elf: ${OBJS}
	${CC} -mmcu=${MCU} -o $@ $^

install: ${BIN}.hex
	avrdude -v -c arduino -p ${MCU} -P ${PORT} -b 19200 -U flash:w:$<
fuse:
	#lfuse:w:0xf7:m for 16MHz
	#lfuse:w:0xe2:m for 8MHz
	avrdude -c avrisp -p ${MCU} -B3 -P ${PORT} -b 19200 -U lfuse:w:0xef7:m -U hfuse:w:0xd9:m
clean:
	rm -f ${BIN}.elf ${BIN}.hex ${OBJS}



