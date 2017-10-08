#pragma once
#include "LinkedList.h" 
#include "Block.h"

class BlockAllocator
{
	typedef unsigned int uint;
	typedef unsigned char uchar;
	static const uint MIN_BLOCK_SIZE = Block::MEMORY_OVERHEAD + sizeof(LinkedList::Node);

	// Block = Header + Data
	uint mSize, mCounter;
	uchar *mPool, *mPoolEnd;
	LinkedList mFreeList;
	//MemoryTable mTable;
	bool canSplitBlock(Block&, size_t);
	void splitBlock(Block&, size_t);
	void fuse(Block*, Block*); // Assumes the blocks are next to each other

public:
	void createPool(size_t);
	void destroyPool();
	void* alloc(size_t);
	void dealloc(void*);

	Block* begin();
	Block* end();
};
