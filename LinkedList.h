#pragma once
class LinkedList
{
public:
	struct Node
	{
		Node *prev, *next;
	};

private:
	Node *mHead;
	int mCount;

public:
	LinkedList();
	bool isEmpty() const;
	int size() const;
	void add(Node*);
	void remove(Node*);
	Node* begin() const;
	Node* end() const;
};

