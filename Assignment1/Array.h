#ifndef ARRAY_H
#define ARRAY_H
#include<cstdlib>
#include<iostream>
#include<initializer_list>
using namespace std;
template<typename T>
class Array
{
	T* data;
	int capacity;

	bool isValidIndex(int index)const;
public:
	~Array();
	T& operator[](int i);
	const T& operator[](int i)const;
	int getCapacity()const;
	void reSize(int newCapacity);
	Array<T>& operator=(const Array<T>&);
	Array(const Array<T>&);
	Array<T>& operator=(Array<T>&&);
	Array(Array<T>&&);
	Array(initializer_list<T> list);
	

	template<typename T>
	class Iterator
	{
		T* ptr;

	public:
		Iterator(T* _ptr):ptr(_ptr)
		{ }
		T* operator++()
		{
			ptr++;
			return ptr;
		}
		T* operator++(int)
		{
			T* temp = ptr;
			ptr++;
			return temp;
		}
		T& operator*()
		{
			return *ptr;
		}
		bool operator!=(Iterator<T> obj)
		{
			return ptr != obj.ptr;
		}
	};

	Iterator<T> begin()
	{
		return data;
	}
	Iterator<T> end()
	{
		return data + capacity;
	}
};
#endif
// !ARRAY_H
