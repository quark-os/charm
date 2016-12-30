#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

extern "C"
{
	void test_handler();
	
	void pic_routine();
	
	void keyboard_isr();
	
	void picEOI();
}

#endif
