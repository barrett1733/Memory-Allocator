#pragma once
class MemoryBlock
{
	typedef unsigned int uint;
	typedef unsigned char uchar;

	// Header = char Allocated + uint Size
	// Data
	// Footer = uint Size

	static const uint ALLOC_INDEX = 0;
	static const uint SIZE_INDEX = 1;
	static const uint HEADER_SIZE = sizeof(uint) + SIZE_INDEX;
	static const uint FOOTER_SIZE = sizeof(uint);
	static const uint DATA_START_INDEX = HEADER_SIZE;

	uchar* mBlock;

public:
	static const uint MEMORY_OVERHEAD = HEADER_SIZE + FOOTER_SIZE;
	MemoryBlock();
	MemoryBlock(void*);
	MemoryBlock* getAddr() const;
	void setAddr(void*);
	void setAddrFromDataAddr(void* p);
	void alloc();
	void dealloc();
	bool isAlloc();
	uint getSize() const; // Includes Memory Overhead
	void setSize(uint); // Automatically adds Memory Overhead
	void* getData();
	void clear();
	MemoryBlock* next();
	MemoryBlock* prev();

	void swap(MemoryBlock&); // Assumes the blocks are right next to each other
	void split(size_t); // Assumes their is enough room for split
	void fuse(MemoryBlock&);
};

