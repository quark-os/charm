;	BX	Pointer to quad to add onto LBA
;	CX	Number of blocks to read
;	SI	Pointer to LBA to start read at
;	DI	Pointer to read to
;	ES	Segment to read to
read_block:
	PUSHA
	
	MOV AX, ES
	MOV [blkcnt], CX
	MOV [da_off], DI
	MOV [da_seg], AX
	MOV CX, 4
	MOV DI, da_lba
	REP MOVSW															; Fill data packet
	
	OR BX, BX
	JZ _nooff
	MOV AX, [BX]
	ADD [da_lba], AX
	MOV AX, [BX + 2]
	ADC [da_lba + 2], AX
	MOV AX, [BX + 4]
	ADC [da_lba + 4], AX
	MOV AX, [BX + 6]
	ADC [da_lba + 6], AX
	
	_nooff:
	
	MOV AX, [da_lba + 6]
	CALL print_ax
	MOV AX, [da_lba + 4]
	CALL print_ax
	MOV AX, [da_lba + 2]
	CALL print_ax
	MOV AX, [da_lba]
	CALL print_ax
	
	MOV SI, da_packet
	MOV AH, 0x42
	MOV DL, 0x80
	INT 0x13
	
	JC error
	
	POPA
	RET
