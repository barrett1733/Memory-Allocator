#pragma once
#include "MemoryHeader.h"
#include <inttypes.h>

class MemoryTable
{
	typedef unsigned int uint;
	typedef unsigned char uchar;
	static const uint MIN_BLOCK_SIZE = 
		MemoryBlock::MEMORY_OVERHEAD + sizeof(char);

	// table[hashed pointer] = pointer

	uint mSize, mOffset;
	uintptr_t *mMemoryTable;
	uint hash(void*);
public:
	void createTable(size_t, uint offset);
	void destroyTable();
	uintptr_t& operator[](void*);
};

