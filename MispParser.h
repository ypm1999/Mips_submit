#ifndef __MipsParser
#define __MipsParser

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Format.h"
#include "Memory.h"
#include "Exceptions.h"
using namespace std;

// TODO: Format the text into entries
class MipsParser {
	friend class MipsSimulator;
	string s;
	vector<Command> command;
	map<string, Word> commandMap;
	map<string, Word> dataMap;
	map<string, vector<size_t>> table;

	string getSingleString(unsigned int &pos) const{
		while (s[pos] == ' ' || s[pos] == ',' || s[pos] == '\t' || s[pos] == '\n' || s[pos] == '\r' || s[pos] == '#') {
			if(s[pos] == '#')
				while (s[pos] != '\n')
				pos++;
			pos++;
		}
		string res = "";
		if (s[pos] == '\"') {
			pos++;
			while (s[pos] != '\"') {
				if (s[pos] == '\\') {
					pos++;
					switch (s[pos]) {
					case 'a':
						res += '\a';
						break;
					case 'b':
						res += '\b';
						break;
					case 'f':
						res += '\f';
						break;
					case 'n':
						res+= '\n';
						break;
					case 'r':
						res += '\r';
						break;
					case '0':
						res += '\0';
						break;
					case '?':
						res += '\?';
						break;
					case 'v':
						res += '\v';
						break;
					case '\\':
						res += '\\';
						break;
					case '\'':
						res += '\'';
						break;
					case '\"':
						res += '\"';
						break;
					}
				}
				else
					res += s[pos];
				pos++;
			}
			pos++;
			return std::move(res);
		}
		bool flag = true;
		while (pos < s.length() && flag) {
			switch (s[pos]) {
			case ' ': case ',': case '\t': case '\n': case '\r':
				flag = false;
				break;
			default:
				res += s[pos];
			}
			pos++;
		}
		return std::move(res);
	}

	//This function will put the data in mem.
	Data getData(unsigned int &pos, Memory &mem) const{
		Data res;
		unsigned int lastpos = pos;
		if (pos >= s.length())
			return std::move(res);
		string tmp = getSingleString(pos);
		auto it = dataNameMap.find(tmp);
		if (it == dataNameMap.end()) {
			lastpos = pos;
			return std::move(res);
		}
		res._type = (*it).second;

		switch (res._type) {

		case DataType::_ascii: {
			string data = getSingleString(pos);
			res.address = mem.getSpace(data.length(), data.c_str());
			break;
		}
		case DataType::_asciiz: {
			string data = getSingleString(pos);
			res.address = mem.getSpace(data.length() + 1, data.c_str());
			break;
		}
		case DataType::_byte: {
			string data = getSingleString(pos);
			Byte tmp = stoi(data);
			res.address = mem.getSpace(tmp);
			unsigned int lastpos = pos;
			data = getSingleString(pos);
			while (data[0] >= '0' && data[0] <= '9') {
				mem.getSpace(tmp);
				lastpos = pos;
				data = getSingleString(pos);
			}
			pos = lastpos;
			break;
		}
		case DataType::_half: {
			string data = getSingleString(pos);
			Half tmp = stoi(data);
			res.address = mem.getSpace(tmp);
			unsigned int lastpos = pos;
			data = getSingleString(pos);
			while (data[0] >= '0' && data[0] <= '9') {
				mem.getSpace(tmp);
				lastpos = pos;
				data = getSingleString(pos);
			}
			pos = lastpos;
			break;
		}
		case DataType::_word: {
			string data = getSingleString(pos);
			Word tmp = stoi(data);
			res.address = mem.getSpace(tmp);
			unsigned int lastpos = pos;
			data = getSingleString(pos);
			while (data[0] >= '0' && data[0] <= '9') {
				mem.getSpace(tmp);
				lastpos = pos;
				data = getSingleString(pos);
			}
			pos = lastpos;
			break;
		}
		case DataType::_space: {
			string num = getSingleString(pos);
			res.address = mem.getSpace(stoi(num));
			break;
		}
		case DataType::_align: {
			mem.algin(stoi(getSingleString(pos)));
			break;
		}
		default :
			throw command_not_found(tmp);
		}

		return std::move(res);
	}

	Command getCommand(unsigned int &pos){
		Command res;
		unsigned int lastpos = pos;
		if (pos >= s.length())
			return std::move(res);
		string tmp = getSingleString(pos);
		auto it = commandNameMap.find(tmp);
		if (it == commandNameMap.end()) {
			pos = lastpos;
			return std::move(res);
		}
		res._type = (*it).second;
		switch (res._type) {
		//rs rd src
		case CommandType::_add:
		case CommandType::_addu:
		case CommandType::_sub:
		case CommandType::_subu:
		case CommandType::_addiu:
		case CommandType::_subiu:
		case CommandType::_xor:
		case CommandType::_xoru:
		case CommandType::_rem:
		case CommandType::_remu:
		case CommandType::_seq:
		case CommandType::_sge:
		case CommandType::_sgt:
		case CommandType::_sle:
		case CommandType::_slt:
		case CommandType::_sne: {
			string rs = getSingleString(pos);
			string rd = getSingleString(pos);
			string rt = getSingleString(pos);
			res.rs = regMap[rs];
			res.rd = regMap[rd];
			if (rt[0] == '$')
				res.rt = regMap[rt];
			else
				res.cons = stoi(rt);
			break;
		}

		//rs rd
		case CommandType::_neg:
		case CommandType::_negu:
		case CommandType::_move: {
			string rs = getSingleString(pos);
			string rd = getSingleString(pos);
			res.rs = regMap[rs];
			res.rd = regMap[rd];
			break;
		}
		//rs rd / rs rd rt
		case CommandType::_mul:
		case CommandType::_mulu:
		case CommandType::_div:
		case CommandType::_divu: {
			string rs = getSingleString(pos);
			string rd = getSingleString(pos);
			int lastpos = pos;
			string rt = getSingleString(pos);

			if (rt[0] == '$' || (rt[0] >= '0' && rt[0] <= '9')) {
				res.rs = regMap[rs];
				res.rd = regMap[rd];
				if (rt[0] == '$')
					res.rt = regMap[rt];
				else
					res.cons = stoi(rt);
			}
			else {
				pos = lastpos;
				res.rd = regMap[rs];
				if (rd[0] == '$')
					res.rt = regMap[rd];
				else
					res.cons = stoi(rd);
			}
			break;
		}
		//rs con
		case CommandType::_li: {
			string rs = getSingleString(pos);
			string con = getSingleString(pos);
			res.rs = regMap[rs];
			res.cons = stoi(con);
			break;
		}

		//rs
		case CommandType::_mfhi:
		case CommandType::_mflo:
		case CommandType::_jr:
		case CommandType::_jalr: {
			string rs = getSingleString(pos);
			res.rs = regMap[rs];
			break;
		}

		//label
		case CommandType::_b:
		case CommandType::_j:
		case CommandType::_jal: {
			string label = getSingleString(pos);
			table[label].push_back(command.size());
			break;
		}

		//rd label
		case CommandType::_beqz:
		case CommandType::_bnez:
		case CommandType::_bgez:
		case CommandType::_blez:
		case CommandType::_bgtz:
		case CommandType::_bltz: {
			string rd= getSingleString(pos);
			string label = getSingleString(pos);
			table[label].push_back(command.size());
			res.rd = regMap[rd];
			break;
		}

		//rd rt label
		case CommandType::_beq:
		case CommandType::_bne:
		case CommandType::_bge:
		case CommandType::_ble:
		case CommandType::_bgt:
		case CommandType::_blt: {
			string rd = getSingleString(pos);
			string rt = getSingleString(pos);
			string label = getSingleString(pos);
			table[label].push_back(command.size());
			res.rd = regMap[rd];
			if (rt[0] == '$')
				res.rt = regMap[rt];
			else
				res.cons = stoi(rt);
			break;
		}

		//rs address
		case CommandType::_la:
		case CommandType::_lb:
		case CommandType::_lh:
		case CommandType::_lw:
		case CommandType::_sb:
		case CommandType::_sh:
		case CommandType::_sw: {
			string rs = getSingleString(pos);
			string ad = getSingleString(pos);
			res.rs = regMap[rs];
			//TODO calc address;
			if (ad[0] == '-' || (ad[0] >= '0' && ad[0] <= '9')) {
				int flag = 1;
				int t = 0, re = 0;
				if (ad[0] == '-')
					flag = -1, t++;
				while (ad[t] != '(')
					re = re * 10 + ad[t++] - '0';
				res.offset = re * flag;
				ad = ad.substr(t + 1);
				ad.pop_back();
			}
			if (ad[0] == '$') {
				res.rd = regMap[ad];
			}
			else {
				table[ad].push_back(command.size());
				res.address = 0;
			}
			break;
		}

		case CommandType::_nop:
		case CommandType::_syscall:
		case CommandType::none:
			break;
		default :
			throw command_not_found(tmp);
		}
		return std::move(res);
	}

	Word getDataBlock(unsigned int &pos, Memory &mem) const {
		Word ad(-1);
		while (true) {
			unsigned int lastpos = pos;
			Data tmp = getData(pos, mem);
			if (tmp._type == DataType::none) {
				pos = lastpos;
				break;
			}
			if (ad.i == -1)
				ad = tmp.address;
		}
		if (ad.i == -1)
			throw empty_label();
		return std::move(ad);
	}

	Word getCommandBlock(unsigned int &pos, Memory &mem){
		int tmp = command.size() ;
		while (true) {
			unsigned int lastpos = pos;
			Command t = getCommand(pos);
			if (t._type == CommandType::none) {
				pos = lastpos;
				break;
			}
			command.push_back(t);
		}
		return tmp;
	}
public:

	MipsParser(const string &str) :s(str) {}
	~MipsParser() = default;

	bool parser(Memory &mem) {
		try {
			unsigned int pos = 0;
			bool text = 0;
			while (pos < s.length()) {
				int lastpos = pos;
				string  name = getSingleString(pos);
				if (name == "")
					break;
				if (name == ".text") {
					text = true;
					continue;
				}
				if (name == ".data") {
					text = false;
					continue;
				}
				if (lastpos == 0) {
					cerr << "ERROR! Code don't have .text or .data at beginning!" << endl;
					return false;
				}
				if (name.back() != ':')
					name = "", pos = lastpos;
				else
					name.pop_back();
				if (text) {
					Word ad = getCommandBlock(pos, mem);
					if (name != "")
						commandMap[name] = ad;
				}
				else {
					Word ad = getDataBlock(pos, mem);
					if (name != "")
						dataMap[name] = ad;
				}
			}
			for (auto i : table) {
				auto &vec = i.second;
				if (command[vec.front()].address.i == 0) {
					auto it = dataMap.find(i.first);
					if (it == dataMap.end())
						throw function_not_defined(i.first);
					for (auto j : vec) {
						command[j].address = (*it).second;
					}
				}
				else {
					auto it = commandMap.find(i.first);
					if (it == commandMap.end())
						throw function_not_defined(i.first);
					for (auto j : vec) {
						command[j].address = (*it).second;
					}
				}
			}
		}
		catch (...) {
			cerr << "Unknow Error with parser!" << endl;
			return false;
		}
#ifdef DEBUG
		
		for(int i = 0; i < command.size(); i++) {
			string na = getname(i);
			if(na != "none")
				cout << na << ":  ";
			command[i].out();
		}

#endif
		return true;
	}
	string getname(Word address) {
		for (auto j : commandMap)
			if (j.second.i == address.i)
				return j.first;
		return "none";
	}
};
#endif // !__MipsParser
