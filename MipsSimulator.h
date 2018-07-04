#ifndef __MipsSimulator
#define __MipsSimulator

#include <algorithm>
#include "Format.h"
#include "MispParser.h"


class MipsSimulator {
	MipsParser *code;
	Memory *mem;
	Word reg[32 + 3];
	Word &hi = reg[32];
	Word &lo = reg[33];

	Word runCommand(const int &i){
		const Command &com = code->command[i];
		Word tmp, ad;

		if (com.rt.ui != 255) tmp = reg[com.rt.ui];
		else tmp = com.cons;

		if (com.address.i != -1) ad = com.address;
		else ad = reg[com.rd.ui].i + com.offset.i;

		switch (com._type){
		//--------------------------# plus and minus #---------------------
		case CommandType::_add:
			reg[com.rs.ui].i = reg[com.rd.ui].i + tmp.i;
			break;
		case CommandType::_addu:
		case CommandType::_addiu:
			reg[com.rs.ui].ui = reg[com.rd.ui].ui + tmp.ui;
			break;
		case CommandType::_sub:
			reg[com.rs.ui].i = reg[com.rd.ui].i - tmp.i;
			break;
		case CommandType::_subu:
		case CommandType::_subiu:
			reg[com.rs.ui].ui = reg[com.rd.ui].ui - tmp.ui;
			break;
		//--------------------------# mul and div #---------------------
		case CommandType::_mul:
			if (com.rs.ui != 255)
				reg[com.rs.ui].i = reg[com.rd.ui].i * tmp.i;
			else {
				unsigned long long t = 1ll * reg[com.rd.ui].i * tmp.i;
				hi.i = t >> 32;
				lo.i = (unsigned int)t;
			}
			break;
		case CommandType::_mulu:
			if (com.rs.ui != 255)
				reg[com.rs.ui].ui = reg[com.rd.ui].ui * tmp.ui;
			else {
				unsigned long long t = 1llu * reg[com.rd.ui].ui * tmp.ui;
				hi.ui = t >> 32;
				lo.ui = (unsigned int)t;
			}
			break;
		case CommandType::_div:
			if (com.rs.ui != 255)
				reg[com.rs.ui].i = reg[com.rd.ui].i / tmp.i;
			else {
				lo.i = reg[com.rd.ui].i / tmp.i;
				hi.i = reg[com.rd.ui].i - lo.i * tmp.i;
			}
			break;
		case CommandType::_divu:
			if (com.rs.ui != 255)
				reg[com.rs.ui].ui = reg[com.rd.ui].ui / tmp.ui;
			else {
				lo.ui = reg[com.rd.ui].ui / tmp.ui;
				hi.ui = reg[com.rd.ui].ui - lo.ui * tmp.ui;
			}
			break;
		//------------------------# xor #----------------------
		case CommandType::_xor:
			reg[com.rs.ui].i = reg[com.rd.ui].i ^ tmp.i;
			break;
		case CommandType::_xoru:
			reg[com.rs.ui].ui = reg[com.rd.ui].ui ^ tmp.ui;
			break;
		//----------------------# negitave #---------------
		case CommandType::_neg:
			reg[com.rs.ui].i = -reg[com.rd.ui].i;
			break;
		case CommandType::_negu:
			reg[com.rs.ui].ui = ~reg[com.rd.ui].ui;
			break;
		//----------------------# %(rest) #---------------
		case CommandType::_rem:
			reg[com.rs.ui].i = reg[com.rd.ui].i % tmp.i;
			break;
		case CommandType::_remu:
			reg[com.rs.ui].ui = reg[com.rd.ui].ui % tmp.ui;
			break;
		//----------------------# load imm #---------------
		case CommandType::_li:
			reg[com.rs.ui].i = com.cons.i;
			break;
		//----------------------# cmp #---------------
		case CommandType::_seq:
			reg[com.rs.ui].i = reg[com.rd.ui].i == tmp.i;
			break;
		case CommandType::_sge:
			reg[com.rs.ui].i = reg[com.rd.ui].i >= tmp.i;
			break;
		case CommandType::_sgt:
			reg[com.rs.ui].i = reg[com.rd.ui].i > tmp.i;
			break;
		case CommandType::_sle:
			reg[com.rs.ui].i = reg[com.rd.ui].i <= tmp.i;
			break;
		case CommandType::_slt:
			reg[com.rs.ui].i = reg[com.rd.ui].i < tmp.i;
			break;
		case CommandType::_sne:
			reg[com.rs.ui].i = reg[com.rd.ui].i != tmp.i;
			break;

		//----------------------# branch #---------------
		case CommandType::_b:
			return ad;
			break;
		case CommandType::_beq:
			if (reg[com.rd.ui].i == tmp.i)
				return ad;
			break;
		case CommandType::_bne:
			if (reg[com.rd.ui].i != tmp.i)
				return ad;
			break;
		case CommandType::_bge:
			if (reg[com.rd.ui].i >= tmp.i)
				return ad;
			break;
		case CommandType::_ble:
			if (reg[com.rd.ui].i <= tmp.i)
				return ad;
			break;
		case CommandType::_bgt:
			if (reg[com.rd.ui].i > tmp.i)
				return ad;
			break;
		case CommandType::_blt:
			if (reg[com.rd.ui].i < tmp.i)
				return ad;
			break;
		//----------------# branch, cmp with zero #---------------
		case CommandType::_beqz:
			if (reg[com.rd.ui].i == 0)
				return ad;
			break;
		case CommandType::_bnez:
			if (reg[com.rd.ui].i != 0)
				return ad;
			break;
		case CommandType::_bgez:
			if (reg[com.rd.ui].i >= 0)
				return ad;
			break;
		case CommandType::_blez:
			if (reg[com.rd.ui].i <= 0)
				return ad;
			break;
		case CommandType::_bgtz:
			if (reg[com.rd.ui].i > 0)
				return ad;
			break;
		case CommandType::_bltz:
			if (reg[com.rd.ui].i < 0)
				return ad;
			break;

		//----------------------# jump #---------------
		case CommandType::_j:
			return ad;
			break;
		case CommandType::_jr:
			return reg[com.rs.ui];
			break;
		case CommandType::_jal:
			reg[31] = i + 1;
			return ad;
			break;
		case CommandType::_jalr:
			reg[31] = i + 1;
			return reg[com.rs.ui];
			break;
		//----------------------# move #---------------
		case CommandType::_move:
			reg[com.rs.ui] = reg[com.rd.ui];
			break;
		case CommandType::_mfhi:
			reg[com.rs.ui] = hi;
			break;
		case CommandType::_mflo:
			reg[com.rs.ui] = lo;
			break;
		//----------------------# load #---------------
		case CommandType::_la:
			reg[com.rs.ui] = ad;
			break;
		case CommandType::_lb:
			reg[com.rs.ui] = mem->getByte(ad).i;
			break;
		case CommandType::_lh:
			reg[com.rs.ui] = mem->getHalf(ad).i;
			break;
		case CommandType::_lw:
			reg[com.rs.ui] = mem->getWord(ad).i;
			break;
		//----------------------# store #---------------
		case CommandType::_sb:
			mem->writeByte(ad, reg[com.rs.ui].i);
			break;
		case CommandType::_sh:
			mem->writeHalf(ad, reg[com.rs.ui].i);
			break;
		case CommandType::_sw:
			mem->writeWord(ad, reg[com.rs.ui].i);
			break;
		//----------------------# move #---------------
		case CommandType::_nop:
			break;
		case CommandType::_syscall: {
			switch (reg[2].i) {
			case 1:
				std::cout << reg[4].i;
				break;
			case 4:
				std::cout << mem->getString(reg[4]);
				break;
			case 5:
				std::cin >> reg[2].i;
				break;
			case 8: {
				string str;
				std::cin >> str;
				mem->writeString(reg[4], reg[5].i, str);
				break;
			}
			case 9:
				mem->algin(2);
				reg[2] = mem->getSpace(reg[4].i);
				break;
			case 10:
				return EXIT_WITHOUT_VALUE;
				break;
			case 17:
				return EXIT_WITH_VALUE;
				break;
			default:
				throw run_command_error("syscall");
			}
			break;
		}

		case CommandType::none:
		default:
			throw command_not_found();
		}
		return -1;
	}

	Word runBlock(const Word &address){
		for (int i = address.i; i < code->command.size(); i++) {
			Word next = runCommand(i);
			if (next.i != -1)
				return next;
		}
		return 0x3fffffff;
	}

	void __init() {
		memset(reg, 0, sizeof(reg));
		reg[29] = mem->top();
	}

public:
	MipsSimulator(MipsParser *_code = nullptr, Memory *_mem = nullptr)
		:code(_code), mem(_mem) {}

	bool run(int &value) {
		__init();
		auto main = code->commandMap.find("main");
		if (main == code->commandMap.end())
			throw without_main();
		auto now = (*main).second;
		while (true) {
			string name = code->getname(now);
			Word next = runBlock(now);
			if (next.i == EXIT_WITHOUT_VALUE)
				return false;
			if (next.i == EXIT_WITH_VALUE) {
				value = reg[4].i;
				return true;
			}
			if (next.i < 0 || next.ui > code->command.size()) {
				throw command_address_error(next.i);
			}

			now = next;
		}
		throw command_address_error(-1);
	}
};

#endif // !__MipsSimulator
