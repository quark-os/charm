#include <stdint.h>

#include "system.h"

extern "C"
{
	
	char* vga = 0xB8000;
	int vgaPos = 80;
	
	int main()
	{		
		System::initialize();
		asm("sti");
		while(true)
		{
			asm("hlt");
		}
		
		return 0;
	}
}
