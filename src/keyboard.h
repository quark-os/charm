#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BUFFER_INPUT 0x02
#define BUFFER_OUTPUT 0x01

#include "keyboardevent.h"

class Keyboard
{
public:
		
	static void initialize();
	
	static void sendEvent(KeyboardEvent event);
	
	static void scan();
			
private:

	static uint8_t readByte();

	static void flushData();
	
	static bool waitForBuffer(uint8_t buffer, bool state);
	
	static bool waitForAck();

	static bool keyflags[128];
		
};

#endif
