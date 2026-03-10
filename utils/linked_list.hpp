#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include "color.hpp"
#include <iostream>

#include <cstddef>


#pragma region Linked List Node

template <typename T>
struct lnode
{
	T value;
	lnode<T>* next;
	lnode<T>* prev;
	

	lnode(T new_value);
	bool is_next_null();
	bool is_prev_null();
};


template <typename T>
lnode<T>::lnode(T new_value)
{
	value = new_value;
	next = nullptr;
	prev = nullptr;
};


template <typename T>
bool lnode<T>::is_next_null()
{
	return next == nullptr ? true : false;
}


template <typename T>
bool lnode<T>::is_prev_null()
{
	return prev == nullptr ? true : false;
}

#pragma endregion


#pragma region Linked List

template <typename T>
struct llist
{
	lnode<T>* head;
	lnode<T>* tail;
	

	llist();
	lnode<T>* push(T new_value);
	lnode<T>* append(T new_value);
	void remove_node(lnode<T>* node);
	void clear();
	~llist();
};


template <typename T>
llist<T>::llist()
{
	head = nullptr;
	tail = nullptr;
}


template <typename T>
lnode<T>* llist<T>::push(T new_value)
{
	lnode<T>* new_node = new lnode<T>(new_value);

	if (head and tail)
	{
		new_node->next = head;
		head->prev = new_node;
		head = new_node;
	}

	if (not head)
		head = new_node;
	
	if (not tail)
		tail = new_node;
	
	return new_node;
}


template <typename T>
lnode<T>* llist<T>::append(T new_value)
{
	lnode<T>* new_node = new lnode<T>(new_value);

	if (head and tail)
	{
		tail->next = new_node;
		new_node->prev = tail;
		tail = new_node;
	}
	
	if (not head)
		head = new_node;
	
	if (not tail)
		tail = new_node;
	
	return new_node;
}


template <typename T>
void llist<T>::remove_node(lnode<T>* node)
{
	if (not node)
		return;

	auto prev = node->prev;
	auto next = node->next;
	
	if (prev)
		prev->next = next;
	else
		head = next;
	
	if (next)
		next->prev = prev;
	else
		tail = prev;

	delete node;
}


template <typename T>
void llist<T>::clear()
{
	for (lnode<T>* node = head; node != nullptr; node = node->next)
	{
		if (not node->prev)
			delete node->prev;
	}
	
	delete tail;

	head = nullptr;
	tail = nullptr;
}


template <typename T>
llist<T>::~llist()
{
	clear();
}

#pragma endregion


#pragma region Linked Matrix

template <typename T>
struct lmatrix : llist<llist<T>*>
{
	// This struct can be empty, it's only purpose is to semplify
	// the writing of a linked list of linked list.

	// It's more readable to write `lmatrix<...>` instead of `llist<llist<...>*>`.
};

#pragma endregion


#endif
