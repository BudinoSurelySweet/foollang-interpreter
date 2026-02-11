#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP


template <typename T>
struct list_node
{
	T value;
	list_node<T>* next;
	list_node<T>* prev;
	

	list_node(T new_value);
	bool is_next_null();
	bool is_prev_null();
};


template <typename T>
list_node<T>::list_node(T new_value)
{
	value = new_value;
	next = nullptr;
	prev = nullptr;
};


template <typename T>
bool list_node<T>::is_next_null()
{
	return next == nullptr ? true : false;
}


template <typename T>
bool list_node<T>::is_prev_null()
{
	return prev == nullptr ? true : false;
}


template <typename T>
struct linked_list
{
	list_node<T>* head;
	list_node<T>* tail;
	

	linked_list();
	void push(T new_value);
	void append(T new_value);
	void clear();
	~linked_list();
};


template <typename T>
linked_list<T>::linked_list()
{
	head = nullptr;
	tail = nullptr;
}


template <typename T>
void linked_list<T>::push(T new_value)
{
	if (head and tail)
	{
		new_value->next = head;
		head->prev = new_value;
		head = new_value;
	}

	if (not head)
		head = new_value;
	
	if (not tail)
		tail = new_value;
}


template <typename T>
void linked_list<T>::append(T new_value)
{
	list_node<T>* new_node = new list_node<T>(new_value);

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
}


template <typename T>
void linked_list<T>::clear()
{
	for (list_node<T>* node = head; node != nullptr; node = node->next)
	{
		if (not node->prev)
			delete node->prev;
	}
	
	delete tail;

	head = nullptr;
	tail = nullptr;
}


template <typename T>
linked_list<T>::~linked_list()
{
	clear();
}

#endif
