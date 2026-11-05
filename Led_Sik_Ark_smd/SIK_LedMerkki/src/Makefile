BIN=main
OBJS=main.o ctrl_functions.o
MCU_avrdude=t861
MCU_gcc=attiny861a

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -DF_CPU=8000000UL -mmcu=${MCU_gcc} -Wall -std=c99
PORT=/dev/ttyUSB0

${BIN}.hex: ${BIN}.elf
	${OBJCOPY} -O ihex $< $@

${BIN}.elf: ${OBJS}
	${CC} -mmcu=${MCU_gcc} -o $@ $^

install: ${BIN}.hex
	avrdude -v -c arduino -p ${MCU_avrdude} -P ${PORT} -b 19200 -U flash:w:$<
fuse:
	avrdude -c avrisp -p t861 -B3 -P ${PORT} -b 19200 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m
clean:
	rm -f ${BIN}.elf ${BIN}.hex ${OBJS}




