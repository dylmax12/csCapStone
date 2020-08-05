#include "Assembler.h"
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

Assembler::Assembler(char*	_memory, int	_MEMSIZE, int*	_program, int*	_heap) {
	memory = _memory;
	MEMSIZE = _MEMSIZE;
	filename = "";
	instructions["JMP"] = 1;
	instructions["JMR"] = 2;
	instructions["BNZ"] = 3;
	instructions["BGT"] = 4;
	instructions["BLT"] = 5;
	instructions["BRZ"] = 6;
	instructions["MOV"] = 7;
	instructions["LDA"] = 8;
	instructions["STR"] = 9;
	instructions["LDR"] = 10;
	instructions["STB"] = 11;
	instructions["LDB"] = 12;
	instructions["ADD"] = 13;
	instructions["ADI"] = 14;
	instructions["SUB"] = 15;
	instructions["MUL"] = 16;
	instructions["DIV"] = 17;
	instructions["AND"] = 18;
	instructions["OR"] = 19;
	instructions["CMP"] = 20;
	instructions["__9__"] = 21;
	instructions["__10__"] = 22;
	instructions["__11__"] = 23;
	instructions["__12__"] = 24;
	instructions["RUN"] = 30;
	instructions["END"] = 31;
	instructions["BLK"] = 32;
	instructions["LCK"] = 33;
	instructions["ULK"] = 34;
	instructions["TRP"] = 0;
	symbolTable["R0"] = 0;
	symbolTable["R1"] = 1;
	symbolTable["R2"] = 2;
	symbolTable["R3"] = 3;
	symbolTable["R4"] = 4;
	symbolTable["R5"] = 5;
	symbolTable["R6"] = 6;
	symbolTable["R7"] = 7;
	symbolTable["(R0)"] = 0;
	symbolTable["(R1)"] = 1;
	symbolTable["(R2)"] = 2;
	symbolTable["(R3)"] = 3;
	symbolTable["(R4)"] = 4;
	symbolTable["(R5)"] = 5;
	symbolTable["(R6)"] = 6;
	symbolTable["(R7)"] = 7;
	symbolTable["PC"] = 8;
	symbolTable["FP"] = 9;
	symbolTable["SP"] = 10;
	symbolTable["SL"] = 11;
	symbolTable["SB"] = 12;
	symbolTable["(FP)"] = 9;
	symbolTable["(SP)"] = 10;
	symbolTable["(SL)"] = 11;
	symbolTable["(SB)"] = 12;
	program = _program;
	*program = 0;
	heap = _heap;
	*heap = 0;
}

Assembler::~Assembler() {
}

std::string Assembler::searchArray(std::string theArray[], int arraySize, std::string letter) {
	//search array function that returns a STRING if it is in the array.. false if else.
	int letterCount = 0;
	for (int x = 0; x < arraySize; ++x) {
		if (theArray[x] == letter) {
			return letter;
		}
	}
	//not found
	return "false";
}
int Assembler::searchArrayIndex(std::string theArray[], int arraySize, std::string letter) {

	//search array function that returns the INDEX of an array member if found in the array.. -1 if else.
	int letterCount = 0;
	for (int x = 0; x < arraySize; ++x) {
		if (theArray[x] == letter) {
			return x;
		}
	}
	//not found
	return -1;
}


void Assembler::createInstTable(std::list<std::string> stringList, int &bytecount)
{
	std::string first, second, third;
	first = stringList.front();
	stringList.pop_front();
	if (!stringList.empty()) {
		second = stringList.front();
		stringList.pop_front();
	}
	else {
		second = "";
	}

	if (!stringList.empty())
	{
		third = stringList.front();
		stringList.pop_front();
	}
	else
		third = "";

	if (first.substr(0, 1) == ";")
		return;
	if (instructions.find(first) != instructions.end())
	{
		bytecount += INST_SIZE;
	}
	else if (first.substr(0, 1) == ".")
	{
		if (first == ".INT")
			bytecount += INT_SIZE;
		else if (first == ".BYT")
			bytecount += BYT_SIZE;
	}
	else // Label
	{
		if (instructions.find(second) != instructions.end())
		{
			if (symbolTable.find(second) != symbolTable.end()) {
				std::cout << "already in table" << std::endl;
			}
			symbolTable[first] = bytecount;
			bytecount += INST_SIZE;
		}
		else if (second.substr(0, 1) == ".")
		{
			if (symbolTable.find(first) != symbolTable.end()) {
				std::cout << "already in table" << std::endl;
			}
			symbolTable[first] = bytecount;
			if (second == ".INT")
				bytecount += INT_SIZE;
			else if (second == ".BYT")
				bytecount += BYT_SIZE;
		}
		else
		{
		}
	}
}

void Assembler::code(std::list<std::string> stringList, int &linecount, int &bytecount)
{
	std::string first, second, third, fourth, fifth;
	bool comment = false;

	first = stringList.front();
	stringList.pop_front();

	if (first.substr(0, 1) == ";")
		comment = true;

	if (!stringList.empty() && !comment)
	{
		second = stringList.front();
		stringList.pop_front();
	}
	else
		second = "";

	if (!stringList.empty() && !comment)
	{
		third = stringList.front();
		stringList.pop_front();
		if (third.substr(0, 1) == ";")
		{
			comment = true;
			third = "";
		}
	}
	else
		third = "";

	if (!stringList.empty() && !comment)
	{
		fourth = stringList.front();
		stringList.pop_front();
		if (fourth.substr(0, 1) == ";") {
			comment = true;
			fourth = "";
		}
	}
	else {
		fourth = "";
	}


	if (!stringList.empty() && !comment)
	{
		fifth = stringList.front();
		if (fifth.substr(0, 1) != ";")
		{

		}
		else
		{
			comment = true;
			fifth = "";
		}
	}

	bool label = false;

	// FIRST RUN
	int opcode = -1;
	int position = 0;
	if (first.substr(0, 1) == ";") return;
	if (instructions.find(first) != instructions.end())
	{
		writeInt(instructions[first], bytecount);
		opcode = instructions[first];
		position = 1;
		bytecount += INT_SIZE;
		if (instructions[first] == 31 || instructions[first] == 32)
		{
			writeInt(0, bytecount);
			bytecount += INT_SIZE;
			writeInt(0, bytecount);
			bytecount += INT_SIZE;
			return;
		}
	}
	else if (symbolTable.find(first) != symbolTable.end())
	{
		writeInt(symbolTable[first], bytecount);
		label = true;
	}
	else if (first.substr(0, 1) == ".")
	{
		if (first == ".INT")
		{
			if (second == "") { std::cout << "MISSING" << std::endl; }
			writeInt(std::strtol(second.c_str(), NULL, 10), bytecount);
			bytecount += INT_SIZE;
			if (third != "" && third.substr(0, 1) != ";") {
				std::cout << "MISSING" << std::endl;
			}
			*program = bytecount;
			return;
		}
		else if (first == ".BYT")
		{
			if (second == "") {
				std::cout << "MISSING" << std::endl;
			}
			if (second.length() == 3)
				if (second.substr(0, 1) == "'" && second.substr(second.length() - 1, 1) == "'")
					writeChar(*second.substr(1, 1).c_str(), bytecount);
				else
					writeChar((char)std::strtol(second.c_str(), NULL, 10), bytecount);
			else
				writeChar((char)std::strtol(second.c_str(), NULL, 10), bytecount);
			bytecount += BYT_SIZE;
			if (third != "" && third.substr(0, 1) != ";") {
				std::cout << "MISSING" << std::endl;
			}
			*program = bytecount;
			return;
		}
		else
		{
			std::cout << "MISSING" << std::endl;
		}

	}
	else
	{
		std::cout << "MISSING" << std::endl;
	}

	// SECOND RUN
	bool operandFlag = false;

	if (second.substr(0, 1) == ";")
	{
		std::cout << "MISSING" << std::endl;
	}
	if (second.substr(0, 1) == ".") // Directive
	{
		if (!label) {
			std::cout << "MISSING" << std::endl;
		}
		if (second == ".INT")
		{
			if (third == "") {
				std::cout << "MISSING" << std::endl;
			}
			writeInt(std::strtol(third.c_str(), NULL, 10), bytecount);
			bytecount += INT_SIZE;
			if (fourth != "" && fourth.substr(0, 1) != ";") { std::cout << "MISSING" << std::endl; }
			*program = bytecount;
			return;
		}
		else if (second == ".BYT")
		{
			if (third == "") { std::cout << "MISSING" << std::endl; }
			if (third.length() == 3)
				if (third.substr(0, 1) == "'" && third.substr(third.length() - 1, 1) == "'")
					writeChar(*third.substr(1, 1).c_str(), bytecount);
				else
					writeChar((char)std::strtol(third.c_str(), NULL, 10), bytecount);
			else
				writeChar((char)std::strtol(third.c_str(), NULL, 10), bytecount);
			bytecount += BYT_SIZE;
			if (fourth != "" && fourth.substr(0, 1) != ";") { std::cout << "MISSING" << std::endl; }
			*program = bytecount;
			return;
		}
		else
		{
			std::cout << "MISSING" << std::endl;
		}

	}
	else if (symbolTable.find(second) != symbolTable.end())
	{
		writeInt(symbolTable[second], bytecount);
		bytecount += INT_SIZE;
		operandFlag = true;
	}
	else if (instructions.find(second) != instructions.end())
	{
		writeInt(instructions[second], bytecount);
		opcode = instructions[second];
		position = 2;
		bytecount += INT_SIZE;
	}
	else if (second == "")
	{
		std::cout << "MISSING" << std::endl;
	}
	else
	{
		writeInt(std::strtol(second.c_str(), NULL, 10),
			bytecount);
		bytecount += INT_SIZE;
		operandFlag = true;
	}

	// THIRD RUN
	if (third == "")
	{
		if (operandFlag)
		{
			writeInt(0, bytecount);
			bytecount += INT_SIZE;
			return;
		}

	}
	if (third.substr(0, 1) == ";")
	{
		writeInt(0, bytecount);
		bytecount += INT_SIZE;
		return;
	}
	if (symbolTable.find(third) != symbolTable.end())
	{
		if ((third == "FP" || third == "SP" || third == "SB" || third == "SL" ||
			third == "(FP)" || third == "(SP)" || third == "(SB)" || third == "(SL)") &&
			(position == 1 && (opcode >= instructions["STR"] && opcode <= instructions["LDB"])))
		{
			writeInt(opcode + INDIRECT, bytecount - 2 * INT_SIZE);
		}
		else if (third == "FP" || third == "SP" || third == "SB" || third == "SL" ||
			third == "(FP)" || third == "(SP)" || third == "(SB)" || third == "(SL)")
		{
		}
		else
		{
			try
			{
				int itemp = 0;
				if (third.length() >= 1 && third.length() <= 2)
					itemp = std::stoi(third.substr(1, 1), nullptr);
				else if (third.length() >= 3 && third.length() <= 4)
					itemp = std::stoi(third.substr(2, 1), nullptr);
				if ((third.substr(0, 1) == "R" && itemp >= 0 && itemp <= 7) ||
					(third.substr(0, 2) == "(R" && itemp >= 0 && itemp <= 7))
				{
					if (position == 1 && (opcode >= instructions["STR"] && opcode <= instructions["LDB"]))
					{
						writeInt(opcode + INDIRECT, bytecount - 2 * INT_SIZE);
					}
				}
			}
			catch (...)
			{
			}
		}
		writeInt(symbolTable[third],
			bytecount);
		bytecount += INT_SIZE;
		operandFlag = !operandFlag;
	}
	else
	{
		writeInt(std::strtol(third.c_str(), NULL, 10), bytecount);
		bytecount += INT_SIZE;
		operandFlag = !operandFlag;
	}

	// FOURTH RUN

	if (fourth.substr(0, 1) == ";")
	{
		if (operandFlag)
		{
			writeInt(0, bytecount);
			bytecount += INT_SIZE;
		}
		return;
	}

	if (fourth != "" && !operandFlag) { std::cout << "MISSING" << std::endl; }
	if (fourth == "")
	{
		if (position == 2)
		{
			writeInt(0, bytecount);
			bytecount += INT_SIZE;
		}
		return;
	}
	if (symbolTable.find(fourth) != symbolTable.end())
	{
		if ((fourth == "FP" || fourth == "SP" || fourth == "SB" || fourth == "SL" ||
			fourth == "(FP)" || fourth == "(SP)" || fourth == "(SB)" || fourth == "(SL)")
			&& (position == 2 && (opcode >= instructions["STR"] && opcode <= instructions["LDB"])))
		{
			writeInt(opcode + INDIRECT, bytecount - 2 * INT_SIZE);
		}
		else if (fourth == "FP" || fourth == "SP" || fourth == "SB" || fourth == "SL" ||
			fourth == "(FP)" || fourth == "(SP)" || fourth == "(SB)" || fourth == "(SL)")
		{

		}
		else
		{
			try
			{
				int itemp = 0;
				if (fourth.length() >= 1 && fourth.length() <= 2)
					itemp = std::stoi(fourth.substr(1, 1), nullptr);
				else if (fourth.length() >= 3 && fourth.length() <= 4)
					itemp = std::stoi(fourth.substr(2, 1), nullptr);
				if ((fourth.substr(0, 1) == "R" && itemp >= 0 && itemp <= 7) ||
					(fourth.substr(0, 2) == "(R" && itemp >= 0 && itemp <= 7))
					if (position == 2 && (opcode >= instructions["STR"] && opcode <= instructions["LDB"]))
						writeInt(opcode + INDIRECT, bytecount - 2 * INT_SIZE);
			}
			catch (...)
			{

			}
		}

		writeInt(symbolTable[fourth], bytecount);
		bytecount += INT_SIZE;
		operandFlag = !operandFlag;
	}
	else
	{
		writeInt(std::strtol(fourth.c_str(), NULL, 10),
			bytecount);
		bytecount += INT_SIZE;
		operandFlag = !operandFlag;
	}

}

void Assembler::firstPass()
{
	int linecount = 0;
	int bytecount = 0;
	std::ifstream in;
	try
	{
		in.open(filename.c_str());
		std::string line;
		while (std::getline(in, line))
		{
			linecount++;
			if (line == "")
				continue;
			createInstTable(tokenize(line), bytecount);
		}
	}
	catch (...)
	{
		exit(1);
	}
	in.close();
}

void Assembler::secondPass()
{
	int linecount = 0;
	int bytecount = 0;
	std::ifstream in;
	try
	{
		in.open(filename.c_str());

		std::string line;
		while (std::getline(in, line))
		{
			linecount++;
			if (line == "")
				continue;

			code(tokenize(line), linecount, bytecount);
		}
		*heap = bytecount;
	}
	catch (...)
	{
		exit(1);
	}
	in.close();
}
bool Assembler::goodFile(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void Assembler::load(std::string fileName)
{
	filename = fileName;

	if (!goodFile(fileName.c_str()))
	{
		std::cout << "bad file" << std::endl;
		exit(1);
	}
	firstPass();
	secondPass();
}

std::list<std::string> Assembler::tokenize(const std::string &tstr)
{
	const std::string &delimiters = " 	,";
	std::list<std::string> stringList;
	std::string::size_type lastPos = tstr.find_first_not_of(delimiters, 0);
	std::string::size_type pos = tstr.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos) {
		stringList.push_back(tstr.substr(lastPos, pos - lastPos));
		lastPos = tstr.find_first_not_of(delimiters, pos);
		pos = tstr.find_first_of(delimiters, lastPos);
	}
	return stringList;
}

void Assembler::writeInt(int data, int address)
{
	char* ptr = memory;
	ptr += address;
	(*(int*)ptr) = data;
}

void Assembler::writeChar(char data, int address)
{
	memory[address] = data;
}