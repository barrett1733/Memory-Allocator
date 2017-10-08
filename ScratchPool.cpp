#include "ScratchPool.h"

void ScratchPool::createPool(size_t size)
{
	mPool = new uchar[size];
	mSize = size;
	mCounter = 0;
	mFree = &mPool[0];
}

void ScratchPool::destroyPool()
{
	delete[] mPool;
	mPool = nullptr;
	mFree = nullptr;
}

void *ScratchPool::alloc(size_t size)
{
	if (mFree + size > mPool + mSize)
		return nullptr;
	else
	{
		++mCounter;
		mTemp = mFree;
		mFree += size;
		return mTemp;
	}
}

void ScratchPool::dealloc()
{
	--mCounter;
	if (mCounter == 0)
		mFree = &mPool[0];
}
