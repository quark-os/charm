;	SI	String to print
print_string:
	PUSHA
	_print_loop:
		LODSB
		MOV AH, 0x0E
		INT 0x10
		OR AL, AL
		JNZ _print_loop
	POPA
	RET

print_bx:
	PUSHA
	MOV DI, outstr16
	MOV CX, 4   ;four places
	hexloop:
		ROL AX, 4   ;leftmost will
		MOV AX, BX   ; become
		AND AX, 0x0f   ; rightmost index into hexstr
		ADD AL, '0'
					;CMP BL, '9'
					;JLE noshift
					;ADD BL, 7
					;noshift:
					; The code above maps digits A-F properly. Until it can
					; fit in the finished binary, developers will have to
					; interpret the characters between '9' and 'A' themselves:
						; A = ':'
						; B = ';'
						; C = '<'
						; D = '='
						; E = '>'
						; F = '?'
		;MOV [DI], BL
		STOSB
		LOOP hexloop
	MOV SI, outstr16
	CALL print_string
	POPA
	RET
