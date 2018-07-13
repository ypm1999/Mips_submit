#ifndef __MipsSimulator
#define __MipsSimulator

#include <algorithm>
#include <bitset>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "Format.h"
#include "MipsParser.h"
using std::bitset;
using std::cout;
using std::cin;
using std::endl;
using std::atomic;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::condition_variable;
using std::this_thread::yield;

class MipsSimulator {
	MipsParser *code;
	Memory *mem;
	Word Entry;
	unsigned int codeLimit;

	bool withReturnValue;
	int returnValue;

	Word reg[32 + 3];
	unsigned char regLock[32];
	Word &hi = reg[32];
	Word &lo = reg[33];
	Word &pc = reg[34];
	Word pc0;

	static const unsigned int addressLen = 8;
	static const unsigned int addressBIT = (1 << addressLen) - 1;
	static const unsigned int pridLen = 4;
	static const unsigned int pridBIT = (1 << pridLen) - 1;
	bitset<1 << (addressLen + pridLen)> BHT;
	unsigned char BH[1 << addressLen];


	Byte load, EXreg, MEMreg;
	Word EXdata, MEMdata;

	bool End;
	atomic<bool> st1, st2, st3, st0, EXo, MEMo, finished;
	atomic<int> ready;


	class IF_ID {
		friend class MipsSimulator;
		Word res1, res2;
		Word npc;
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

	bool getReg(const unsigned char id, Word & res);

	void IF(IF_ID & write);
	void ID(const IF_ID & get, ID_EX & write);
	void EX(const ID_EX & get, EX_MEM  & write);
	void MEM(const EX_MEM & get, MEM_WB & write);
	void WB(const MEM_WB & get);

	void thread0();
	void thread1();
	void thread2();
	void thread3();


	void tik_tok();

	void doit(bool predictionFiled, bool jump);

public:
	MipsSimulator(MipsParser *_code = nullptr) noexcept :code(_code), mem(nullptr) {};
	bool with_returnValue() { return withReturnValue; }
	int get_returnValue() { return returnValue; }
	bool run(Word _Entry, unsigned int len, Memory *_mem);

};



#endif // !__MipsSimulator
