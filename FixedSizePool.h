#pragma once

class FixedSizePool
{
	typedef unsigned int uint;
	typedef unsigned char uchar;
	typedef uint index;

	// Block = Header + Data
	const uint cmHeaderSize = sizeof(bool);
	uint mBlockSize, mAmt, mCounter;
	uchar *mPool;
	index mStart, mNext, mTemp;

	uchar* indexToPointer(index);
	index pointerToIndex(void*);
	index getIndex(index block);
	void storeIndex(index block, index to_store);
public:
	void createPool(size_t size, uint amt);
	void destroyPool();
	void* alloc();
	void dealloc(void*);
	void* operator[](uint i);
	bool isAllocated(uint);
};
