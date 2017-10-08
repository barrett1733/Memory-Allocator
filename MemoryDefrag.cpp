#include "MemoryDefrag.h"

void MemoryDefrag::run(MemoryBlock & start, MemoryBlock & end)
{
	mCurrent = start;
	while (mCurrent.next()->getAddr() != end.getAddr())
	{
		mNext = mCurrent.next();
		if (!mCurrent.isAlloc() && !mNext.isAlloc())
		{
			mCurrent.fuse(mNext);
			//mCurrent.clear();
		}
		else 
		{
			if (!mCurrent.isAlloc() && mNext.isAlloc())
			{
				mCurrent.swap(mNext);
				//mNext.clear();
				mNext.dealloc();
			}
			mCurrent = mNext;
		}
	}
}
