#ifndef CONSOLE_H
#define CONSOLE_H

#include "window.h"
#include "eventlistenerkey.h"
#include "keyboardevent.h"

class Console: public Window, public EventListenerKey
{
public:

		Console();
		
		Console(int x, int y, int width, int height);
		
		virtual void process(KeyboardEvent event);
			
private:
	
	int bufferLength;
	
	char commandBuffer[256];
	
};

#endif
