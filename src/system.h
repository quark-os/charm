#ifndef SYSTEM_H
#define SYSTEM_H

#include "window.h"
#include <stdint.h>

class System
{
public:

	static void initialize();
	
	static void print(char* msg);
	static void print(uint8_t msg);
	static void print(uint16_t msg);
	static void print(uint32_t msg);
	static void print(uint64_t msg);
	
	static void printError(char* msg);
	
	static void panic();
			
private:
	
	static Window console, error;
	
};

#endif
