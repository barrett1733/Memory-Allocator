#pragma once
#include "MemoryHeader.h"
#include "MemoryTable.h"
#include "LinkedList.h" 

class MemoryAllocator
{
	typedef unsigned int uint;
	typedef unsigned char uchar;
	static const uint MIN_BLOCK_SIZE = MemoryBlock::MEMORY_OVERHEAD + sizeof(LinkedList::Node);
	
	// Block = Header + Data
	uint mSize, mCounter;
	uchar *mPool, *mPoolEnd;
	LinkedList mFreeList;
	//MemoryTable mTable;
	bool canSplitBlock(MemoryBlock&, size_t);
	void splitBlock(MemoryBlock&, size_t);

public:
	void createPool(size_t);
	void destroyPool();
	void* alloc(size_t);
	void dealloc(void*);

	MemoryBlock begin();
	MemoryBlock end();
};

