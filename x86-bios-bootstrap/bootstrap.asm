org 0x7C00

XOR AX, AX
MOV SP, 0x7C00
MOV SS, AX

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

MOV AX, 0x1000
MOV DS, AX
MOV BX, fslba
MOV SI, 0x8000
MOV DI, 0x0000
CALL load_bootprog

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
hexstr:	DB	"0123456789ABCDEF"
outstr16:
		DB	0, 0, 0, 0, 0
da_packet:
		DB	0x10
		DB	0x00
blkcnt:	DW	0x0000
da_off:	DW	0x0000
da_seg:	DW	0x0000
da_lba:	DQ	0x00000000
gpt_magic_num:
		DB	"EFI PART"
efs_magic_num:
		DB	"HI THIS IS "
boot_marker:															; boot_marker is just a label in the middle of efs_magic_num,
		DB	"BOOT!"														; in order to save a few bytes. it really reads to 'BOOT'

times 510-($-$$) db 0
db 0x55
db 0xAA
