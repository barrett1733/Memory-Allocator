#pragma once

class ScratchPool
{
	typedef unsigned int uint;
	typedef unsigned char uchar;
	
	uchar *mPool, *mFree, *mTemp;
	size_t mSize, mCounter;

public:
	void createPool(size_t size);
	void destroyPool();
	void* alloc(size_t size);
	void dealloc();
};