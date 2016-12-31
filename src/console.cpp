#include "console.h"
#include "keyboardtranslator.h"
#include "keyboard.h"
#include "system.h"

Console::Console()
	: Window(), EventListenerKey()
{
	bufferLength = 0;
	for(int i = 0; i < 256; i++)
		commandBuffer[i] = 0;
}

Console::Console(int x, int y, int width, int height)
	: Window(x, y, width, height), EventListenerKey()
{
	bufferLength = 0;
	for(int i = 0; i < 256; i++)
		commandBuffer[i] = 0;
}

void Console::process(KeyboardEvent event)
{
	char c = KeyboardTranslator::keycodeToAscii(Keyboard::getKeyState(0x33) ? event.getKeycode() + 0x60 : event.getKeycode());
	if(event.isPressed() && c != '\0')
	{
		if(c == '\n')
		{
			newline();
			// Do command
		}
		else
		{
			putChar(c);
			commandBuffer[bufferLength] = c;
			bufferLength++;
		}
		// Handle other keys...
	}
}


