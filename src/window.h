#ifndef __WINDOW_H
#define __WINDOW_H

#include <stdint.h>

#include "stream.h"

class Window : public Stream
{
public:

	Window();
	
	Window(int x, int y, int width, int height);
	
	virtual void operator<<(char* data);
	
	virtual void operator<<(uint8_t data);
	
	virtual void operator<<(uint16_t data);
	
	virtual void operator<<(uint32_t data);
	
	virtual void operator<<(uint64_t data);
	
	void clear();
	
private:

	char* vga;

	int x, y;
	
	int width, height;

	int cursorX, cursorY;
	
	int coordToOffset(int x, int y);

};

#endif
