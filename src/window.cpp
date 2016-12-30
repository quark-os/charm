#include "window.h"

#define BOX_TOPLEFT 201
#define BOX_TOPRIGHT 187
#define BOX_BOTTOMLEFT 200
#define BOX_BOTTOMRIGHT 188
#define BOX_LATSIDE 186
#define BOX_VERTSIDE 205

Window::Window()
{
	
}

Window::Window(int x, int y, int width, int height)
{
	this->vga = (char*) 0xB8000;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->cursorX = 0;
	this->cursorY = 0;
	
	for(int boxY = 0; boxY < height; boxY++)
	{
		if(boxY == 0)
		{
			vga[coordToOffset(x, y)] = BOX_TOPLEFT;
			for(int boxX = 1; boxX < width - 1; boxX++)
			{
				vga[coordToOffset(x + boxX, y)] = BOX_VERTSIDE;
			}
			vga[coordToOffset(x + width - 1, y)] = BOX_TOPRIGHT;
		}
		else if(boxY == height - 1)
		{
			for(int boxY = 1; boxY < height - 1; boxY++)
			{
				vga[coordToOffset(x, y + boxY)] = BOX_LATSIDE;
				vga[coordToOffset(x + width - 1, y + boxY)] = BOX_LATSIDE;
			}
		}
		else
		{
			vga[coordToOffset(x, y + height - 1)] = BOX_BOTTOMLEFT;
			for(int boxX = 1; boxX < width - 1; boxX++)
			{
				vga[coordToOffset(x + boxX, y + height - 1)] = BOX_VERTSIDE;
			}
			vga[coordToOffset(x + width - 1, y + height - 1)] = BOX_BOTTOMRIGHT;
		}
	}
}

void Window::operator<<(char* data)
{
	for(int i = 0; data[i] != 0; i++)
	{
		if(data[i] != '\n')
		{
			vga[coordToOffset(cursorX + 1 + this->x, cursorY + 1 + this->y)] = data[i];
			if(cursorX < width - 3)
			{
				cursorX++;
			}
			else
			{
				cursorY++;
				cursorX = 0;
			}
		}
		else
		{
			cursorY++;
			cursorX = 0;
		}
	}
}

void Window::operator<<(uint8_t data)
{
	
}

void Window::operator<<(uint16_t data)
{
	
}

void Window::operator<<(uint32_t data)
{
	
}

void Window::operator<<(uint64_t data)
{
	
}

void Window::clear()
{
	for(int x = 1; x < width - 1; x++)
	{
		for(int y = 1; y < height - 1; y++)
		{
			vga[coordToOffset(x + this->x, y + this->y)] = ' ';
		}
	}
	
}

int Window::coordToOffset(int x, int y)
{
	return (80*y + x) * 2;
}
