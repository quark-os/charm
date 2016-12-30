charm_obj = charm.o entry.o pio.o idtdescriptor.o idt.o lidt.o interrupts.o window.o purevirtual.o interruptcontroller.o \
	picdriver.o isr.o interrupthandlers.o keyboard.o keyboardevent.o keyboardtranslator.o system.o display.o eventlistener.o \
	eventlistenerkey.o console.o
link_script = linker.ld

CXX = i686-elf-g++
CC = i686-elf-g++
CPPFLAGS += -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -fpermissive 

LINKFLAGS = -T $(addprefix src/, $(link_script)) -ffreestanding -nostdlib -lgcc

charm: $(addprefix src/, $(charm_obj))
	$(CXX) $(LINKFLAGS) $(addprefix src/, $(charm_obj)) -o bin/charm
