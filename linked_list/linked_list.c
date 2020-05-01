#include <stdlib.h>

#include "linked_list.h"

static struct node** get_tail_pp(list_t* list_p);
static struct node* new_node(const char new_data);

void init(list_t* list_p)
{
	 *list_p = NULL;
}

int push_front(list_t* list_p, const char new_data)
{
	struct node* new_p = new_node(new_data);
	if(!new_p) return RESULT_NOK;

	if(*list_p)
	{
		new_p->next = *list_p;
	}

	*list_p = new_p;

	return RESULT_OK;
}

int push_back(list_t* list_p, const char new_data)
{
	struct node* new_p = new_node(new_data);
	if(!new_p) return RESULT_NOK;

	struct node** tail_pp = get_tail_pp(list_p);

	if(tail_pp)
	{
		(*tail_pp)->next = new_p;
	}
	else
	{
		*list_p = new_p;
	}

	return RESULT_OK;
}

int pop_front(list_t* list_p, char* data_storage)
{
	if(!(*list_p)) return RESULT_NOK;

	*data_storage = (*list_p)->data;
	struct node* to_free = *list_p;
	*list_p = (*list_p)->next;
	free(to_free);

	return RESULT_OK;
}

int pop_back(list_t* list_p, char* data_storage)
{
	struct node** tail_pp = get_tail_pp(list_p);
	if(tail_pp)
	{
		*data_storage = (*tail_pp)->data;
		free(*tail_pp);
		*tail_pp = NULL;
		return RESULT_OK;
	}
	else
	{
		return RESULT_NOK;
	}
}

unsigned int count(list_t* list_p)
{
	if(!(*list_p)) return 0;

	struct node* next_p = *list_p;
	unsigned int elements_c = 0;
	while(next_p)
	{
		++elements_c;
		next_p = next_p->next;
	}
	return elements_c;
}

void clear(list_t* list_p)
{
	while(*list_p)
	{
		struct node* to_free = *list_p;
		*list_p = (*list_p)->next;
		free(to_free);
	}
}

static struct node** get_tail_pp(list_t* list_p)
{
	if(!(*list_p)) return NULL;

	while ((*list_p)->next)
	{
		list_p = &(*list_p)->next;
	}
	return list_p;
}

static struct node* new_node(const char new_data)
{
	struct node* new_p;
	new_p = malloc(sizeof(struct node));
	if(!new_p) return NULL;
	else
	{
		new_p->data = new_data;
		new_p->next = NULL;
		return new_p;
	}
}
