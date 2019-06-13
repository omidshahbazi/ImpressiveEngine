/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
//
/// The license is
///
/// Permission is denied, to any person or company
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// Project leader : O.Shahbazi <sh_omid_m@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////*/
#include "Common.h"
#include <ostream>
#include <vector>
#include <cassert>

BEGIN_NAMESPACE
template<class T>
void Swap(T &a, T &b)
{
  T temp = a;

  a = b;

  b = temp;
}

template<class T>
void ReorderUpwards(std::vector<T>& heap, int nd)
{
	while ( (nd>1) && (heap[nd/2] < heap[nd]))
	{
		Swap(heap[nd/2], heap[nd]);

		nd /= 2;
	}
}

template<class T>
void ReorderDownwards(std::vector<T>& heap, int nd, int HeapSize)
{
	while (2*nd <= HeapSize)
	{
		int child = 2 * nd;

		if ( (child < HeapSize) && (heap[child] < heap[child+1]) )
		{
			++child;
		}

		if (heap[nd] < heap[child])
		{
			Swap(heap[child], heap[nd]);

			nd = child;
		}
		else
		{
			break;
		}
	}
}



template<class T>
class PriorityQ
{
private:

	std::vector<T>  m_Heap;

	int             m_iSize;

	int             m_iMaxSize;

	void ReorderUpwards(std::vector<T>& heap, int nd)
	{
		while ( (nd>1) && (heap[nd/2] < heap[nd]))
		{
			Swap(heap[nd/2], heap[nd]);

			nd /= 2;
		}
	}

	void ReorderDownwards(std::vector<T>& heap, int nd, int HeapSize)
	{
		while (2*nd <= HeapSize)
		{
			int child = 2 * nd;

			if ( (child < HeapSize) && (heap[child] < heap[child+1]) )
			{
				++child;
			}

			if (heap[nd] < heap[child])
			{
				Swap(heap[child], heap[nd]);

				nd = child;
			}
			else
			{
				break;
			}
		}
	}

public:

	PriorityQ(int MaxSize):m_iMaxSize(MaxSize), m_iSize(0)
	{
		m_Heap.assign(MaxSize+1, T());
	}

	bool empty()const{return (m_iSize==0);}

	void insert(const T item)
	{
		assert (m_iSize+1 <= m_iMaxSize);

		++m_iSize;

		m_Heap[m_iSize] = item;

		ReorderUpwards(m_Heap, m_iSize);
	}

	T pop()
	{
		Swap(m_Heap[1], m_Heap[m_iSize]);

		ReorderDownwards(m_Heap, 1, m_iSize-1);

		return m_Heap[m_iSize--];
	}

	const T& Peek()const{return m_Heap[1];}
};

template<class T>
class PriorityQLow
{
private:

	std::vector<T>  m_Heap;

	int             m_iSize;

	int             m_iMaxSize;

	void ReorderUpwards(std::vector<T>& heap, int nd)
	{
		while ( (nd>1) && (heap[nd/2] > heap[nd]))
		{
			Swap(heap[nd/2], heap[nd]);

			nd /= 2;
		}
	}

void ReorderDownwards(std::vector<T>& heap, int nd, int HeapSize)
{
	while (2*nd <= HeapSize)
	{
		int child = 2 * nd;

		if ( (child < HeapSize) && (heap[child] > heap[child+1]) )
		{
			++child;
		}

		if (heap[nd] > heap[child])
		{
			Swap(heap[child], heap[nd]);

			nd = child;
		}
  
		else
		{
			break;
		}
	}
}

public:

	PriorityQLow(int MaxSize):m_iMaxSize(MaxSize), m_iSize(0)
	{
		m_Heap.assign(MaxSize+1, T());
	}

	bool empty()const{return (m_iSize==0);}

	void insert(const T item)
	{
		assert (m_iSize+1 <= m_iMaxSize);

		++m_iSize;

		m_Heap[m_iSize] = item;

		ReorderUpwards(m_Heap, m_iSize);
	}

	T pop()
	{
		Swap(m_Heap[1], m_Heap[m_iSize]);

		ReorderDownwards(m_Heap, 1, m_iSize-1);

		return m_Heap[m_iSize--];
	}

	const T& peek()const{return m_Heap[1];}
};

template<class KeyType>
class IndexedPriorityQLow
{
private:

	std::vector<KeyType>&  m_vecKeys;

	std::vector<int>       m_Heap;
 
	std::vector<int>       m_invHeap;

	int                    m_iSize, m_iMaxSize;

	void Swap(int a, int b)
	{
		int temp = m_Heap[a]; m_Heap[a] = m_Heap[b]; m_Heap[b] = temp;

		m_invHeap[m_Heap[a]] = a; m_invHeap[m_Heap[b]] = b;
	}

	void ReorderUpwards(int nd)
	{
		while ( (nd>1) && (m_vecKeys[m_Heap[nd/2]] > m_vecKeys[m_Heap[nd]]) )
		{      
			Swap(nd/2, nd);

			nd /= 2;
		}
	}

void ReorderDownwards(int nd, int HeapSize)
{
	while (2*nd <= HeapSize)
	{
		int child = 2 * nd;

		if ((child < HeapSize) && (m_vecKeys[m_Heap[child]] > m_vecKeys[m_Heap[child+1]]))
		{
			++child;
		}

		if (m_vecKeys[m_Heap[nd]] > m_vecKeys[m_Heap[child]])
		{
			Swap(child, nd);

			nd = child;
		}
		else
		{
			break;
		}
	}
}


public:
  
	IndexedPriorityQLow(std::vector<KeyType>& keys,	int MaxSize) : m_vecKeys(keys),
																   m_iMaxSize(MaxSize),
																   m_iSize(0)
	{
		m_Heap.assign(MaxSize+1, 0);
		m_invHeap.assign(MaxSize+1, 0);
	}

	bool empty() const {return (m_iSize==0);}

void insert(const int idx)
{
	assert (m_iSize+1 <= m_iMaxSize);

	++m_iSize;

	m_Heap[m_iSize] = idx;

	m_invHeap[idx] = m_iSize;

	ReorderUpwards(m_iSize);
}

int Pop()
{
	Swap(1, m_iSize);

	ReorderDownwards(1, m_iSize-1);

	return m_Heap[m_iSize--];
}

	void ChangePriority(const int idx)
	{
		ReorderUpwards(m_invHeap[idx]);
	}
};
END_NAMESPACE