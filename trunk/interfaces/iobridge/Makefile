# Name: Makefile
# Project: raspberry webradio io bridge

DEVICE  = attiny2313
F_CPU   = 8000000L # in Hz
AVRDUDE = sudo avrdude -c usbasp -p $(DEVICE) -B 8 # edit this line for your programmer

CFLAGS  = -I. -O1 -DDEBUG_LEVEL=0 -B /usr/avr/lib -I /usr/avr/include
OBJECTS = main.o

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build main.hex"
	@echo "make flash ..... to flash the firmware (use this on metaboard)"
	@echo "make clean ..... to delete objects and hex file"
	@echo "make fuses ..... set the fuses"

hex: main.hex

fuses:
	$(AVRDUDE) -U lfuse:w:0xe4:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m


# rule for uploading firmware:
flash: main.hex
	$(AVRDUDE) -U flash:w:main.hex:i

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.elf *.o main.s 

# Generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(COMPILE) -S $< -o $@

# file targets:

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf main.s
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size main.hex

main.s: main.c
	$(COMPILE) -S main.c -o main.s

# debugging targets:

disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
