#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "MispParser.h"
#include "MipsSimulator.h"
using namespace std;

int main(int argc, char *argv[]) {

#ifdef DEBUG
	freopen("code.in", "r", stdin);
	freopen("code.out", "w", stdout);
#endif
	__initialization();
	string source(argv[1]);
	//string source("code.s");
	ifstream sourceFile(source);
	if (!sourceFile.is_open()) {
		std::cerr << "Source code file do not exit!" << std::endl;
		return 0;
	}
	stringstream buffer;
	buffer << sourceFile.rdbuf();
	std::unique_ptr<MipsParser> Parser(new MipsParser(buffer.str()));
	std::unique_ptr<Memory> mem(new Memory(memorySize));
	std::unique_ptr<MipsSimulator> program(new MipsSimulator(Parser.get(), mem.get()));
	Parser->parser(*mem);
#ifdef DEBUG
	return 0;
#endif
	int returnValue;
	if (program->run(returnValue))
		std::cerr << source << " return Value is " << returnValue << std::endl;
	else
		std::cerr << source << " return without returnValue" << std::endl;
	return 0;
}
