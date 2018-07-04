#ifndef __Formats
#define __Formats

#include <vector>
#include <string>
using namespace std;
//#define DEBUG

const int EXIT_WITHOUT_VALUE = -2;
const int EXIT_WITH_VALUE = -3;
const int memorySize = 1024 * 1024 * 16;

union Byte {
	char i;
	unsigned char ui;
	Byte(char _t = 0) :i(_t) {}
	operator char(){
		return i;
	}
};

union Half{
	short i;
	unsigned short ui;
	struct {char b0, b1; };
	Half(short _t = 0) :i(_t) {}
};

union Word {
	int i;
	unsigned int ui;
	struct { char b0, b1, b2, b3; };
	Word(int _t = 0) :i(_t) {}
};

enum class DataType:char {
	_ascii, _asciiz, _byte, _half, _word, _space, _align, none
};

enum class CommandType:char  {
	_label,
	_add, _addu, _addiu, _sub, _subu, _subiu,
	_mul, _mulu, _div, _divu,
	_xor, _xoru, _neg, _negu, _rem, _remu,
	_seq, _sge, _sgt, _sle, _slt, _sne,
	_b, _beq, _bne, _bge, _ble, _bgt, _blt,
	_beqz, _bnez, _bgez, _blez, _bgtz, _bltz,
	_j, _jr, _jal, _jalr,
	_li, _la, _lb, _lh, _lw,
	_sb, _sh, _sw,
	_move, _mfhi, _mflo,
	_nop, _syscall,
	none
};

class Data {
	friend class MipsParser;
	friend class MipsSimulator;
	DataType _type;
	Word address;
public:
	Data(DataType d = DataType::none, Word ad = -1)
		:_type(d), address(ad){}
	~Data() = default;
	void out() {
		printf("data: %d %d\n", _type, address.i);
	}
};

class Command {
	friend class MipsParser;
	friend class MipsSimulator;
	CommandType _type;
	Byte rs, rd, rt;
	Word cons, address, offset;
public:
	Command(CommandType c = CommandType::none, Byte r1 = 255, Byte r2 = 255, Byte r3 = 255, Word con = 0, Word ad = -1, Word of = 0)
		:_type(c), rs(r1), rd(r2), rt(r3), cons(con), address(ad), offset(of) {}
	~Command() = default;
	void out() {
		printf("command: %d %d %d %d %d %d %d\n", _type, rs.i, rd.i, rt.i, cons.i, address.i, offset.i);
	//	printf("command: %d %d %d\n", _type, address.i, offset.i);
	}
};
//
//class CommandBlock {
//	friend class MipsParser;
//	friend class MipsSimulator;
//	std::string name;
//	Word address;
//public:
//	CommandBlock(string na = "") :name(na), address(-1) {}
//	~CommandBlock() = default;
//};
//
//class DataBlock {
//	friend class MipsParser;
//	friend class MipsSimulator;
//	std::string name;
//	Word address;
//public:
//	DataBlock(std::string na = "") :name(na), address(-1) {}
//	~DataBlock() = default;
//};

map<string, CommandType> commandNameMap;
map<string, DataType> dataNameMap;
map<string, Byte> regMap;

void __initialization() {
	regMap[""] = -1;
	for(int i = 0; i < 32; i++)
		regMap[string("$") + to_string(i)] = i;
	regMap["$zero"] = 0;
	regMap["$at"] = 1;
	regMap["$v0"] = 2;
	regMap["$v1"] = 3;
	regMap["$a0"] = 4;
	regMap["$a1"] = 5;
	regMap["$a2"] = 6;
	regMap["$a3"] = 7;
	regMap["$t0"] = 8;
	regMap["$t1"] = 9;
	regMap["$t2"] = 10;
	regMap["$t3"] = 11;
	regMap["$t4"] = 12;
	regMap["$t5"] = 13;
	regMap["$t6"] = 14;
	regMap["$t7"] = 15;
	regMap["$s0"] = 16;
	regMap["$s1"] = 17;
	regMap["$s2"] = 18;
	regMap["$s3"] = 19;
	regMap["$s4"] = 20;
	regMap["$s5"] = 21;
	regMap["$s6"] = 22;
	regMap["$s7"] = 23;
	regMap["$t8"] = 24;
	regMap["$t9"] = 25;
	regMap["$k0"] = 26;
	regMap["$k1"] = 27;
	regMap["$gp"] = 28;
	regMap["$sp"] = 29;
	regMap["$fp"] = 30;
	regMap["$ra"] = 31;

	dataNameMap[""] = DataType::none;
	dataNameMap[".ascii"] = DataType::_ascii;
	dataNameMap[".asciiz"] = DataType::_asciiz;
	dataNameMap[".byte"] = DataType::_byte;
	dataNameMap[".half"] = DataType::_half;
	dataNameMap[".word"] = DataType::_word;
	dataNameMap[".space"] = DataType::_space;
	dataNameMap[".align"] = DataType::_align;
	commandNameMap[""] = CommandType::none;
	commandNameMap["add"] = CommandType::_add;
	commandNameMap["addu"] = CommandType::_addu;
	commandNameMap["addiu"] = CommandType::_addiu;
	commandNameMap["sub"] = CommandType::_sub;
	commandNameMap["subu"] = CommandType::_subu;
	commandNameMap["subiu"] = CommandType::_subiu;
	commandNameMap["mul"] = CommandType::_mul;
	commandNameMap["mulu"] = CommandType::_mulu;
	commandNameMap["div"] = CommandType::_div;
	commandNameMap["divu"] = CommandType::_divu;
	commandNameMap["xor"] = CommandType::_xor;
	commandNameMap["xoru"] = CommandType::_xoru;
	commandNameMap["neg"] = CommandType::_neg;
	commandNameMap["negu"] = CommandType::_negu;
	commandNameMap["rem"] = CommandType::_rem;
	commandNameMap["remu"] = CommandType::_remu;
	commandNameMap["seq"] = CommandType::_seq;
	commandNameMap["sge"] = CommandType::_sge;
	commandNameMap["sgt"] = CommandType::_sgt;
	commandNameMap["sle"] = CommandType::_sle;
	commandNameMap["slt"] = CommandType::_slt;
	commandNameMap["sne"] = CommandType::_sne;
	commandNameMap["b"] = CommandType::_b;
	commandNameMap["beq"] = CommandType::_beq;
	commandNameMap["bne"] = CommandType::_bne;
	commandNameMap["bge"] = CommandType::_bge;
	commandNameMap["ble"] = CommandType::_ble;
	commandNameMap["bgt"] = CommandType::_bgt;
	commandNameMap["blt"] = CommandType::_blt;
	commandNameMap["beqz"] = CommandType::_beqz;
	commandNameMap["bnez"] = CommandType::_bnez;
	commandNameMap["bgez"] = CommandType::_bgez;
	commandNameMap["blez"] = CommandType::_blez;
	commandNameMap["bgtz"] = CommandType::_bgtz;
	commandNameMap["bltz"] = CommandType::_bltz;
	commandNameMap["j"] = CommandType::_j;
	commandNameMap["jr"] = CommandType::_jr;
	commandNameMap["jal"] = CommandType::_jal;
	commandNameMap["jalr"] = CommandType::_jalr;
	commandNameMap["li"] = CommandType::_li;
	commandNameMap["la"] = CommandType::_la;
	commandNameMap["lb"] = CommandType::_lb;
	commandNameMap["lh"] = CommandType::_lh;
	commandNameMap["lw"] = CommandType::_lw;
	commandNameMap["sb"] = CommandType::_sb;
	commandNameMap["sh"] = CommandType::_sh;
	commandNameMap["sw"] = CommandType::_sw;
	commandNameMap["move"] = CommandType::_move;
	commandNameMap["mfhi"] = CommandType::_mfhi;
	commandNameMap["mflo"] = CommandType::_mflo;
	commandNameMap["nop"] = CommandType::_nop;
	commandNameMap["syscall"] = CommandType::_syscall;
}


#endif // !__Formats
