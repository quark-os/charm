#include "system.h"
#include "keyboard.h"
#include "keyboardtranslator.h"
#include "interruptcontroller.h"

Display System::display;

void System::initialize()
{	
	display = Display();		
	KeyboardTranslator::initialize();
	Keyboard::initialize();
	display.initialize();
	InterruptController::init(0x100000);
	
	display.print("Finished init.\n");
}

void System::run()
{
	asm("sti");
	while(true)
	{
		asm("hlt");
		Keyboard::handleEvents();
	}
}

void System::panic()
{
	display.printError("PANIC: Halting system");
	asm("cli");
	while(true)
		asm("hlt");
}
