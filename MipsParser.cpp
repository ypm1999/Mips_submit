#include "MipsParser.h"


void MipsParser::__initialization() {
	if (regMap.size() != 0)
		return;
	regMap[""] = -1;
	for (unsigned int i = 0; i < 32; i++)
		regMap[string("$") + std::to_string(i)] = i;
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


string MipsParser::getSingleString(unsigned int &pos) const {
	const string &code = *sourceCode;
	while (code[pos] == ' ' || code[pos] == ',' || code[pos] == '\t' 
		|| code[pos] == '\n' || code[pos] == '\r' || code[pos] == '#') {
		if (code[pos] == '#')
			while (code[pos] != '\n')
				pos++;
		pos++;
	}
	string res = "";
	if (code[pos] == '\"') {
		pos++;
		while (code[pos] != '\"') {
			if (code[pos] == '\\') {
				pos++;
				switch (code[pos]) {
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
					res += '\n';
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
				res += code[pos];
			pos++;
		}
		pos++;
		return std::move(res);
	}
	bool flag = true;
	while (pos < sourceCode->length() && flag) {
		switch (code[pos]) {
		case ' ': case ',': case '\t': case '\n': case '\r':
			flag = false;
			break;
		default:
			res += code[pos];
		}
		pos++;
	}
	return std::move(res);
}

bool MipsParser::getData(unsigned int &pos, Data &res) const {
	unsigned int lastpos = pos;
	if (pos >= sourceCode->length())
		return false;
	string tmp = getSingleString(pos);
	auto it = dataNameMap.find(tmp);
	if (it == dataNameMap.end()) {
		pos = lastpos;
		return false;
	}
	res.type = (*it).second;
	
	switch ((*it).second) {

	case DataType::_ascii: {
		res.data = getSingleString(pos);
		break;
	}
	case DataType::_asciiz: {
		res.data = getSingleString(pos) + '\0';
		break;
	}
	case DataType::_byte: {
		string data = getSingleString(pos);
		lastpos = pos;
		res.data = "";
		while (data.length() && (isdigit(data[0]) || (data[0] == '-' && isdigit(data[1])))) {
			res.data += (char)stoi(data);
			lastpos = pos;
			data = getSingleString(pos);
		}
		pos = lastpos;
		break;
	}
	case DataType::_half: {
		string data = getSingleString(pos);
		lastpos = pos;
		res.data = "";
		while (data.length() && (isdigit(data[0]) || (data[0] == '-' && isdigit(data[1])))) {
			Half tmp = (short)stoi(data);
			res.data += tmp.b0;
			res.data += tmp.b1;
			lastpos = pos;
			data = getSingleString(pos);
		}
		pos = lastpos;
		break;
	}
	case DataType::_word: {
		string data = getSingleString(pos);
		lastpos = pos;
		res.data = "";
		while (data.length() && (isdigit(data[0]) || (data[0] == '-' && isdigit(data[1])))) {
			Word tmp = stoi(data);
			res.data += tmp.b0;
			res.data += tmp.b1;
			res.data += tmp.b2;
			res.data += tmp.b3;
			lastpos = pos;
			data = getSingleString(pos);
		}
		pos = lastpos;
		break;
	}
	case DataType::_space:
		res.data.resize(stoi(getSingleString(pos)));
		break;
	case DataType::_align: {
		res.data = getSingleString(pos);
		break;
	}
	default:
		throw command_not_found(tmp);
	}
	return true;
}

bool MipsParser::getCommand(unsigned int &pos) {
	unsigned int lastpos = pos;
	if (pos >= sourceCode->length())
		return false;
	string tmp = getSingleString(pos);
	auto it = commandNameMap.find(tmp);
	if (it == commandNameMap.end()) {
		pos = lastpos;
		return false;
	}
	Word res1 = 0xffffffff, res2 = 0;
	res1.b0 = (char)((*it).second);
	switch ((*it).second) {
	//rs rd src
	case CommandType::_add:
	case CommandType::_addu:
	case CommandType::_sub:
	case CommandType::_subu:
	case CommandType::_addiu:
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
		string rd = getSingleString(pos);
		string rs = getSingleString(pos);
		string rt = getSingleString(pos);
		res1.b1 = regMap[rd];
		res1.b2 = regMap[rs];
		if (rt[0] == '$')
			res1.b3 = regMap[rt];
		else
			res2 = stoi(rt);
		break;
	}

	// rd rs
	case CommandType::_neg:
	case CommandType::_negu:
	case CommandType::_move: {
		string rd = getSingleString(pos);
		string rs = getSingleString(pos);
		res1.b1 = regMap[rd];
		res1.b2 = regMap[rs];
		break;
	}
	//rd rs / rd rs rt
	case CommandType::_mul:
	case CommandType::_mulu:
	case CommandType::_div:
	case CommandType::_divu: {
		string rd = getSingleString(pos);
		string rs = getSingleString(pos);
		unsigned int lastpos = pos;
		string rt = getSingleString(pos);

		if (rt[0] == '$' || (rt[0] >= '0' && rt[0] <= '9')) {
			res1.b1 = regMap[rd];
			res1.b2 = regMap[rs];
			if (rt[0] == '$')
				res1.b3 = regMap[rt];
			else
				res2 = stoi(rt);
		}
		else {
			pos = lastpos;
			res1.b2 = regMap[rd];
			if (rs[0] == '$')
				res1.b3 = regMap[rs];
			else
				res2 = stoi(rs);
		}
		break;
	}
	//rd con
	case CommandType::_li: {
		string rd = getSingleString(pos);
		string cons = getSingleString(pos);
		res1.b1 = regMap[rd];
		res2 = stoi(cons);
		break;
	}

	//rd
	case CommandType::_mfhi:
	case CommandType::_mflo:
	case CommandType::_jr:
	case CommandType::_jalr: {
		string rd = getSingleString(pos);
		res1.b1 = regMap[rd];
		break;
	}


	//special for option with label
	//res.b1 = rs, res1.b2b3 = label, res2 = imm, res2.b0 = rt;
	//label
	case CommandType::_b:
	case CommandType::_j:
	case CommandType::_jal: {
		string label = getSingleString(pos);
		(*table)[label].push_back(mem->top());
		break;
	}

	//rs label
	case CommandType::_beqz:
	case CommandType::_bnez:
	case CommandType::_bgez:
	case CommandType::_blez:
	case CommandType::_bgtz:
	case CommandType::_bltz: {
		string rs = getSingleString(pos);
		string label = getSingleString(pos);
		(*table)[label].push_back(mem->top());
		res1.b1 = regMap[rs];
		break;
	}

	//rs rt label
	case CommandType::_beq:
	case CommandType::_bne:
	case CommandType::_bge:
	case CommandType::_ble:
	case CommandType::_bgt:
	case CommandType::_blt: {
		string rs = getSingleString(pos);
		string rt = getSingleString(pos);
		string label = getSingleString(pos);
		(*table)[label].push_back(mem->top());
		res1.b1 = regMap[rs];
		if (rt[0] == '$')
			res2.b0 = regMap[rt], res1.b1 |= 1 << 7;
		else
			res2 = stoi(rt);
		break;
	}

	//rd address
	case CommandType::_la:
	case CommandType::_lb:
	case CommandType::_lh:
	case CommandType::_lw:
	case CommandType::_sb:
	case CommandType::_sh:
	case CommandType::_sw: {
		string rd = getSingleString(pos);
		string ad = getSingleString(pos);
		res1.b1 = regMap[rd];
		//TODO calc address;
		if (ad[0] == '-' || (ad[0] >= '0' && ad[0] <= '9')) {
			int flag = 1;
			int t = 0, re = 0;
			if (ad[0] == '-')
				flag = -1, t++;
			while (ad[t] != '(')
				re = re * 10 + ad[t++] - '0';
			res2.i = re * flag;
			ad = ad.substr(t + 1);
			ad.pop_back();
		}
		if (ad[0] == '$')
			res1.b2 = regMap[ad];
		else {
			(*table)[ad].push_back(mem->top());
			res2 = -1;
		}
		break;
	}

	case CommandType::_nop:
	case CommandType::_syscall:
		break;
	case CommandType::none:
	default:
		throw command_not_found(tmp);
	}
	mem->getSpace(res1.ui + ((unsigned long long)res2.ui << 32));
	return true;
}

void MipsParser::getDataBlock(unsigned int &pos, vector<Data> &res) const {
	Data tmp;
	unsigned int lastpos;
	while (true) {
		lastpos = pos;
		if (!getData(pos, tmp)) {
			pos = lastpos;
			break;
		}
		res.push_back(tmp);
	}
	if (res.size() == 0)
		throw empty_label();
}

Word MipsParser::getCommandBlock(unsigned int &pos) {
	Word tmp = mem->top();
	while (getCommand(pos));
	return tmp;
}


void MipsParser::getText() {
	unsigned int pos = 0;
	bool text = 0;
	while (pos < sourceCode->length()) {
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
		if (lastpos == 0)
			throw without_text_or_data();
		if (name.back() != ':')
			name = "", pos = lastpos;
		else
			name.pop_back();
		if (text) {
			Word ad = getCommandBlock(pos);
			if (name != ""){
				(*commandMap)[name] = ad.ui >> 3;
			}

		}
		else {
			pair<string, vector<Data> > res;
			res.first = name;
			getDataBlock(pos, res.second);
			data->push_back(res);
		}
	}
	mem->getSpace((unsigned long long)CommandType::none);
}

void MipsParser::writeData() {
	for (auto i : *data) {
		if (i.first != "")
			(*dataMap)[i.first] = mem->top();
		for (auto j : i.second) {
			if (j.type != DataType::_align)
				mem->getSpace(j.data.length(), j.data.c_str());
			else
				mem->algin(stoi(j.data));
		}
	}
}

void MipsParser::matchLabel() {
	for (auto i : *table) {
		auto &vec = i.second;
		int tmp = vec.front();
		CommandType com = (CommandType)mem->getWord(tmp).b0;
		if (com >= CommandType::_la && com <= CommandType::_sw) {
			auto it = dataMap->find(i.first);
			if (it == dataMap->end())
				throw function_not_defined(i.first + "(Data)");
			for (auto j : vec) {
				mem->writeWord(j + (commandSize >> 1), (*it).second);
			}
		}
		else {
			auto it = commandMap->find(i.first);
			if (it == commandMap->end())
				throw function_not_defined(i.first);
			for (auto j : vec) {
				mem->writeHalf(j + (commandSize >> 2), (short)(*it).second);
			}
		}
	}
}


MipsParser::MipsParser() noexcept {
	__initialization();
}

bool MipsParser::work(const string &str, Memory *_mem) {
	codeLimit = 0;
	entry = 0;
	mem = _mem;

	try {
		data = new vector< pair<string, vector<Data> > >;
		table = new map<string, vector<int> >;
		commandMap = new map<string, Word>;
		dataMap = new map<string, Word>;
		sourceCode = new string(str);

		getText();
		codeLimit = mem->top() >> 3;
		writeData();
		matchLabel();
		entry = commandMap->find(string("main"))->second.ui;
		if (entry == 0)
			throw without_main();

		delete data;
		delete table;
#ifndef DEBUG
		delete commandMap;
		delete dataMap;
		delete sourceCode;
		mem = nullptr;
#endif // !DEBUG
	}
	catch (...) {
		return false;
	}
	return true;
}
