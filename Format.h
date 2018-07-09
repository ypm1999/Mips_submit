#ifndef __Formats
#define __Formats

//#define DEBUG
#include <string>
const unsigned int commandSize = 8;

//union Word;
//union Half;

union Byte {
	char i;
	unsigned char ui;
	struct { unsigned char b0; };
	Byte(char _t = 0) noexcept :i(_t) {}
	operator unsigned char(){ return (unsigned char)i; }
	//operator Half();
	//operator Word();
};

union Half{
	short i;
	unsigned short ui;
	struct { unsigned char b0, b1; };
	Half(short _t = 0) noexcept :i(_t) {}
	operator short() { return i; }
	//operator Byte();
	//operator Word();
};

union Word {
	int i;
	unsigned int ui;
	struct { unsigned char b0, b1, b2, b3; };
	Word(int _t = 0) noexcept :i(_t) {}
	operator int() { return i; }
	//operator Byte();
	//operator Half();
};

//Byte::operator Half() { return Half(i); }
//Byte::operator Word() { return Word(i); }
//Half::operator Byte() { return b0; }
//Half::operator Word() { return Word(i); }
//Word::operator Byte() { return b0; }
//Word::operator Half() { return Half(i); }

enum class DataType:unsigned char {
	_ascii, _asciiz, _byte, _half, _word, _space, _align, none
};

enum class CommandType:unsigned char  {
	_add, _addu, _addiu, _sub, _subu,
	_mul, _mulu, _div, _divu,
	_xor, _xoru, _neg, _negu, _rem, _remu,
	_seq, _sge, _sgt, _sle, _slt, _sne,
	_move, _mfhi, _mflo,
	_beq, _bne, _bge, _ble, _bgt, _blt,
	_beqz, _bnez, _bgez, _blez, _bgtz, _bltz,
	_b, _j, _jr, _jal, _jalr,
	_li, _la, _lb, _lh, _lw,
	_sb, _sh, _sw,
	_nop, _syscall,
	_syscall1, _syscall4, _syscall5, _syscall8, _syscall9, _syscall10, _syscall17,
	none
};

#endif // !__Formats



/*
switch () {
//--------------------------# plus and minus #---------------------
case CommandType::_add:
case CommandType::_addu:
case CommandType::_addiu:
case CommandType::_sub:
case CommandType::_subu:
//--------------------------# mul and div #---------------------
case CommandType::_mul:
case CommandType::_mulu:
case CommandType::_div:
case CommandType::_divu:
//------------------------# xor #----------------------
case CommandType::_xor:
case CommandType::_xoru:
//----------------------# negitave #---------------
case CommandType::_neg:
case CommandType::_negu:
//----------------------# %(rest) #---------------
case CommandType::_rem:
case CommandType::_remu:
//----------------------# load imm #---------------
case CommandType::_li:
//----------------------# cmp #---------------
case CommandType::_seq:
case CommandType::_sge:
case CommandType::_sgt:
case CommandType::_sle:
case CommandType::_slt:
case CommandType::_sne:
//----------------------# branch #---------------
case CommandType::_b:
case CommandType::_beq:
case CommandType::_bne:
case CommandType::_bge:
case CommandType::_ble:
case CommandType::_bgt:
case CommandType::_blt:
//----------------# branch, cmp with zero #---------------
case CommandType::_beqz:
case CommandType::_bnez:
case CommandType::_bgez:
case CommandType::_blez:
case CommandType::_bgtz:
case CommandType::_bltz:
//----------------------# jump #---------------
case CommandType::_j:
case CommandType::_jr:
case CommandType::_jal:
case CommandType::_jalr:
//----------------------# move #---------------
case CommandType::_move:
case CommandType::_mfhi:
case CommandType::_mflo:
//----------------------# load #---------------
case CommandType::_la:
case CommandType::_lb:
case CommandType::_lh:
case CommandType::_lw:
//----------------------# store #---------------
case CommandType::_sb:
case CommandType::_sh:
case CommandType::_sw:
case CommandType::_nop:
case CommandType::_syscall:
default:
throw command_not_found();
}
*/
