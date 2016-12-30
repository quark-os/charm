#include "keyboard.h"
#include "keyboardtranslator.h"
#include "pio.h"

bool Keyboard::keyflags[128];

void Keyboard::initialize()
{
	// For now, do nothing
}

void Keyboard::sendEvent(KeyboardEvent event)
{
	keyflags[event.getKeycode()] = event.isPressed();
	char* vga = (char*) 0xB8000;
	vga[16] = KeyboardTranslator::keycodeToAscii(event.getKeycode());
}


