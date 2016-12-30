#include "keyboardevent.h"

KeyboardEvent::KeyboardEvent()
{
	
}

KeyboardEvent::KeyboardEvent(uint8_t keycode, bool state)
{
	this->keycode = keycode;
	this->state = state;
}

uint8_t KeyboardEvent::getKeycode()
{
	return keycode;
}

bool KeyboardEvent::isPressed()
{
	return state;
}

