#include "MemoryTable.h"

MemoryTable::uint MemoryTable::hash(void *p)
{
	return ((uint)p - mOffset) / MIN_BLOCK_SIZE;
}

void MemoryTable::createTable(size_t size, uint offset)
{
	mOffset = offset;
	mSize = size / MIN_BLOCK_SIZE;
	mMemoryTable = new uintptr_t[mSize];
}

void MemoryTable::destroyTable()
{
	delete[] mMemoryTable;
	mMemoryTable = nullptr;
}

uintptr_t & MemoryTable::operator[](void *p)
{
	return mMemoryTable[hash(p)];
}
