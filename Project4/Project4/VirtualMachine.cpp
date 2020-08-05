#include <iostream>
#include <cstdio>
#include <fstream>
#include <queue>
#include <map>
#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(std::string filename, int _memsize, int _mainsize, int threadNum) {
	MEM_SIZE = _memsize;
	const int memsize = _memsize;
	memory = new char[memsize];
	flushmem();
	assembler = new Assembler(memory, MEM_SIZE, &program, &heap);
	assembler->load(filename);
	data = 0;
	registers[PC] = program;
	stack = MEM_SIZE - 1;
	endOfMain = stack - _mainsize;
	threadsize = threadNum;
	registers[FP] = stack;
	registers[SL] = heap;
	registers[SP] = stack;
	registers[SB] = stack;
	int ret = run();
}

VirtualMachine::~VirtualMachine() {
	delete[] memory;
	delete assembler;
}

int VirtualMachine::run()
{
	bool running = true;
	bool end = false;
	std::queue<int> threadText;
	int threadnum = 1;
	std::map<int, int> mutex;
	threadText.push(endOfMain);
	try {
		while (running)
		{
			fetch();
			int addr;
			switch (ir.opCode)
			{
			case TRP:
				switch (ir.op1)
				{
				case 0:
					running = false;
					break;
				case 1:
					std::cout << (int)registers[3];
					break;
				case 2:
					int itemp;
					std::cin >> itemp;
					registers[3] = itemp;
					break;
				case 3:
					std::cout << (char)registers[3];
					break;
				case 4:
					char ctemp;
					ctemp = std::getchar();
					//std::cin >> ctemp;
					registers[3] = ctemp;
					break;
				case 10:
					registers[3] -= 48;
					break;
				case 11:
					registers[3] += 48;
					break;
				default:
					std::cout << "Unknown Implementation" << std::endl;
				}
				registers[PC] += INST_SIZE;
				break;
			case JMP:
				registers[PC] = ir.op1;
				break;
			case JMR:
				registers[PC] = registers[ir.op1];
				break;
			case BNZ:
				if (registers[ir.op1] != 0)
					registers[PC] = ir.op2;
				else
					registers[PC] += INST_SIZE;
				break;
			case BGT:
				if (registers[ir.op1] > 0)
					registers[PC] = ir.op2;
				else
					registers[PC] += INST_SIZE;
				break;
			case BLT:
				if (registers[ir.op1] < 0)
					registers[PC] = ir.op2;
				else
					registers[PC] += INST_SIZE;
				break;
			case BRZ:
				if (registers[ir.op1] == 0)
					registers[PC] = ir.op2;
				else
					registers[PC] += INST_SIZE;
				break;
			case MOV:
				registers[ir.op1] = registers[ir.op2];
				registers[PC] += INST_SIZE;
				break;
			case LDA:
				registers[ir.op1] = ir.op2;
				registers[PC] += INST_SIZE;
				break;
			case STR:
				if (mutex.find(ir.op2) != mutex.end())
					if (mutex[ir.op2] != threadText.front())
						break;
				writeInt(registers[ir.op1], ir.op2);
				registers[PC] += INST_SIZE;
				break;
			case LDR:
				if (mutex.find(ir.op2) != mutex.end())
					if (mutex[ir.op2] != threadText.front())
						break;
				registers[ir.op1] = readInt(ir.op2);
				registers[PC] += INST_SIZE;
				break;
			case STB:
				if (mutex.find(ir.op2) != mutex.end())
					if (mutex[ir.op2] != threadText.front())
						break;
				writeChar(registers[ir.op1], ir.op2);
				registers[PC] += INST_SIZE;
				break;
			case LDB:
				if (mutex.find(ir.op2) != mutex.end())
					if (mutex[ir.op2] != threadText.front())
						break;
				registers[ir.op1] = readChar(ir.op2);
				registers[PC] += INST_SIZE;
				break;
			case ADD:
				registers[ir.op1] += registers[ir.op2];
				registers[PC] += INST_SIZE;
				break;
			case ADI:
				registers[ir.op1] += ir.op2;
				registers[PC] += INST_SIZE;
				break;
			case SUB:
				registers[ir.op1] -= registers[ir.op2];
				registers[PC] += INST_SIZE;
				break;
			case MUL:
				registers[ir.op1] *= registers[ir.op2];
				registers[PC] += INST_SIZE;
				break;
			case DIV:
				if (registers[ir.op2] == 0)
					std::cout << "error DIV" << std::endl;
				registers[ir.op1] /= registers[ir.op2];
				registers[PC] += INST_SIZE;
				break;
			case AND:
				registers[ir.op1] = registers[ir.op1] && registers[ir.op1];
				registers[PC] += INST_SIZE;
				break;
			case OR:
				registers[ir.op1] = registers[ir.op1] || registers[ir.op1];
				registers[PC] += INST_SIZE;
				break;
			case CMP:
				if (registers[ir.op1] == registers[ir.op2])
					registers[ir.op1] = 0;
				else if (registers[ir.op1] > registers[ir.op2])
					registers[ir.op1] = 1;
				else registers[ir.op1] = -1;
				registers[PC] += INST_SIZE;
				break;
			case STRI:
				if (mutex.find(ir.op2) != mutex.end())
					if (mutex[ir.op2] != threadText.front())
						break;
				writeInt(registers[ir.op1], registers[ir.op2]);
				registers[PC] += INST_SIZE;
				break;
			case LDRI:
				if (mutex.find(ir.op2) != mutex.end())
					if (mutex[ir.op2] != threadText.front())
						break;
				registers[ir.op1] = readInt(registers[ir.op2]);
				registers[PC] += INST_SIZE;
				break;
			case STBI:
				if (mutex.find(ir.op2) != mutex.end())
					if (mutex[ir.op2] != threadText.front())
						break;
				writeChar(registers[ir.op1], registers[ir.op2]);
				registers[PC] += INST_SIZE;
				break;
			case LDBI:
				if (mutex.find(ir.op2) != mutex.end())
					if (mutex[ir.op2] != threadText.front())
						break;
				registers[ir.op1] = readChar(registers[ir.op2]);
				registers[PC] += INST_SIZE;
				break;
			case RUN:
				addr = endOfMain - threadsize * (threadnum);
				registers[ir.op1] = threadnum++;
				threadText.push(addr);
				for (int i = 0; i<8; i++) {
					writeInt(registers[i], addr - 4 * (i + 1));
				}
				writeInt(ir.op2, addr - 4 * (PC + 1));
				writeInt(addr - 4 * (13), addr - 4 * (FP + 1));
				writeInt(addr - 4 * (13), addr - 4 * (SP + 1));
				writeInt(endOfMain - threadsize * (threadnum), addr - 4 * (SL + 1));
				writeInt(addr - 4 * (13), addr - 4 * (SB + 1));
				registers[PC] += INST_SIZE;
				break;
			case END:
				if (threadText.front() == endOfMain || threadText.size() == 1)
				{
					registers[PC] += INST_SIZE;
					break;
				}
				threadText.pop();
				registers[PC] += INST_SIZE;
				end = true;
				break;
			case BLK:
				if (threadText.size() == 1)
					registers[PC] += INST_SIZE;
				break;
			case LCK:
				if (mutex.find(ir.op1) != mutex.end())
					if (mutex[ir.op1] != threadText.front())
						break;
				mutex[ir.op1] = threadText.front();
				registers[PC] += INST_SIZE;
				break;
			case ULK:
				mutex.erase(mutex.find(ir.op1));
				registers[PC] += INST_SIZE;
				break;
			default:
				std::cout << "Unknown INST" << std::endl;
			};

			if (!end)
			{
				addr = threadText.front();
				for (int i = 0; i<13; i++)
				{
					writeInt(registers[i], addr - 4 * (i + 1));
				}
				threadText.pop();
				threadText.push(addr);
			}
			end = false;
			addr = threadText.front();
			for (int i = 0; i<13; i++)
			{
				registers[i] = readInt(addr - 4 * (i + 1));
			}
		}
	}
	catch (...)
	{
		return -1;
	}
	return 0;
}

void VirtualMachine::writeInt(int data, int address)
{
	char* ptr = memory;
	ptr += address;
	(*(int*)ptr) = data;
}

void VirtualMachine::writeChar(char data, int address)
{
	memory[address] = data;
}

int VirtualMachine::readInt(int address)
{
	char* ptr = memory;
	ptr += address;
	return (*(int*)ptr);
}

char VirtualMachine::readChar(int address)
{

	return memory[address];
}

void VirtualMachine::fetch() {
	ir.opCode = readInt(registers[PC]);
	ir.op1 = readInt(registers[PC] + INT_SIZE);
	ir.op2 = readInt(registers[PC] + 2 * INT_SIZE);
}

void VirtualMachine::flushmem() {
	for (int i = 0; i<MEM_SIZE; i++)
		memory[i] = 0;
}
