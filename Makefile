charm_obj = charm.o entry.o
link_script = linker.ld
bootstrap_src = bootstrap.asm fs.asm util.asm

CC = i686-elf-gcc
CFLAGS += -ffreestanding -nostdlib -Wall -Wextra -lgcc -T $(addprefix charm/, $(link_script))

charm: $(addprefix charm/, $(charm_obj))
	$(CC) $(CFLAGS) $(addprefix charm/, $(charm_obj)) -o bin/charm

.PHONY: x86-bios-bootstrap
x86-bios-bootstrap: #$(addprefix x86-bios-bootstrap/, $(bootstrap_src))
	nasm x86-bios-bootstrap/bootstrap.asm -o bin/x86-bios-bootstrap -O0
	mkefs ~/quark -o bin/boot.img -b charm
	mksysimg bin/boot.img -m bin/x86-bios-bootstrap -o bin/sys.img
