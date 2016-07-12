#pragma once
#include<iostream>
#include<vector>
#include<assert.h>
#include"Heap.h"

using namespace std;

template<class T>
struct HuffmanNode
{
	T _weight;
	HuffmanNode<T>* _parent;
	HuffmanNode<T>* _left;
	HuffmanNode<T>* _right;

	HuffmanNode(const T& weight)
		:_weight(weight)
		, _parent(NULL)
		, _left(NULL)
		, _right(NULL)
	{}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}
	HuffmanTree(T *arr, size_t size, const T& invalid)
	{
		assert(arr);
		_CreateHuffman(arr, size, invalid);
	}
	~HuffmanTree()
	{
		_DestoryTree(_root);
	}
	template<class T>
	struct CompareLess
	{
		bool operator()(Node* l, Node* r)
		{
			return l->_weight < r->_weight;
		}
	};

	Node* GetRootNode()
	{
		return _root;
	}
	void PrintTree()
	{
		_Print(_root);
		cout << endl;
	}
protected:
	void _CreateHuffman(const T *arr, size_t size, const T& invalid)
	{
		Node* parent;
		Node* left;
		Node* right;
		Heap<Node*, CompareLess<T>> h;

		for (size_t i = 0; i < size; ++i)
		{
			if (arr[i] != invalid)
			{
				Node* tmp = new Node(arr[i]);
				h.Push(tmp);
			}
		}
		while (h.Size()>1)
		{
			left = h.Top(); 
			h.Pop();
			right = h.Top();
			h.Pop();

			parent = new Node(left->_weight + right->_weight);

			parent->_left = left;
			left->_parent = parent;
			parent->_right = right;
			right->_parent = parent;
			h.Push(parent);
		}
		_root = h.Top();
	}
	void _DestoryTree(Node*& root)
	{
		if (root)
		{
			_DestoryTree(root->_left);
			_DestoryTree(root->_right);
			delete root;
		}
	}
	void _Print(Node* root)
	{
		if (root)
		{
			cout << root->_weight << " ";
			_Print(root->_left);
			_Print(root->_right);
		}
	}
private:
	Node* _root;
};

void TestHuffman()
{
	int arr[] = { 1,2,4,5,3};
	HuffmanTree<int> hf(arr, 5, '#');
	hf.PrintTree();
}