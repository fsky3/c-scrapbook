#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#define RESULT_OK   0
#define RESULT_NOK -1
#define is_empty(list) (list == NULL)

struct node
{
	struct node* next;
	char data;
};

typedef struct node* list_t;

void init(list_t* list_p);
int push_front(list_t* list_p, const char new_data);
int push_back(list_t* list_p, const char new_data);
int pop_front(list_t* list_p, char* data_storage);
int pop_back(list_t* list_p, char* data_storage);
void clear(list_t* list_p);
unsigned int count(list_t* list_p);

#endif /* LINKED_LIST_H_ */
