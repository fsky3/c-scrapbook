#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#define RESULT_OK   0
#define RESULT_NOK -1
#define is_empty(list) (list == nullptr)

struct node
{
	struct node* next;
	char data;
};

typedef struct node* list_t;

list_t make_new(char data);
int push_front(list_t& list, char new_data);
int push_back(list_t& list, char new_data);
int pop_front(list_t& list, char& data_storage);
int pop_back(list_t& list, char& data_storage);
void clear(list_t& list);
unsigned int count(const list_t& list);

#endif /* LINKED_LIST_H_ */
