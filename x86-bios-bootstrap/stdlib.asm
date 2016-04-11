;	SI	First operand of compare
;	DI	Second operand to compare
;	CX	Size of block to compare
blkcmp:
	PUSHA
	XOR AX, AX
	_cmploop:
		LODSB
		XCHG SI, DI
		ROL AX, 8
		LODSB
		CMP AL, AH
	LOOPE _cmploop
	OR CX, CX
	JNZ _ne
	STC
	_ne:
	POPA
	RET
