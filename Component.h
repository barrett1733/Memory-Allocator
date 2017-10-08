#pragma once
#include <iostream>
#include <ctime>

struct position
{
	float x, y, z;
	position() : x(0), y(0), z(0) {}
	position(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {}
};

template<typename T>
class PoolAllocator
{
	unsigned int mCounter, mAmt;
	struct node
	{
		bool active;
		union
		{
			position data;
			node *next;
		};
		node() {}
	};
	node *mPool;
	node *mPoolHead;
	node *mPoolCur;

public:
	PoolAllocator();
	~PoolAllocator();
	void createPool(unsigned int amt);
	void destroyPool();
	T* alloc();
	void dealloc(void*);
	unsigned int getCount();
};

template<typename T>
PoolAllocator<T>::PoolAllocator()
{
}

template<typename T>
PoolAllocator<T>::~PoolAllocator()
{
	destroyPool();
}

template<typename T>
void PoolAllocator<T>::createPool(unsigned int amt)
{
	mCounter = 0;
	mAmt = amt;
	mPool = new node[amt];
	mPoolHead = &mPool[0];
	mPool[amt - 1].next = &mPool[0];
	mPool[amt - 1].active = false;
	for (unsigned int i = 0; i < amt - 1; ++i)
	{
		mPool[i].next = &mPool[i + 1];
		mPool[i].active = false;
	}
}

template<typename T>
void PoolAllocator<T>::destroyPool()
{
	delete[] mPool;
	mPool = NULL;
}

template<typename T>
T* PoolAllocator<T>::alloc()
{
	if (mCounter == mAmt)
	{
		mPoolCur = NULL;
		mPoolHead = NULL;
		return NULL;
	}
	else
	{
		++mCounter;
		mPoolCur = mPoolHead;
		mPoolCur->active = true;
		mPoolHead = mPoolHead->next;
		return &mPoolCur->data;
	}
}

template<typename T>
void PoolAllocator<T>::dealloc(void *p)
{
	--mCounter;
	mPoolCur = static_cast<node*>(p);
	mPoolCur->active = false;
	mPoolCur->next = mPoolHead;
	mPoolHead = mPoolCur;
}

template<typename T>
unsigned int PoolAllocator<T>::getCount()
{
	return mCounter;
}
