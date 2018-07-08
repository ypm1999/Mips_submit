#include "Memory.h"

Word Memory::getSpace(const unsigned int &len, const Byte _data[]) {
	if (used + len > size)
		throw memory_used_up();
	memcpy(data + used, _data, len);
	used += len;
	return used - len;
}

Word Memory::getSpace(const unsigned int &len, const char _data[]) {
	if (used + len > size)
		throw memory_used_up();
	memcpy(data + used, _data, len);
	used += len;
	return used - len;
}

Word Memory::getSpace(const unsigned int &len) {
	if (used + len > size)
		throw memory_used_up();
	used += len;
	return used - len;
}

Word Memory::getSpace(const Byte &_data) {
	if (used + 1 > size)
		throw memory_used_up();
	data[used++] = _data;
	return used - 1;
}

Word Memory::getSpace(const Half &_data) {
	if (used + 2 > size)
		throw memory_used_up();
	data[used++] = _data.b0;
	data[used++] = _data.b1;
	return used - 2;
}

Word Memory::getSpace(const Word &_data) {
	if (used + 4 > size)
		throw memory_used_up();
	memcpy(data + used, &_data.ui, 4);
	used += 4;
	return used - 4;
}

Word Memory::getSpace(const unsigned long long & _data) {
	memcpy(data + used, &_data, 8);
	used += 8;
	return used - 8;
}


void Memory::writeByte(const Word &address, const Byte &_data) {
	data[address.ui] = _data.i;
}

void Memory::writeHalf(const Word &address, const Half &_data) {
	data[address.i] = _data.b0;
	data[address.i + 1] = _data.b1;
}

void Memory::writeWord(const Word &address, const Word &_data) {
	memcpy(data + address.ui, &_data.ui, 4);
}

void Memory::writeString(const Word &address, const unsigned int &len, const string &str) {
	if (len < str.length())
		throw write_out_of_bound();
	memcpy(data + address.ui, str.c_str(), str.length());
}


void Memory::algin(const int &n) {
	if (((used >> n) << n) != used)
		used = ((used >> n) + 1) << n;
	if (used > size)
		throw memory_used_up();
}


Byte Memory::getByte(const Word &address) const {
	return data[address.ui];
}

Half Memory::getHalf(const Word &address) const {
	Half tmp;
	tmp.b0 = data[address.ui];
	tmp.b1 = data[address.ui + 1];
	return std::move(tmp);
}

Word Memory::getWord(const Word &address) const {
	Word tmp;
	tmp.b0 = data[address.ui];
	tmp.b1 = data[address.ui + 1];
	tmp.b2 = data[address.ui + 2];
	tmp.b3 = data[address.ui + 3];
	return std::move(tmp);
}

string Memory::getString(const Word &address) const {
	string s = "";
	int pos = address.ui;
	while (data[pos].i != 0) {
		s += data[pos++].i;
	}
	return s;
}


void Memory::out(int start) const {
	for (unsigned int i = start; i < used; i++) {
		std::cout << (unsigned int)data[i] << " \n"[(i + 1) % 8 == 0];
	}
	puts("--------------------------");
}

Memory::Memory(unsigned int _size) noexcept :used(commandSize), size(_size) {
	data = new Byte[_size];
	memset(data, 0, size);
}

Memory::~Memory() {
	delete[] data;
}
