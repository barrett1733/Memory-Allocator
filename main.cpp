#include <iostream>
#include <ctime>
#include "MemoryAllocator.h"
#include "MemoryDefrag.h"
#include "BlockAllocator.h"
#include "FixedSizePool.h"

using namespace std;

const int times = 1000;
const int amt = 1000;
std::clock_t start;

struct position
{
	float a, b, c;
	position() :a(0), b(0), c(0) {}
	position(int a, int b, int c) :a(a), b(b), c(c) {}
};

const position pos = { 1,2,3 };

double getDuration(std::clock_t start)
{
	return (std::clock() - start) / (double)CLOCKS_PER_SEC;
}

double testFixedSize(double times)
{
	int i;
	FixedSizePool pool;
	pool.createPool(sizeof(position), amt);
	start = std::clock();
	for (i = 0; i < amt; ++i)
	{
		pool.alloc();
		*(position*)pool[i] = position(i + times, i + times + 1, i + times + 2);
	}
	for (i = 0; i < amt; ++i)
	{
		pool.dealloc(pool[i]);
	}
	double end = getDuration(start);
	return end;
}

double testBlockAllocator(double times)
{
	int i;
	BlockAllocator pool;
	pool.createPool((20 + Block::MEMORY_OVERHEAD) * amt);
	position **p = new position*[amt];
	start = std::clock();
	for (i = 0; i < amt; ++i)
	{
		p[i] = (position*)pool.alloc(sizeof(position));
		*p[i] = position(i + times, i + times + 1, i + times + 2);
	}
	for (i = 0; i < amt; ++i)
	{
		pool.dealloc(p[i]);
	}
	double end = getDuration(start);
	delete[] p;
	return end;
}

double testNew(double times)
{
	int i;
	position **p = new position*[amt];
	start = std::clock();
	for (i = 0; i < amt; ++i)
	{
		p[i] = new position(i + times, i + times + 1, i + times + 2);
	}
	for (i = 0; i < amt; ++i)
	{
		delete p[i];
	}
	delete[] p;
	double end = getDuration(start);
	return end;
}

double test2()
{
	Block* block;
	BlockAllocator pool;
	pool.createPool((20 + Block::MEMORY_OVERHEAD) * amt);
	start = std::clock();
	for (block = pool.begin(); block != pool.end();)
	{
		pool.alloc(sizeof(position));
		if(block->isAlloc())
			*((position*)block->getData()) = pos;
		block = block->next();
	}
	for (block = pool.begin(); block != pool.end();)
	{
		if (block->isAlloc())
			pool.dealloc(block->getData());
		block = block->next();
	}
	double end = getDuration(start);
	return end;
}

void test()
{
	double sum = 0;

	cout << "BlockAllocator\n";
	for (int i = 0; i < times; i++)
		sum += testBlockAllocator(times);
	cout << "Time: " << fixed << sum / times << endl;
	sum = 0;

	cout << "BlockAllocator2\n";
	for (int i = 0; i < times; i++)
		sum += test2();
	cout << "Time: " << fixed << sum / times << endl;
	sum = 0;

	cout << "New\n";
	for (int i = 0; i < times; i++)
		sum += testNew(times);
	cout << "Time: " << fixed << sum / times << endl;
	sum = 0;

	cout << "FixedSize\n";
	for (int i = 0; i < times; i++)
		sum += testFixedSize(times);
	cout << "Time: " << fixed << sum / times << endl;
	sum = 0;

}

void pretest()
{
	position **p = new position*[5];

	BlockAllocator pool;
	pool.createPool(20000);

	for (int i = 0; i < 5; ++i)
		p[i] = (position*)pool.alloc(sizeof(position));

	for (int i = 0; i < 5; ++i)
		cout << p[i] << endl;

	pool.dealloc(p[1]);
	pool.dealloc(p[0]);
	pool.dealloc(p[4]);

	p[1] = (position*)pool.alloc(sizeof(position));
	p[0] = (position*)pool.alloc(sizeof(position));
	p[4] = (position*)pool.alloc(sizeof(position));

	pool.destroyPool();

	delete[] p;
	std::cin.ignore();
}

int main()
{
	test();
	//pretest();

	std::cin.ignore();
}