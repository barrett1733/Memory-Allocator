#include "MemoryAllocator.h"
#include <exception>
#include <cassert>

bool MemoryAllocator::canSplitBlock(MemoryBlock &mb, size_t size)
{
	if (mb.getSize() >= MemoryBlock::MEMORY_OVERHEAD * 2 + size + MIN_BLOCK_SIZE)
		return true;
	else
		return false;
}

void MemoryAllocator::splitBlock(MemoryBlock &mb, size_t size)
{
	uint total_size = mb.getSize();
	mb.setSize(size);
	mb.next()->setSize(total_size - mb.getSize() - MemoryBlock::MEMORY_OVERHEAD);
	mb.next()->dealloc();
}

void MemoryAllocator::createPool(size_t size)
{
	mSize = size;
	mCounter = 0;

	//mTable.createTable(size, (uint)mPool);

	mPool = new uchar[mSize + MemoryBlock::MEMORY_OVERHEAD];
	mPoolEnd = mPool + mSize + MemoryBlock::MEMORY_OVERHEAD;
	
	MemoryBlock mem_block;
	mem_block.setAddr(mPool);
	mem_block.dealloc();
	mem_block.setSize(mSize);
	mFreeList.add((LinkedList::Node*)mem_block.getData());
}

void MemoryAllocator::destroyPool()
{
	delete[] mPool;
	mPool = nullptr;
}

void* MemoryAllocator::alloc(size_t size)
{
	assert(mFreeList.size() != 0);
	MemoryBlock mem_block;
	LinkedList::Node* node = mFreeList.begin();

	uint new_size = (size >= MIN_BLOCK_SIZE ? size : MIN_BLOCK_SIZE);
	while (node != mFreeList.end())
	{
		mem_block.setAddrFromDataAddr(node);
		if (mem_block.getSize() >= new_size + MemoryBlock::MEMORY_OVERHEAD)
		{
			if (canSplitBlock(mem_block, new_size))
			{
				splitBlock(mem_block, new_size);
				mFreeList.add((LinkedList::Node*)mem_block.next()->getData());
			}

			++mCounter;
			
			//mTable[mFree.getData()] = (uintptr_t)mFree.getData();
			mem_block.alloc();
			mFreeList.remove(node);

			break;
		}
		node = node->next;
	}
	//mem_block.clear();
	return mem_block.getData();
}

void MemoryAllocator::dealloc(void *p)
{
	if (p >= mPool && p < mPoolEnd)
	{
		--mCounter;
		MemoryBlock mem_block;
		mem_block.setAddrFromDataAddr(p);
		mem_block.dealloc();
		
		if ((uchar*)mem_block.getAddr() != mPool && !mem_block.prev()->isAlloc())
		{
			mFreeList.remove((LinkedList::Node*)mem_block.prev()->getData());
			mem_block.fuse(*mem_block.prev());
		}
		if ((uchar*)mem_block.getAddr() != mPoolEnd && !mem_block.next()->isAlloc())
		{
			mFreeList.remove((LinkedList::Node*)mem_block.next()->getData());
			mem_block.fuse(*mem_block.next());
		}
		//mem_block.clear();
		mFreeList.add((LinkedList::Node*)mem_block.getData());
	}
}

MemoryBlock MemoryAllocator::begin()
{
	return MemoryBlock(mPool);
}

MemoryBlock MemoryAllocator::end()
{
	return MemoryBlock(mPoolEnd);
}
