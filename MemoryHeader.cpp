#include "MemoryHeader.h"
#include <exception>

MemoryBlock::MemoryBlock() : mBlock(nullptr)
{
}

MemoryBlock::MemoryBlock(void *p) : mBlock((uchar*)p)
{
}

MemoryBlock * MemoryBlock::getAddr() const
{
	return (MemoryBlock*)mBlock;
}

void MemoryBlock::setAddr(void * p)
{
	mBlock = (uchar*)p;
}

void MemoryBlock::setAddrFromDataAddr(void * p)
{
	mBlock = (uchar*)p - DATA_START_INDEX;
}

void MemoryBlock::alloc()
{
	mBlock[ALLOC_INDEX] = 1;
}

void MemoryBlock::dealloc()
{
	mBlock[ALLOC_INDEX] = 0;
}

bool MemoryBlock::isAlloc()
{
	return mBlock[ALLOC_INDEX] == 0 ? false : true;
}

MemoryBlock::uint MemoryBlock::getSize() const
{
	return *(uint*)&mBlock[SIZE_INDEX];
}

void MemoryBlock::setSize(uint size)
{
	*(uint*)&mBlock[SIZE_INDEX] = size + MEMORY_OVERHEAD;
	*(uint*)&mBlock[DATA_START_INDEX + size] = size + MEMORY_OVERHEAD;
}

void* MemoryBlock::getData()
{
	return &mBlock[DATA_START_INDEX];
}

void MemoryBlock::clear()
{
	for (int i = 0; i < getSize() - MEMORY_OVERHEAD; ++i)
	{
		mBlock[i + DATA_START_INDEX] = 0xcd;
	}
}

MemoryBlock* MemoryBlock::next()
{
	return (MemoryBlock*)(mBlock + *(uint*)&mBlock[SIZE_INDEX]);
}

MemoryBlock* MemoryBlock::prev()
{
	return (MemoryBlock*)(mBlock - *(uint*)(mBlock - FOOTER_SIZE));
}

void MemoryBlock::swap(MemoryBlock & mb)
{
	uint totalsize = getSize() + mb.getSize();
	for (uint i = 0; i < mb.getSize(); ++i)
	{
		mBlock[i] = mb.mBlock[i];
	}
	mb = next();
	mb.setSize(totalsize - getSize() - MEMORY_OVERHEAD);
}

void MemoryBlock::split(size_t size)
{
	if (size + MEMORY_OVERHEAD * 2 > getSize())
		throw std::bad_alloc();
	uint total = getSize();
	setSize(size);
	next()->setSize(total - getSize() - MEMORY_OVERHEAD);
	next()->dealloc();
}

void MemoryBlock::fuse(MemoryBlock &mb)
{
	if (mb.mBlock < mBlock)
	{
		mb.setSize(getSize() + mb.getSize() - MemoryBlock::MEMORY_OVERHEAD);
		mBlock = mb.mBlock;
	}
	else
	{
		setSize(getSize() + mb.getSize() - MemoryBlock::MEMORY_OVERHEAD);
		mb.mBlock = mBlock;
	}
}

