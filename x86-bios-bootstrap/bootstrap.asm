org 0x7C00

%define da_packet	0x500
%define blkcnt		0x502
%define da_off		0x504
%define da_seg		0x506
%define da_lba		0x508
%define outstr16	0x510

XOR AX, AX
MOV SP, 0x7C00
MOV SS, AX

MOV BYTE [da_packet], 0x10
MOV [da_packet + 1], AL
MOV [outstr16 + 4], AL

MOV SI, tlba
MOV DI, 0x0600
MOV CX, 32
MOV AX, 0
MOV ES, AX
XOR BX, BX
CALL read_block

MOV SI, 0x0600
CALL verify_gpt

MOV SI, 0x0800
MOV DI, 0x8000
CALL load_boot_part

MOV DX, 0x1000
MOV BX, fslba
MOV SI, 0x8000
MOV DI, 0x0000
CALL load_bootprog

MOV SI, splash
CALL print_string

halt:
	HLT
	JMP halt
error:
	CALL print_ax
	XOR BX, BX
	MOV SI, error_msg
	CALL print_string
	JMP halt

%include "x86-bios-bootstrap/print.asm"
%include "x86-bios-bootstrap/stdlib.asm"
%include "x86-bios-bootstrap/disk.asm"
%include "x86-bios-bootstrap/partmgmt.asm"
%include "x86-bios-bootstrap/fs.asm"

tlba:	DQ	1
fslba:	DQ	0
error_msg:
		DB	"error.", 0
splash:
		DB	"LOADED CHARM", 0
hexstr:	DB	"0123456789ABCDEF"
gpt_magic_num:
		DB	"EFI PART"
efs_magic_num:
		DB	"HI THIS IS "
boot_marker:															; boot_marker is just a label in the middle of efs_magic_num,
		DB	"BOOT!"														; in order to save a few bytes. it really reads to 'BOOT'

times 510-($-$$) db 0
db 0x55
db 0xAA
