charm_obj = charm.o entry.o
link_script = linker.ld

CXX = i686-elf-g++
CC = i686-elf-g++
CPPFLAGS += -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -fpermissive 

LINKFLAGS = -T $(addprefix src/, $(link_script)) -ffreestanding -nostdlib -lgcc

charm: $(addprefix src/, $(charm_obj))
	$(CXX) $(LINKFLAGS) $(addprefix src/, $(charm_obj)) -o bin/charm