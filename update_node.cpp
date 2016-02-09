// https://habrahabr.ru/post/276673/
// c++ implementation
// complexity O(n) - need to touch every node in tree
// memory O(n) - on case this algo used maximum (H(k) + H(k-1)) items. n = sum(2^0, 2^k), H - number of items in k level of tree max 2^k
// for this test tree it will be 3 + 2

#include "stdafx.h"

#include <list>
#include <queue>

template <typename T>
struct node_t
{
	node_t* left;
	node_t* right;

	node_t* rs; // right sibling

	node_t(const T& d)
		: data(d)
		, left(nullptr)
		, right(nullptr)
		, rs(nullptr)
	{}

	T data;
};

typedef node_t<int> node;

int main()
{
	std::list<node*> nodes; // to completely remove all data

/* 
    build tree
	     1
	   /   \
    2     3
	 /     /  \
  4     5    6
*/

	node* root(new node(1));
	nodes.push_back(root);

	{
		node* parent = root;
		node* nl = new node(2);
		node* nr = new node(3);
		parent->left = nl;
		parent->right = nr;
		nodes.push_back(nl);
		nodes.push_back(nr);

		parent = nl;
		nl = new node(4);
		parent->left = nl;
		nodes.push_back(nl);

		parent = nr;
		nl = new node(5);
		nr = new node(6);
		parent->left = nl;
		parent->right = nr;
		nodes.push_back(nl);
		nodes.push_back(nr);
	}

	// build complete

	// start with root set right sibling to all nodes

	node* nl = nullptr;

	std::queue<node*>* qn(new std::queue<node*>); // current level
	std::queue<node*>* next(new std::queue<node*>); // next level of tree maximum n elements

	qn->push(root);

	while (!qn->empty())
	{
		node* n = qn->front();
		qn->pop();

		if (nullptr != nl)
		{
			nl->rs = n;
		}

		nl = n;

		if (nullptr != n->left)
		{
			next->push(n->left);
		}

		if (nullptr != n->right)
		{
			next->push(n->right);
		}

		if (qn->empty())
		{
			std::swap(qn, next);
			nl = nullptr;
		}
	}

	// clear

	delete qn;
	delete next;
	for each (auto n in nodes)
	{
		delete n;
	}

    return 0;
}
