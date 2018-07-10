#ifndef __MipsSimulator
#define __MipsSimulator

#include <algorithm>
#include <bitset>
#include "Format.h"
#include "MipsParser.h"
using std::bitset;
using std::cout;
using std::cin;
using std::endl;

class MipsSimulator {
	MipsParser *code;
	Word Entry;
	unsigned int codeLimit;
	Memory *mem;
	bool withReturnValue;
	int returnValue;

	Word reg[32 + 3];
	bool regLock[32];
	Word &hi = reg[32];
	Word &lo = reg[33];
	Word &pc = reg[34];

	static const unsigned int addressLen = 10;
	static const unsigned int addressBIT = (1 << addressLen) - 1;
	static const unsigned int pridLen = 6;
	static const unsigned int pridBIT = (1 << pridLen) - 1;
	bitset<1 << (addressLen + pridLen)> BHT;
	unsigned char BH[1 << addressLen];

	class IF_ID {
		friend class MipsSimulator;
		Word res1, res2;
		Word npc;
		Byte load, EXreg, MEMreg;
		Word EXdata, MEMdata;
		bool empty();
		void init();
		IF_ID() noexcept { init(); }
	}IFID, IFID1;

	class ID_EX {
		friend class MipsSimulator;
		CommandType com;
		Word npc, a, b, imm;
		Byte res;
		bool empty();
		void init();
		ID_EX() noexcept { init(); }
	}IDEX, IDEX1;

	class EX_MEM {
		friend class MipsSimulator;
		CommandType com;
		Word ALUout, address;
		Byte res;
		bool empty();
		void init();
		EX_MEM() noexcept { init(); }
	}EXMEM, EXMEM1;

	class MEM_WB {
		friend class MipsSimulator;
		CommandType com;
		Word result;
		Byte res;
		bool empty();
		void init();
		MEM_WB() noexcept { init(); }
	}MEMWB, MEMWB1;


	bool getBranch(unsigned int i);
	void changeBranch(unsigned int i, bool sta);

	bool getReg(const IF_ID & in, const unsigned char id, Word & res) const;

	bool IF(IF_ID & write);
	bool ID(const IF_ID & get, ID_EX & write);
	bool EX(const ID_EX & get, EX_MEM  & write);
	bool MEM(const EX_MEM & get, MEM_WB & write);
	void WB(const MEM_WB & get);

//	bool tik_tok();

public:
	MipsSimulator(MipsParser *_code = nullptr) :code(_code) {};
	bool with_returnValue() { return withReturnValue; }
	int get_returnValue() { return returnValue; }
	bool run(Word _Entry, unsigned int len, Memory *_mem);
	
//	void thread1(bool & finish);
	

	bool tik_tok1();
};



#endif // !__MipsSimulator
