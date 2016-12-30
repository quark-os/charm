#include "keyboard.h"
#include "keyboardtranslator.h"
#include "pio.h"
#include "system.h"

bool Keyboard::keyflags[128];

void Keyboard::initialize()
{
	uint8_t status = inb(0x64);
	System::print(status);
	System::panic();
	/*if(!waitForEmptyBuffer())
		System::panic();
	outb(0x64, 0xF5);
	
	if(!waitForEmptyBuffer())
		System::panic();
	outb(0x64, 0xF0);
	if(!waitForEmptyBuffer())
		System::panic();
	outb(0x60, 2);
	
	if(!waitForEmptyBuffer())
		System::panic();
	outb(0x64, 0xF4);*/
}

void Keyboard::sendEvent(KeyboardEvent event)
{
	keyflags[event.getKeycode()] = event.isPressed();
	char* vga = (char*) 0xB8000;
	vga[16] = KeyboardTranslator::keycodeToAscii(event.getKeycode());
}

void Keyboard::flushData()
{
	uint8_t d;
	do
	{
		d = inb(0x60);
		io_wait();
	} while(d != 0);
}

bool Keyboard::waitForEmptyBuffer()
{
	int counter = 0;
	uint8_t status = 0xFF;
	do
	{
		status = inb(0x64);
		io_wait();
		counter++;
		if(counter > 16)
			return false;
	} while((status & 0x02) != 0);
	return true;
}
