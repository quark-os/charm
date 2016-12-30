#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BUFFER_INPUT 0x02
#define BUFFER_OUTPUT 0x01

#define CMD_DISABLE 0xF5
#define CMD_ENABLE 0xF4
#define CMD_SCANCODESET 0xF0
#define CMD_SETLED 0xED
#define CMD_WRITECFG 0x60

#include "keyboardevent.h"
#include "eventlistenerkey.h"

class Keyboard
{
public:
		
	static void initialize();
	
	static bool getKeyState(uint8_t keycode);
	
	static uint8_t addListener(EventListenerKey* listener);
	
	static void sendEvent(KeyboardEvent event);
	
	static void handleEvents();
	
	static void scan();
			
private:

	static void doCommand(uint8_t command);
	
	static void doCommand(uint8_t command, uint8_t data);

	static uint8_t readByte();
	
	static bool waitForBuffer(uint8_t buffer, bool state);
	
	static bool waitForAck();

	static bool keyflags[128];
	
	static EventListenerKey* listeners[16];
	
	static KeyboardEvent eventQueue[16];
	
	static int eventQueueLength;
		
};

#endif
