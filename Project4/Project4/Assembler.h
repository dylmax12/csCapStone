
#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <map>
#include <string>
#include <list>


#define INST_SIZE 12
#define INT_SIZE 4
#define BYT_SIZE 1
#define INDIRECT 12

class Assembler {
public:
	Assembler(char*, int, int*, int*);
	~Assembler();
	std::string searchArray(std::string theArray[], int arraySize, std::string letter);
	int searchArrayIndex(std::string theArray[], int arraySize, std::string letter);
	void load(std::string);
private:
	char* memory;
	int* program;
	int* heap;
	int MEMSIZE;
	std::string filename;
	std::map<std::string, int> instructions;
	std::map<std::string, int> symbolTable;
	std::list<std::string> tokenize(const std::string&);
	void createInstTable(std::list<std::string>, int&);
	void code(std::list<std::string>, int&, int&);
	void firstPass();
	void secondPass();
	void writeInt(int, int);
	void writeChar(char, int);
	bool goodFile(const char*);
};

#endif