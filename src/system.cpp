#include "system.h"
#include "keyboard.h"
#include "keyboardtranslator.h"
#include "interruptcontroller.h"

Window System::console;

Window System::error;

void System::initialize()
{
	console = Window(0, 1, 40, 24);
	error = Window(40, 1, 40, 24);
	console.clear();
	error.clear();
	
	KeyboardTranslator::initialize();
	Keyboard::initialize();
	InterruptController::init(0x100000);
}

void System::print(char* msg)
{
	console << msg;
}

void System::print(uint8_t msg)
{
	console << msg;
}

void System::print(uint16_t msg)
{
	console << msg;
}

void System::print(uint32_t msg)
{
	console << msg;
}

void System::print(uint64_t msg)
{
	console << msg;
}

void System::printError(char* msg)
{
	error << msg;
}

void System::panic()
{
	error << "PANIC: Halting system";
	asm("cli");
	while(true)
		asm("hlt");
}
