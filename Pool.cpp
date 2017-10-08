#include "Pool.h"

Pool::Pool()
{
	m_numOfBlocks = 0;
	m_sizeOfEachBlock = 0;
	m_numFreeBlocks = 0;
	m_numInitialized = 0;
	m_memStart = NULL;
	m_next = 0;
}

Pool::~Pool()
{
	DestroyPool();
}

void Pool::CreatePool(size_t sizeOfEachBlock, uint numOfBlocks)
{
	m_numOfBlocks = numOfBlocks;
	m_sizeOfEachBlock = sizeOfEachBlock;
	m_memStart = new uchar[m_sizeOfEachBlock * m_numOfBlocks];
	m_numFreeBlocks = numOfBlocks;
	m_next = m_memStart;
}

void Pool::DestroyPool()
{
	delete[] m_memStart;
	m_memStart = NULL;
}

Pool::uchar* Pool::AddrFromIndex(uint i) const
{
	return m_memStart + (i * m_sizeOfEachBlock);
}

Pool::uint Pool::IndexFromAddr(const uchar* p) const
{
	return (((uint)(p - m_memStart)) / m_sizeOfEachBlock);
}

void* Pool::Allocate()
{
	if (m_numInitialized < m_numOfBlocks)
	{
		uint* p = (uint*)AddrFromIndex(m_numInitialized);
		*p = m_numInitialized + 1;
		m_numInitialized++;
	}
	void* ret = NULL;
	if (m_numFreeBlocks > 0)
	{
		ret = (void*)m_next;
		--m_numFreeBlocks;
		if (m_numFreeBlocks != 0)
		{
			m_next = AddrFromIndex(*((uint*)m_next));
		}
		else
		{
			m_next = NULL;
		}
	}
	return ret;
}

void Pool::DeAllocate(void* p)
{
	if (m_next != NULL)
	{
		(*(uint*)p) = IndexFromAddr(m_next);
		m_next = (uchar*)p;
	}
	else
	{
		*((uint*)p) = m_numOfBlocks;
		m_next = (uchar*)p;
	}
	++m_numFreeBlocks;
}