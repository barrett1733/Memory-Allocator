#pragma once
/*
	From:
	COMPUTATION TOOLS 2012 : The Third International Conference on 
	Computational Logics, Algebras, Programming, Tools, and Benchmarking

	Fast Efficient Fixed-Size Memory Pool
	No Loops and No Overhead
	
	ISBN: 978-1-61208-222-6

	Ben Kenwright
	School of Computer Science
	Newcastle University
	Newcastle, United Kingdom,
	b.kenwright@ncl.ac.uk
*/
#include <iostream>
class Pool
{ // Basic type define
	typedef unsigned int uint;
	typedef unsigned char uchar;

	uint m_numOfBlocks; // Num of blocks
	uint m_sizeOfEachBlock; // Size of each block
	uint m_numFreeBlocks; // Num of remaining blocks
	uint m_numInitialized; // Num of initialized blocks
	uchar* m_memStart; // Beginning of memory pool
	uchar* m_next; // Num of next free block
public:
	Pool();
	~Pool();
	void CreatePool(size_t sizeOfEachBlock,	uint numOfBlocks);
	void DestroyPool();
	uchar* AddrFromIndex(uint i) const;
	uint IndexFromAddr(const uchar* p) const;
	void* Allocate();
	void DeAllocate(void* p);
};