#include "keyboard.h"
#include "keyboardtranslator.h"
#include "keyboardevent.h"
#include "pio.h"
#include "system.h"

bool Keyboard::keyflags[128];

void Keyboard::initialize()
{
	waitForBuffer(BUFFER_INPUT, false);
	outb(0x64, 0xFF);
	waitForAck();
}

void Keyboard::sendEvent(KeyboardEvent event)
{
	keyflags[event.getKeycode()] = event.isPressed();
	char s[2] = {KeyboardTranslator::keycodeToAscii(event.getKeycode()), '\0'};
	System::print(s);
}

void Keyboard::scan()
{
	char scancode[8] = {0};
	int scancodeLength = 0;
	bool pressed = true;

	uint8_t byte = 0;
	do
	{
		byte = readByte();
		scancodeLength++;
	} while(byte != 0);
	
	if(scancode[0] == 0xF0)
		pressed = false;
	else if(scancode[0] == 0xE0)
		if(scancode[1] == 0xF0)
			pressed = false;
	
	int lookup = scancode[scancodeLength - 1] + (scancode[0] == 0xE0 ? 0x80 : 0x00);
	KeyboardEvent event(KeyboardTranslator::lookupToKeycode(lookup), pressed);
	sendEvent(event);
}

uint8_t Keyboard::readByte()
{
	if(waitForBuffer(BUFFER_OUTPUT, true))
	{
		uint8_t b = inb(0x60);
		System::print("Read byte: ");
		System::print(b);
		System::print("\n");
		return b;
	}
	else
		return 0;
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
		System::print("Reply: ");
		System::print(response);
		System::print("\n");
		if(response == 0xFA)
			return true;
		else
			return false;
	}
	else
	{
		System::printError("Keyboard timeout\n");
		return false;
	}
}
