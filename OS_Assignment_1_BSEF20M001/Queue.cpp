#include "Queue.h"
#include<string>
using namespace std;
template <typename T>
Queue<T>::Queue()
{
	head = tail = nullptr;
}

template<typename T>
Queue<T>::Queue(const Queue<T>& ref)
{
	if (!ref.head)
	{
		head = tail = nullptr;
		return;
	}
	Node<T>* refPtr = ref.head;
	Node<T>* objPtr = new Node<T>(refPtr->info);
	head = objPtr;
	while (refPtr->next)
	{
		refPtr = refPtr->next;
		objPtr->next = new Node<T>(refPtr->info);
		objPtr = objPtr->next;
	}
	tail = objPtr;
}
template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& ref)
{
	if (!ref.head)
	{
		head = tail = nullptr;
		return *this;
	}
	Node<T>* refPtr = ref.head;
	Node<T>* objPtr = new Node<T>(refPtr->info);
	head = objPtr;
	tail = ref.tail;
	while (refPtr->next)
	{
		refPtr = refPtr->next;
		objPtr->next = new Node<T>(refPtr->info);
		objPtr = objPtr->next;
	}
	tail = objPtr;
	return *this;
}
template <typename T>
Queue<T>::~Queue()
{
	if (!head)
		return;
	Node<T>* temp = head;
	Node<T>* traverse;
	while (temp)
	{
		traverse = temp->next;
		delete temp;
		temp = traverse;
	}
	head = tail = nullptr;
}
template<typename T>
void Queue<T>:: enQueue(T val)
{
	if (!head)
	{
		head = tail = new Node<T>(val);
		return;
	}
	tail->next = new Node<T>(val);
	tail = tail->next;
}

template<typename T>
T Queue<T>::getElementAtFront()
{
	if (!head)
		throw string("Error : Queue is empty\n");
	return head->info;
}
template<typename T>
T Queue<T>::deQueue()
{
	if (!head)
	{
		throw string("Error : Queue is empty\n");
	}
	if (!head->next)
	{
		T temp = head->info;
		delete head;
		head = tail = nullptr;
		return temp;
	}
	Node<T>* tempPtr = head->next;
	T temp = head->info;
	delete head;
	head = tempPtr;
	return temp;
}
template <typename T>
bool Queue<T>:: isEmpty()
{
	if (head)
		return false;
	return true;
}