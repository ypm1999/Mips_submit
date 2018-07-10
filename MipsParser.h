#ifndef __MipsParser
#define __MipsParser


#include <map>
#include <vector>
#include <string>
#include "Format.h"
#include "Memory.h"
#include "Exceptions.h"
using std::string;
using std::vector;
using std::map;
using std::pair;



class MipsParser {
	friend class MipsSimulator;

	class Data {
		friend class MipsParser;
		DataType type;
		string data;
	};
	
	Memory *mem;
	unsigned int codeLimit;
	unsigned int entry;
	string *sourceCode;
	vector< pair<string, vector<Data> > > *data;
	map<string, vector<int> > *table;
	map<string, Word> *commandMap;
	map<string, Word> *dataMap;
	map<string, CommandType> commandNameMap;
	map<string, DataType> dataNameMap;
	map<string, Byte> regMap;


	void __initialization();

	string getSingleString(unsigned int &pos) const;
	bool getData(unsigned int &pos, Data & res) const;
	bool getCommand(unsigned int &pos);
	void getDataBlock(unsigned int &pos, vector<Data> &res) const;
	Word getCommandBlock(unsigned int &pos);

	void getText();
	void writeData();
	void matchLabel();

public:
	MipsParser() noexcept;
	~MipsParser() = default;

	bool work(const string &str, Memory *_mem);
	unsigned int getLimit() { return codeLimit; }
	unsigned int getEntry() { return entry; }
	

	#ifdef DEBUG
		#include <iostream>
		unsigned int getAddress(string name) {
			auto tmp = commandMap->find(name);
			if (tmp == commandMap->end())
				return 0;
			else
				return tmp->second;
		}

		string getname(Word address) {
			for (auto j : *commandMap)
				if (j.second.i == address.i)
					return j.first;
			return "none";
		}

		string getdata(const Word &address) {
			for (auto i : *dataMap)
				if (i.second.i == address.i)
					return i.first;
			return "none";
		}

		string getcom(const CommandType &com) {
			for (auto i : commandNameMap)
				if (i.second == com)
					return i.first;
			return "none";
		}

		void output(Word now) {
			Word res1 = mem->getWord(now.ui << 3);
			Word res2 = mem->getWord((now.ui << 3) + (commandSize >> 1));
			Word lebal = 0;
			CommandType com = (CommandType)res1.b0;
			std::cout << getcom(com) << "  ";
			switch (com) {
			case CommandType::_add:
			case CommandType::_addu:
			case CommandType::_addiu:
			case CommandType::_sub:
			case CommandType::_subu:
			case CommandType::_mul:
			case CommandType::_mulu:
			case CommandType::_div:
			case CommandType::_divu:
			case CommandType::_xor:
			case CommandType::_xoru:
			case CommandType::_neg:
			case CommandType::_negu:
			case CommandType::_rem:
			case CommandType::_remu:
			case CommandType::_li:
			case CommandType::_seq:
			case CommandType::_sge:
			case CommandType::_sgt:
			case CommandType::_sle:
			case CommandType::_slt:
			case CommandType::_sne:
			case CommandType::_move:
			case CommandType::_mfhi:
			case CommandType::_mflo:
				printf("$%u $%u $%u %d", res1.b1, res1.b2, res1.b3, res2.i);
				break;
			case CommandType::_la:
			case CommandType::_lb:
			case CommandType::_lh:
			case CommandType::_lw:
			case CommandType::_sb:
			case CommandType::_sh:
			case CommandType::_sw:
				if (res1.b2 == 255u)
					printf("$%u %s(%u)", res1.b1, getdata(res2.ui).c_str(), res2.ui);
				else
					printf("$%u $%u %d", res1.b1, res1.b2, res2.i);
				break;

			case CommandType::_jr:
			case CommandType::_jalr:
				printf("$%u", res1.b1);
				break;
			case CommandType::_b:
			case CommandType::_j:
			case CommandType::_jal:
				lebal = ((unsigned int)res1.b3 << 8) + (unsigned int)res1.b2;
				printf("%s(%d)", getname(lebal).c_str(), lebal.ui);
				break;
			case CommandType::_beq:
			case CommandType::_bne:
			case CommandType::_bge:
			case CommandType::_ble:
			case CommandType::_bgt:
			case CommandType::_blt:
				lebal = ((unsigned int)res1.b3 << 8) + (unsigned int)res1.b2;
				if(res1.b1 & (1 << 7))
					printf("$%u $%u %s(%u)", res1.b1 & 0x3f, res2.b0, getname(lebal.ui).c_str(), lebal.ui);
				else
					printf("$%u %u %s(%u)", res1.b1 & 0x3f, res2.ui, getname(lebal.ui).c_str(), lebal.ui);
				break;
			case CommandType::_beqz:
			case CommandType::_bnez:
			case CommandType::_bgez:
			case CommandType::_blez:
			case CommandType::_bgtz:
			case CommandType::_bltz:
				lebal = ((unsigned int)res1.b3 << 8) + (unsigned int)res1.b2;
				printf("$%u %s(%u)", res1.b1, getname(lebal.ui).c_str(), lebal.ui);
				break;
			}
			puts("");
		}

		void out() {
			for (unsigned int i = 1; i < codeLimit; i++) {
				string na = getname(i);
				if (na != "none")
					std::cout << na << ":  ";
				output(i);
			}
		}
	#endif
};
#endif // !__MipsParser
