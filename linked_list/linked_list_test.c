#include <criterion.h>
#include <time.h>

#include "linked_list.h"

Test(singly_linked_list_suite, basic_scenario)
{
	list_t list;
	init(&list);
	cr_assert_eq(list, NULL);

	char char_to_push = 'a', char_popped = '\0';
	cr_assert_eq(RESULT_OK, push_back(&list, char_to_push));
	cr_assert_neq(NULL, list);

	cr_assert_eq(RESULT_OK, pop_back(&list, &char_popped));
	cr_expect_eq(NULL, list);
	cr_assert_eq(char_to_push, char_popped);

	char_to_push = 'b';
	char_popped = '\0';
	cr_assert_eq(RESULT_OK, push_front(&list, char_to_push));
	cr_assert_neq(NULL, list);

	cr_assert_eq(RESULT_OK, pop_front(&list, &char_popped));
	cr_expect_eq(NULL, list);
	cr_assert_eq(char_to_push, char_popped);
}

Test(singly_linked_list_suite, pop_more_than_pushed)
{
	char to_push = 'f';
	char to_pop_into = 0;

	list_t list;
	init(&list);

	int func_result;
	func_result = push_back(&list, to_push);
	cr_assert_eq(func_result, RESULT_OK);

	func_result = pop_back(&list, &to_pop_into);
	cr_assert_eq(func_result, RESULT_OK);
	cr_assert_eq(to_push, to_pop_into);

	for(int i = 0; i < 3; ++i)
	{
		// trying to pop more than pushed should return error
		func_result = pop_back(&list, &to_pop_into);
		cr_expect_eq(func_result, RESULT_NOK);
	}

	// buffer should not be altered after wrong pops
	cr_assert_eq(to_push, to_pop_into);
}

Test(singly_linked_list_suite, push_pop_push_track_length)
{
	list_t letters;
	init(&letters);
	int list_length = 0;

	for (char c = 'a'; c <= 'z'; ++c)
	{
		cr_assert_eq(RESULT_OK, push_back(&letters, c));
		++list_length;
	}

	for (int i = 0; i < 3; ++i)
	{
		char storage = 0;
		cr_assert_eq(RESULT_OK, pop_back(&letters, &storage));
		cr_assert_eq(storage, 'z' - i);
		--list_length;
	}

	for (int i = 0; i < 3; ++i)
	{
		cr_assert_eq(RESULT_OK, push_back(&letters, 'x' + i));
		++list_length;
	}

	while(list_length)
	{
		char storage = 0;

		if(list_length & 0x1)
		{
			cr_assert_eq(RESULT_OK, pop_back(&letters, &storage));;
		}
		else
		{
			cr_assert_eq(RESULT_OK, pop_front(&letters, &storage));;
		}

		--list_length;
	}

	{
		char storage = 0;
		cr_assert_eq(0, list_length, "0 != %d", list_length);
		cr_assert_eq(RESULT_NOK, pop_back(&letters, &storage));
		cr_assert_eq(RESULT_NOK, pop_front(&letters, &storage));
	}
}

Test(singly_linked_list_suite, push_clear_test)
{
	list_t list;
	init(&list);
	const unsigned int elements_c = 100;

	for (unsigned int i = 0; i < elements_c ; ++i)
	{
		if(i & 0x1)
		{
			cr_assert_eq(RESULT_OK, push_back(&list, '\0'));
		}
		else
		{
			cr_assert_eq(RESULT_OK, push_front(&list, '\0'));
		}
	}

	unsigned int counted = count(&list);
	cr_assert_eq(counted, elements_c, "Counted %d != %d", counted, elements_c);

	clear(&list);
	cr_assert_eq(NULL, list);
}

Test(singly_linked_list_suite, time_measurements, .disabled = false)
{
	list_t list;
	init(&list);
	const unsigned int count = 10000;

	cr_log_info("Measure cycles for the push_back "
			    "and pop_back of %d elements:", count);
	clock_t start = clock();

	for(int i = 0; i < count; ++i)
	{
		cr_assert_eq(RESULT_OK, push_back(&list, '\0'));
	}
	for(int i = 0; i < count; ++i)
	{
		char dummy = 0;
		cr_assert_eq(RESULT_OK, pop_back(&list, &dummy));
	}
	cr_assert_eq(NULL, list);

	clock_t end = clock();
	unsigned int ticks = end - start;
	cr_log_info("CPU ticks used: %9d", ticks);

	cr_log_info("Measure cycles for the push_front "
			    "and pop_front of %d elements:", count);
	start = clock();

	for(int i = 0; i < count; ++i)
	{
		cr_assert_eq(RESULT_OK, push_front(&list, '\0'));
	}
	for(int i = 0; i < count; ++i)
	{
		char dummy = 0;
		cr_assert_eq(RESULT_OK, pop_front(&list, &dummy));
	}
	cr_assert_eq(NULL, list);

	end = clock();
	ticks = end - start;
	cr_log_info("CPU ticks used: %9d", ticks);
}
