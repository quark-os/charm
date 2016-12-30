#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "keyboardevent.h"

class Keyboard
{
public:
		
	static void initialize();
	
	static void sendEvent(KeyboardEvent event);
			
private:

	static void flushData();
	
	static bool waitForEmptyBuffer();

	static bool keyflags[128];
		
};

#endif
