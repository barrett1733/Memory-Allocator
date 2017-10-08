#include "Block.h"
#include <exception>

Block* Block::convDataAddress(void * p)
{
	return (Block*)((uchar*)p - DATA_START_INDEX);
}

void Block::alloc()
{
	((uchar*)this)[ALLOC_INDEX] = 1;
}

void Block::dealloc()
{
	((uchar*)this)[ALLOC_INDEX] = 0;
}

bool Block::isAlloc()
{
	return ((uchar*)this)[ALLOC_INDEX] == 0 ? false : true;
}

Block::uint Block::getSize() const
{
	return *(uint*)&((uchar*)this)[SIZE_INDEX];
}

void Block::setSize(uint size)
{
	*(uint*)&((uchar*)this)[SIZE_INDEX] = size + MEMORY_OVERHEAD;
	*(uint*)&((uchar*)this)[DATA_START_INDEX + size] = size + MEMORY_OVERHEAD;
}

void* Block::getData()
{
	return &((uchar*)this)[DATA_START_INDEX];
}

void Block::clear()
{
	for (int i = 0; i < getSize() - MEMORY_OVERHEAD; ++i)
	{
		((uchar*)this)[i + DATA_START_INDEX] = 0xcd;
	}
}

Block* Block::next()
{
	return (Block*)(((uchar*)this) + *(uint*)&((uchar*)this)[SIZE_INDEX]);
}

Block* Block::prev()
{
	return (Block*)(((uchar*)this) - *(uint*)(((uchar*)this) - FOOTER_SIZE));
}
