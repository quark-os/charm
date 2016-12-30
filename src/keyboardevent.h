#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

#include <stdint.h>

class KeyboardEvent
{
public:

	KeyboardEvent();
		
	KeyboardEvent(uint8_t keycode, bool state);
	
	uint8_t getKeycode();
	
	bool isPressed();
			
private:

	uint8_t keycode;
	
	bool state;
		
};

#endif
