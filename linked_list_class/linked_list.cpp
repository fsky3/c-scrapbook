#include "linked_list.h"

static struct node** get_tail_pp(list_t& list);
static struct node* get_tail_p(list_t& list);
static struct node* new_node(const char new_data);

List::List(char data) :list{new_node(data)} {}

int List::push_front(char new_data)
{
	auto new_p = new_node(new_data);
	if(!new_p) return RESULT_NOK;

	if(list)
	{
		new_p->next = list;
	}
	list = new_p;
	return RESULT_OK;
}

int List::push_back(char new_data)
{
	auto new_p = new_node(new_data);
	if(!new_p) return RESULT_NOK;

	auto tail_p = get_tail_p(list);
	if(tail_p)
	{
		tail_p->next = new_p;
	}
	else
	{
		list = new_p;
	}
	return RESULT_OK;
}

int List::pop_front(char& data_storage)
{
	if(!list) return RESULT_NOK;

	data_storage = list->data;
	struct node* to_free_p = list;
	list = list->next;
	delete to_free_p;
	return RESULT_OK;
}

int List::pop_back(char& data_storage)
{
	auto tail_pp = get_tail_pp(list);
	if(tail_pp)
	{
		data_storage = (*tail_pp)->data;
		delete *tail_pp;
		*tail_pp = nullptr;
		return RESULT_OK;
	}
	else
	{
		return RESULT_NOK;
	}
}

unsigned int List::count()
{
	if(!list) return 0;

	auto next = list;
	auto elements_c = 0;
	while(next)
	{
		++elements_c;
		next = next->next;
	}
	return elements_c;
}

void List::clear()
{
	while(list)
	{
		auto to_free = list;
		list = list->next;
		delete to_free;
	}
}

static struct node** get_tail_pp(list_t& list)
{
	if(!list) return nullptr;

	auto next_pp = &list;
	while ((*next_pp)->next)
	{
		next_pp = &((*next_pp)->next);
	}
	return next_pp;
}

static struct node* get_tail_p(list_t& list)
{
	auto tail_pp = get_tail_pp(list);
	if(tail_pp)
	{
		return *tail_pp;
	}
	else
	{
		return nullptr;
	}
}
/*
static struct node* get_tail_p(list_t& current)
{
	if(current && current->next) return get_tail_p(current->next);
	else return current;
}
*/
static struct node* new_node(const char new_data)
{
	auto new_p = new struct node;
	if(!new_p) return nullptr;
	else
	{
		new_p->data = new_data;
		new_p->next = nullptr;
		return new_p;
	}
}
