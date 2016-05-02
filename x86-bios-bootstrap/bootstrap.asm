org 0x7C00

%macro ALIVE 0
	MOV AX, 0xFFFF
	CALL print_ax
%endmacro

; Pointer macros; these point to the location to store some working data
%define da_packet	0x500
%define blkcnt		0x502
%define da_off		0x504
%define da_seg		0x506
%define da_lba		0x508
%define outstr16	0x510
%define status		0x520

; Data macros; these store important constants
%define stack_top 0x7C00
%define gpt_header_location 0x0600
%define gpt_data_location 0x0800
%define gpt_sector_count 32
%define fs_location 0x8000
%define charm_dest_seg 0x4000
%define file_header_buffer 0x7E00

; Program status codes to debugging purposes
%define status_gpt 0x1000
%define status_gptcheck_magic 0x2000
%define status_gptcheck_parts 0x2100
%define status_loadpart 0x3000
%define status_loadelf 0x4000

XOR AX, AX
MOV SP, stack_top
MOV SS, AX

MOV BYTE [da_packet], 0x10
MOV [da_packet + 1], AL
MOV [outstr16 + 4], AL

MOV WORD [status], status_gpt
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Read the GPT from the disk											;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOV SI, tlba
MOV DI, gpt_header_location
MOV CX, gpt_sector_count
XOR BX, BX
CALL read_block

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Verify that a valid GPT had been loaded								;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOV WORD [status], status_gptcheck_magic
MOV SI, gpt_header_location
MOV DI, gpt_magic_num
MOV CX, 8
REPE CMPSB
JNE error

MOV WORD [status], status_gptcheck_parts
SUB SI, 8
MOV AX, [SI + 0x50]
OR AX, AX
JZ error

MOV WORD [status], status_loadpart
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Load first bootable partition to be found								;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOV SI, gpt_data_location
PUSHA
		
MOV DI, efs_magic_num
MOV CX, 16
MOV DX, [SI - 0x200 + 0x50]
_checkboot:
	REPE CMPSB
	JE _isboot
	DEC DX
	JZ error
	ADD SI, 128
	JMP _checkboot
	
_isboot:
ADD SI, 0x20
MOV DI, fslba
MOV CX, 2
REP MOVSD
MOV SI, fslba
MOV DI, fs_location
MOV CX, 1
XOR BX, BX
CALL read_block
	
POPA

MOV WORD [status], status_loadelf
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Load Charm ELF file													;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOV DX, charm_dest_seg
MOV BX, fslba
MOV SI, fs_location
ADD SI, 0x18
_load_next_fh: ; Iterate through file headers until one is found which is marked bootable
	MOV CX, 1
	MOV DI, file_header_buffer
	MOV AX, SI
	CALL print_ax
	CALL read_block ; CRASHES HERE

	MOV SI, boot_marker 
	MOV DI, 0x7E1C
	MOV CX, 4
	REPE CMPSB
	JE _bootable
	MOV SI, file_header_buffer
JMP _load_next_fh	; BUG: If no file is marked bootable, the loop will continue forever
	
_bootable:
MOV SI, file_header_buffer + 0x20
XOR DI, DI
MOV ES, DX
ADD BYTE [BX], 1
MOV CX, [file_header_buffer + 0x10]
CALL read_block

MOV ES, DI ; DI is still zero, so this resets to low memory

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TODO Build program image from ELF										;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOV SI, splash
CALL print_string ; Will show 'y' if the loader works so far

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This should never be reached, it just halts if an error happens		;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
halt:
	HLT
	JMP halt
error:
	MOV AX, [status]
	CALL print_ax
	XOR BX, BX
	MOV SI, error_msg
	CALL print_string
	JMP halt

%include "x86-bios-bootstrap/print.asm"
%include "x86-bios-bootstrap/stdlib.asm"
%include "x86-bios-bootstrap/disk.asm"
;%include "x86-bios-bootstrap/loader.asm"

tlba:	DQ	1
fslba:	DQ	0
error_msg:
		DB	"n", 0
splash:
		DB	"y", 0
hexstr:	DB	"0123456789ABCDEF"
elf_magic_num:
		DB	0x7F, "ELF"
gpt_magic_num:
		DB	"EFI PART"
efs_magic_num:
		DB	"HI THIS IS "
boot_marker:															; boot_marker is just a label in the middle of efs_magic_num,
		DB	"BOOT!"														; in order to save a few bytes. it really reads to 'BOOT'

times 510-($-$$) db 0
db 0x55
db 0xAA
