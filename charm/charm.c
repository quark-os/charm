#include <stdint.h>

int main()
{
	char* vga = 0xB8000;
	vga[0] = '#';
	return 0;
}
