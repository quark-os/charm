#include "system.h"
#include "keyboard.h"
#include "keyboardtranslator.h"
#include "interruptcontroller.h"

Display System::display;

void System::initialize()
{
	display = Display();
	display.initialize();
	panic();
	KeyboardTranslator::initialize();
	Keyboard::initialize();
	InterruptController::init(0x100000);
	
	display.printError("Finished init.\n");
	panic();
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
