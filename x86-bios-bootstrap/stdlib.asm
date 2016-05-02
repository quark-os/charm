;	SI	First operand of compare
;	DI	Second operand to compare
;	CX	Size of block to compare
blkcmp:	
	PUSHA
	REPE CMPSB
	POPA
	RET

;	DI	Location to start zeroing
;	CX	Length of block
blkzero:
	PUSHA
	_zeroloop:
		MOV BYTE [SI], 0
		INC SI
		LOOP _zeroloop
	POPA
	RET
