#include <iostream>
#include <vector>
#include "Assembler.h"

#ifndef VIRTUALMACHINE_H_
#define VIRTUALMACHINE_H_

#define PC 8
#define FP 9
#define SP 10
#define SL 11
#define SB 12
class VirtualMachine {
public:
	VirtualMachine(std::string, int = 10240 * 102400, int = 1024 * 16, int = 1024);
	virtual ~VirtualMachine();
	int run();
private:
	Assembler * assembler;
	bool running;
	void writeInt(int, int);
	void writeChar(char, int);
	int readInt(int);
	int program;
	int heap;
	int stack;
	int endOfMain;
	int threadsize;
	int MEM_SIZE;
	int registers[13];
	char readChar(int);
	char *memory;
	char* data;
	struct InstructionRegister
	{
		int opCode;
		int op1;
		int op2;
	};
	void fetch();
	void flushmem();

	InstructionRegister ir;
	enum Instructions
	{
		TRP, JMP, JMR, BNZ, BGT, BLT, BRZ, MOV, LDA, STR, LDR, STB, LDB, ADD, ADI, SUB, MUL, DIV, AND, OR, CMP, STRI, LDRI, STBI, LDBI, RUN = 30, END = 31, BLK = 32, LCK = 33, ULK = 34
	};
};

#endif