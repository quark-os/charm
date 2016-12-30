#include <stdint.h>

#include "interruptcontroller.h"
#include "window.h"
#include "keyboardtranslator.h"

extern "C"
{
	
	char* vga = 0xB8000;
	int vgaPos = 80;
	
	int main()
	{		
		InterruptController::init(0x100000);
		KeyboardTranslator::initialize();
		
		Window win(30, 1, 50, 24);
		win.clear();
		win << "Hello!\n";
		win << "Hopefully this works... \n";
		
		Window win2(0, 1, 30, 24);
		win2.clear();
		win2 << "Hello!\n";
		win2 << "Hopefully this works... \n";
		
		asm("sti");
		while(true)
		{
			asm("hlt");
		}
		
		return 0;
	}
}
