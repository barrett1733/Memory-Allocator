#include "FixedSizePool.h"

FixedSizePool::uchar* FixedSizePool::indexToPointer(index i)
{
	return &mPool[i];
}

FixedSizePool::index FixedSizePool::pointerToIndex(void *p)
{
	return (uchar*)p - mPool;
}

FixedSizePool::index FixedSizePool::getIndex(index block)
{
	index* temp = (index*)&mPool[block + cmHeaderSize];
	return *temp;
}

void FixedSizePool::storeIndex(index block, index to_store)
{
	index* temp = (index*)&mPool[block + cmHeaderSize];
	*temp = to_store;
}

void FixedSizePool::createPool(size_t size, uint amt)
{
	// Data must be able to at least store the size of a pointer
	uint mDataSize = size < sizeof(index) ? sizeof(index) : size;
	mBlockSize = mDataSize + cmHeaderSize;
	mAmt = amt;
	mCounter = 0;

	mPool = new uchar[mBlockSize * mAmt];
	mStart = 0;
	mNext = mStart;

	for (mTemp = mStart; mTemp < mAmt * mBlockSize; mTemp += mBlockSize)
	{
		mPool[mTemp] = false;
		storeIndex(mTemp, mTemp + mBlockSize);
	}
}

void FixedSizePool::destroyPool()
{
	delete[] mPool;
	mPool = nullptr;
}

void* FixedSizePool::alloc()
{
	if (mNext + mBlockSize > mAmt * mBlockSize)
	{
		return nullptr;
	}
	else
	{
		++mCounter;
		mTemp = mNext;
		mNext = getIndex(mNext);
		mPool[mTemp] = true;
		return indexToPointer(mTemp) + cmHeaderSize;
	}
}

void FixedSizePool::dealloc(void *p)
{
	if (p >= mPool && p < mPool + mAmt * mBlockSize)
	{
		--mCounter;
		mTemp = pointerToIndex(p) - cmHeaderSize;
		mPool[mTemp] = false;
		storeIndex(mTemp, mNext);
		mNext = mTemp;
	}
}

void* FixedSizePool::operator[](uint i)
{
	if (i < mAmt)
		return &mPool[i * mBlockSize] + cmHeaderSize;
	else
		return nullptr;
}

bool FixedSizePool::isAllocated(uint i)
{
	// should throw
	if (i < mAmt)
		return mPool[i * mBlockSize];
	else
		return false;
}
