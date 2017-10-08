#pragma once

template <class T>
class FSPool
{
	typedef unsigned int uint;
	typedef unsigned char uchar;

	struct node
	{
		bool allocated;
		union
		{
			node* next;
			T data;
		};
	};

	const uint cmSize = sizeof(node);
	const uint cmNodeDataOffset = cmSize - sizeof(T);
	uint mAmt, mCounter;
	node *mPool, *mPoolEnd, *mNext, *mTemp;

public:
	void createPool(uint amt);
	void destroyPool();
	T* alloc();
	void dealloc(T*);
};


template <class T>
void FSPool<T>::createPool(uint amt)
{
	mAmt = amt;
	mCounter = 0;
	mPool = new node[mAmt];
	mPoolEnd = mPool + mAmt;
	mNext = mPool[0];

	for (mTemp = mNext; mTemp != mPoolEnd; ++mTemp)
	{
		mTemp->allocated = false;
		mTemp->next = mTemp + 1;
	}

}

template <class T>
void FSPool<T>::destroyPool()
{
	delete[] mPool;
	mPool = nullptr;
	mPoolEnd = nullptr;
	mNext = nullptr;
	mTemp = nullptr;
}

template <class T>
T* FSPool<T>::alloc()
{
	if (mNext + 1 > mPoolEnd)
	{
		return nullptr;
	}
	else
	{
		++mCounter;
		mTemp = mNext;
		mTemp->allocated = true;
		mNext = mNext->next;
		return mTemp->data;
	}
}

template <class T>
void FSPool<T>::dealloc(T *p)
{
	if (p >= mPool && p < mPoolEnd)
	{
		--mCounter;
		mTemp = (node*)(((void*)(p)) - cmNodeDataOffset);
		mTemp->allocated = false;
		mTemp->next = mNext;
		mNext = mTemp;
	}
}
