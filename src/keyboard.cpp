#include "keyboard.h"
#include "keyboardtranslator.h"
#include "keyboardevent.h"
#include "pio.h"
#include "system.h"

bool Keyboard::keyflags[128];

KeyboardEvent Keyboard::eventQueue[16];

int Keyboard::eventQueueLength;

EventListenerKey* Keyboard::listeners[16];

void Keyboard::initialize()
{
	eventQueueLength = 0;
	for(int i = 0; i < 16; i++)
	{
		listeners[i] = (EventListenerKey*) 0;
	}
	
	waitForBuffer(BUFFER_INPUT, false);
	outb(0x64, 0x20);
	waitForBuffer(BUFFER_OUTPUT, true);
	uint8_t config = inb(0x60);
	config &= 0xBF;
	
	waitForBuffer(BUFFER_INPUT, false);
	outb(0x64, 0x60);
	outb(0x60, config);
}

bool Keyboard::getKeyState(uint8_t keycode)
{
	return keyflags[keycode];
}

uint8_t Keyboard::addListener(EventListenerKey* listener)
{
	for(int i = 0; i < 16; i++)
	{
		if(listeners[i] == 0)
		{
			listeners[i] = listener;
			return i;
		}
	}
}

void Keyboard::sendEvent(KeyboardEvent event)
{
	keyflags[event.getKeycode()] = event.isPressed();
	eventQueue[eventQueueLength] = event;
	eventQueueLength++;
	/*if(event.isPressed() && KeyboardTranslator::keycodeToAscii(event.getKeycode()) != '\0')
		System::display.putChar(KeyboardTranslator::keycodeToAscii(keyflags[0x33] ? event.getKeycode() + 0x60 : event.getKeycode()));
		*/
}

void Keyboard::handleEvents()
{
	for(int i = 0; i < eventQueueLength; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			if(listeners[i] != 0)
			{
				(*listeners[j])(eventQueue[i]);
			}
		}
	}
	eventQueueLength = 0;
}

void Keyboard::scan()
{
	uint8_t scancode[8] = {0};
	int scancodeLength = 0;
	bool pressed = true;

	uint8_t byte = 0;
	do
	{
		byte = readByte();
		if(byte != 0)
		{
			scancode[scancodeLength] = byte;
			scancodeLength++;
		}
	} while(byte != 0);
	
	if(scancode[0] == 0xF0)
	{
		
		pressed = false;
	}
	else if(scancode[0] == 0xE0)
	{
		if(scancode[1] == 0xF0)
		{
			pressed = false;
		}
	}
	
	if(scancodeLength > 0)
	{
		uint8_t lookup = scancode[scancodeLength - 1] + (scancode[0] == 0xE0 ? 0x80 : 0x00);
		KeyboardEvent event(KeyboardTranslator::lookupToKeycode(lookup), pressed);
		sendEvent(event);
	}
}

uint8_t Keyboard::readByte()
{
	if(waitForBuffer(BUFFER_OUTPUT, true))
	{
		uint8_t b = inb(0x60);
		return b;
	}
	else
		return 0;
}

void Keyboard::doCommand(uint8_t command)
{
	if(waitForBuffer(BUFFER_INPUT, false))
	{
		outb(0x64, command);
		waitForAck();
	}
	else
	{
		System::display.printError("Kbd cmd timeout\n");
	}
}

void Keyboard::doCommand(uint8_t command, uint8_t data)
{
	if(waitForBuffer(BUFFER_INPUT, false))
	{
		outb(0x64, command);
		if(waitForBuffer(BUFFER_INPUT, false))
		{
			outb(0x60, data);
			waitForAck();
		}
		else
		{
			System::display.print("Kbd data timeout\n");
		}
	}
	else
	{
		System::display.printError("Kbd cmd timeout\n");
	}
}

bool Keyboard::waitForBuffer(uint8_t buffer, bool state)
{
	int counter = 0;
	uint8_t status = 0x00;
	do
	{
		status = inb(0x64);
		io_wait();
		counter++;
		if(counter > 16)
			return false;
	} while((status & buffer) != (state ? buffer : 0));
	return true;
}

bool Keyboard::waitForAck()
{
	if(waitForBuffer(BUFFER_OUTPUT, false))
	{
		uint8_t response = inb(0x60);
		System::display.print("Reply: ");
		System::display.print(response);
		System::display.print("\n");
		if(response == 0xFA)
			return true;
		else
			return false;
	}
	else
	{
		System::display.printError("Keyboard timeout\n");
		return false;
	}
}
