#include "interrupts.h"
#include "pio.h"
#include "keyboardtranslator.h"
#include "keyboardevent.h"
#include "keyboard.h"

extern "C"
{
	
	void handleKeyboardEvent()
	{
		char* vga = (char*) 0xB8000;
		
		char scancode[8] = {0};
		int scancodeLength = 0;
		bool pressed = true;
		
		scancode[0] = inb(0x60);
		vga[10] = '~';
		if(scancode[0] == 0xF0)
		{
			vga[10] = '#';
			scancode[1] = inb(0x60);
			pressed = false;
			scancodeLength++;
		}
		else if(scancode[0] == 0xE0)
		{
			vga[12] = '@';
			scancode[1] = inb(0x60);
			scancodeLength++;
			if(scancode[1] == 0xF0)
			{
				vga[10] = '#';
				scancode[2] = inb(0x60);
				pressed = false;
				scancodeLength++;
			}
		}
		
		while(inb(0x60) != 0x00);
		
		int lookup = scancode[scancodeLength] + (scancode[0] == 0xE0 ? 0x80 : 0x00);
		KeyboardEvent event(KeyboardTranslator::lookupToKeycode(lookup), pressed);
		Keyboard::sendEvent(event);
	}
	
}
