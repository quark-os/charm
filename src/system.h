#ifndef SYSTEM_H
#define SYSTEM_H

#include "display.h"
#include <stdint.h>

class System
{
public:

	static Display display;

	static void initialize();
	
	static void run();
	
	static void panic();
			
private:
	
	
	
};

#endif
