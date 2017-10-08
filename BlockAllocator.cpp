#include "BlockAllocator.h"
#include <exception>
#include <cassert>

bool BlockAllocator::canSplitBlock(Block &mb, size_t size)
{
	if (mb.getSize() >= Block::MEMORY_OVERHEAD * 2 + size + MIN_BLOCK_SIZE)
		return true;
	else
		return false;
}

void BlockAllocator::splitBlock(Block &mb, size_t size)
{
	uint total_size = mb.getSize();
	mb.setSize(size);
	mb.next()->setSize(total_size - mb.getSize() - Block::MEMORY_OVERHEAD);
	mb.next()->dealloc();
}

void BlockAllocator::fuse(Block *mb1, Block *mb2)
{
	if (mb2 < mb1)
	{
		mb2->setSize(mb1->getSize() + mb2->getSize() - Block::MEMORY_OVERHEAD);
		mb1 = mb2;
	}
	else
	{
		mb1->setSize(mb1->getSize() + mb2->getSize() - Block::MEMORY_OVERHEAD);
		mb2 = mb1;
	}
}

void BlockAllocator::createPool(size_t size)
{
	mSize = size;
	mCounter = 0;

	//mTable.createTable(size, (uint)mPool);

	mPool = new uchar[mSize + Block::MEMORY_OVERHEAD];
	mPoolEnd = mPool + mSize + Block::MEMORY_OVERHEAD;

	Block* mem_block = (Block*)mPool;
	mem_block->dealloc();
	mem_block->setSize(mSize);
	mFreeList.add((LinkedList::Node*)mem_block->getData());
}

void BlockAllocator::destroyPool()
{
	delete[] mPool;
	mPool = nullptr;
}

void* BlockAllocator::alloc(size_t size)
{
	Block *mem_block = nullptr;
	LinkedList::Node* node = mFreeList.begin();
	assert(node != nullptr);

	uint new_size = (size >= MIN_BLOCK_SIZE ? size : MIN_BLOCK_SIZE);
	while (node != mFreeList.end())
	{
		mem_block = Block::convDataAddress(node);
		if (mem_block->getSize() >= new_size + Block::MEMORY_OVERHEAD)
		{
			if (canSplitBlock(*mem_block, new_size))
			{
				splitBlock(*mem_block, new_size);
				mFreeList.add((LinkedList::Node*)mem_block->next()->getData());
			}

			++mCounter;
			mem_block->alloc();
			mFreeList.remove(node);

			break;
		}
		node = node->next;
	}
	//mem_block.clear();
	return mem_block->getData();
}

void BlockAllocator::dealloc(void *p)
{
	if (p >= mPool && p < mPoolEnd)
	{
		--mCounter;
		Block *mem_block = Block::convDataAddress(p);
		mem_block->dealloc();

		if ((uchar*)mem_block != mPool && !mem_block->prev()->isAlloc())
		{
			mFreeList.remove((LinkedList::Node*)mem_block->prev()->getData());
			fuse(mem_block, mem_block->prev());
		}
		else if ((uchar*)mem_block != mPoolEnd && !mem_block->next()->isAlloc())
		{
			mFreeList.remove((LinkedList::Node*)mem_block->next()->getData());
			fuse(mem_block, mem_block->next());
		}
		//mem_block.clear();
		mFreeList.add((LinkedList::Node*)mem_block->getData());
	}
}

Block* BlockAllocator::begin()
{
	return (Block*)mPool;
}

Block* BlockAllocator::end()
{
	return (Block*)mPoolEnd;
}
