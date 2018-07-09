#ifndef __Memory
#define __Memory

#include <iostream>
#include <cstring>
#include "Format.h"
#include "Exceptions.h"

class Memory {
public:
	unsigned int used;
	const unsigned int size;
	Byte *data;
public:
	Word getSpace(const unsigned int &len, const Byte _data[]);
	Word getSpace(const unsigned int &len, const char _data[]);
	Word getSpace(const unsigned int &len);
	Word getSpace(const Byte &_data);
	Word getSpace(const Half &_data);
	Word getSpace(const Word &_data);
	Word getSpace(const unsigned long long &_data);

	void writeByte(const Word &address, const Byte &_data);
	void writeHalf(const Word &address, const Half &_data);
	void writeWord(const Word &address, const Word &_data);
	void writeString(const Word &address, const unsigned int &len, const string &str);

	void algin(const int &n);

	Byte getByte(const Word &address) const;
	Half getHalf(const Word &address) const;
	Word getWord(const Word &address) const;
	string getString(const Word &address) const;

	unsigned int top() const { return used; }
	unsigned int getSize() const { return size; }

#ifdef DEBUG
	void out(int start) const;
#endif // DEBUG

	Memory(unsigned int _size = 1024 * 1024 * 4) noexcept;
	~Memory() { delete[] data; }
};

#endif // !__Memory

