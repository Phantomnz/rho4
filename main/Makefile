# ===================================================================
# Makefile for ELEC Lab p4
#
# Targets:
# - make flash-p:   Compiles and flashes the P-only test (control.c)
# - make flash-pid: Compiles and flashes the full PID controller (pid.c)
# - make clean:     Removes compiled files
# ===================================================================

# --- Configuration ---
MCU = atmega644p
F_CPU = 12000000
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
DUDE_FLAGS = -c usbasp -p m644p

# --- Compiler Flags ---
# Both files need the floating point printf and math libraries
# (-Wl,-u,vfprintf -lprintf_flt -lm)
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os
LDFLAGS = -Wl,-u,vfprintf -lprintf_flt -lm

# --- Target for P-Controller (control.c) ---
flash-p: prog-p.hex
	$(AVRDUDE) $(DUDE_FLAGS) -U flash:w:prog-p.hex

prog-p.hex: prog-p.elf
	$(OBJCOPY) -O ihex prog-p.elf prog-p.hex

prog-p.elf: control.c
	$(CC) $(CFLAGS) $(LDFLAGS) control.c -o prog-p.elf

# --- Target for Full PID Controller (pid.c) ---
flash-pid: prog-pid.hex
	$(AVRDUDE) $(DUDE_FLAGS) -U flash:w:prog-pid.hex

prog-pid.hex: prog-pid.elf
	$(OBJCOPY) -O ihex prog-pid.elf prog-pid.hex

prog-pid.elf: pid.c
	$(CC) $(CFLAGS) $(LDFLAGS) pid.c -o prog-pid.elf

# --- Clean Target ---
clean:
	rm -f prog-p.elf prog-p.hex prog-pid.elf prog-pid.hex