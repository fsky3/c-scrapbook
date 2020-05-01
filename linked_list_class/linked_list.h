#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#define RESULT_OK   0
#define RESULT_NOK -1

struct node
{
	struct node* next;
	char data;
};

typedef struct node* list_t;

class List {
private:
	list_t list;
public:
	List(char data);
	int push_front(char new_data);
	int push_back(char new_data);
	int pop_front(char& data_storage);
	int pop_back(char& data_storage);
	void clear();
	unsigned int count();
};

#endif /* LINKED_LIST_H_ */
