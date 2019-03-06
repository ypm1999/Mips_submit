#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "MipsParser.h"
#include "MipsSimulator.h"
using std::unique_ptr;

void runCode(string);



int main(int argc, char *argv[]) {
#ifndef DEBUG
	std::ios::sync_with_stdio(false);
	cin.tie(false);
	cout.tie(false);
#endif // !DEBUG

	//freopen("code.in", "r", stdin);
	//freopen("code.out", "w", stdout);
	//runCode("code.s");
	runCode(argv[1]);
	return 0;
}


void runCode(string source) {

	std::ifstream sourceFile(source);
	if (!sourceFile.is_open())
		throw Error("Source code file do not exit!");
	std::stringstream buffer;
	buffer << sourceFile.rdbuf();

	printf("test!!!!!!");

	unique_ptr<MipsSimulator> program(new MipsSimulator(Parser.get()));
	if (!program->run(Parser.get()->getEntry(), Parser.get()->getLimit(), mem.get()))
		throw Error("Run time error!");

#ifdef DEBUG
	if (program->with_returnValue())
		std::cerr << source << " return Value is " << program->get_returnValue() << std::endl;
	else
		std::cerr << source << " return without returnValue" << std::endl;
#endif // DEBUG
}
