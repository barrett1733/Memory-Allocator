#pragma once
#include "MemoryHeader.h"

class MemoryDefrag
{
	MemoryBlock mCurrent, mNext;
public:
	void run(MemoryBlock& start, MemoryBlock& end);
};

