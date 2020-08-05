#include "Assembler.h"
#include "VirtualMachine.h"
#include <iostream>

int main(int argc, char *argv[])
{
	//std::string input;
	//std::cout << "Enter asm file name: " << std::endl;
	//std::cin >> input;
	VirtualMachine vm(argv[1]); //argv[1]
	return 0;
}