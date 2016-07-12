#include<iostream>
#include<vector>
#include<assert.h>

using namespace std;

template<class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};
template<class T>
struct Greater
{
	bool operator()(const T& l, const T& r)
	{
		return l>r;
	}
};

template<class T,class compare=Greater<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(const T *arr, size_t size)
	{
		assert(arr);
		for (size_t i = 0; i < size; ++i)
			_arr.push_back(arr[i]);
		int begin = (_arr.size()-2)/2;
		for (; begin >= 0; --begin)
			_AdjustDown(begin);
	}
	void Push(const T& x)
	{
		_arr.push_back(x);
		int begin = _arr.size() - 1;
		_AdjustUp(begin);
	}
	void Pop()
	{
		if (_arr.size() <= 0)
			return;
		swap(_arr[0], _arr[_arr.size() - 1]);
		_arr.pop_back();
		_AdjustDown(0);
	}
	T& Top()
	{
		if (!IsEmpty())
			return _arr[0];
		exit(1);
	}
	bool IsEmpty()
	{
		return _arr.empty();
	}
	size_t Size()
	{
		return _arr.size();
	}
	void Print()
	{
		for (size_t i = 0; i < _arr.size(); ++i)
			cout << _arr[i] << " ";
		cout << endl;
	}
protected:
	void _AdjustDown(int parent)
	{
		compare com;
		int child = 2 * parent+1;
		while (child < _arr.size())
		{
			if (child + 1 < _arr.size() && com(_arr[child+1], _arr[child]))
				child++;
			if (com(_arr[child], _arr[parent]))
			{
				swap(_arr[parent], _arr[child]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
				break;
		}
	}
	void _AdjustUp(int child)
	{
		compare com;
		int parent = (child - 1) / 2;
		while (parent >= 0)
		{
			if (com(_arr[child], _arr[parent]))
			{
				swap(_arr[parent], _arr[child]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
				break;
		}
	}
private:
	vector<T> _arr;
};

void TestHeap()
{
	int a[] = { 1, 2, 4, 3,5,0,7 };
	Heap<int> h;
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
		h.Push(a[i]);
	h.Print();
	h.Pop();
	h.Print();
	h.Pop();
	h.Print();
}