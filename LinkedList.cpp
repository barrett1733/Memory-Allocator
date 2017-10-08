#include "LinkedList.h"



LinkedList::LinkedList() : mHead(nullptr), mCount(0)
{
}

bool LinkedList::isEmpty() const
{
	return mHead;
}

int LinkedList::size() const
{
	return mCount;
}

void LinkedList::add(Node *p)
{
	if (mCount == 0)
	{
		mHead = p;
		mHead->prev = nullptr;
		mHead->next = nullptr;
		++mCount;
	}
	else
	{
		p->prev = nullptr;
		p->next = mHead;
		mHead->prev = p;
		mHead = p;
		++mCount;
	}
}

void LinkedList::remove(Node *node)
{
	if (mCount == 1)
	{
		mHead = nullptr;
		--mCount;
	}
	else if (mCount != 0)
	{
		if (node->next == nullptr)
		{
			node->prev->next = nullptr;
		}
		else if (node->prev == nullptr)
		{
			mHead = node->next;
			mHead->prev = nullptr;
		}
		else
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
		--mCount;
	}
}

LinkedList::Node * LinkedList::begin() const
{
	return mHead;
}

LinkedList::Node * LinkedList::end() const
{
	return nullptr;
}
