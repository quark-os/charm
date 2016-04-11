;	BX	Pointer to offset of fs header on disk
;	SI	Offset of fs header in memory
;	DI	Offset at which to read the file
;	DS	Segment at which to load file to
load_bootprog:
	PUSHA
	PUSH DI
	
	ADD SI, 0x18
	
	_load_next_fh:
		MOV CX, 1
		MOV DI, 0x7E00
		CALL read_block
	
		MOV SI, boot_marker
		MOV DI, 0x7E1B
		MOV CX, 4
		CALL blkcmp
		JC _bootable
		MOV SI, 0x7E00
	JMP _load_next_fh
	
	_bootable:
	MOV SI, 0x7E20
	POP DI
	MOV AX, DS
	MOV ES, AX
	ADD BYTE [BX], 1
	MOV CX, [0x7E10]
	CALL read_block
	
	POPA
	RET
