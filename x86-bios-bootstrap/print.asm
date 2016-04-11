;	SI	String to print
print_string:
	PUSH AX
	PUSH SI
	_print_loop:
		LODSB
		MOV AH, 0x0E
		INT 0x10
		OR AL, AL
		JNZ _print_loop
	POP SI
	POP AX
	RET

print_ax:
	PUSHA
	MOV DI, outstr16
	MOV SI, hexstr
	MOV CX, 4   ;four places
	hexloop:
		ROL AX, 4   ;leftmost will
		MOV BX, AX   ; become
		AND BX, 0x0f   ; rightmost
		MOV BL, [SI + BX];index into hexstr
		MOV [DI], BL
		INC DI
		DEC CX
		JNZ hexloop
	MOV SI, outstr16
	CALL print_string
	POPA
	RET
