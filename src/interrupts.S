.intel_syntax noprefix

.global test_handler
test_handler:
	pusha
	movb [0xB8004], 0x24
	popa
	iret
	
.global pic_routine
pic_routine:
	pusha
	movb [0xB8008], 0x24
	call picEOI
	popa
	iret

.global keyboard_isr
keyboard_isr:
	pusha
	call handleKeyboardEvent
	call picEOI
	popa
	iret
