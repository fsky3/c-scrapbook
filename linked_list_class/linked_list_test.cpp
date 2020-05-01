#include <time.h>
#include <criterion.h>
#include "linked_list.h"

Test(singly_linked_list_suite, basic_scenario)
{
	const char initial_char = 'x';
	List list {initial_char};
	cr_assert_eq(1, list.count());

	char char_to_push = 'a';
	char char_popped = '\0';
	cr_assert_eq(RESULT_OK, list.push_back(char_to_push));
	cr_assert_eq(RESULT_OK, list.pop_back(char_popped));
	cr_assert_eq(char_to_push, char_popped);

	char_to_push = 'b';
	char_popped = '\0';
	cr_assert_eq(RESULT_OK, list.push_front(char_to_push));
	cr_assert_eq(RESULT_OK, list.pop_front(char_popped));
	cr_assert_eq(char_to_push, char_popped);

	cr_assert_eq(RESULT_OK, list.pop_front(char_popped));
	cr_assert_eq(initial_char, char_popped);
	cr_expect_eq(0, list.count());
}

Test(singly_linked_list_suite, pop_more_than_pushed)
{
	const char to_push = 'f';
	List list {to_push};
	int op_result;
	char to_pop_into = '\0';
	op_result = list.pop_back(to_pop_into);
	cr_assert_eq(op_result, RESULT_OK);
	cr_assert_eq(to_push, to_pop_into);

	for(int i = 0; i < 3; ++i)
	{
		// trying to pop more than pushed should return error
		op_result = list.pop_back(to_pop_into);
		cr_expect_eq(op_result, RESULT_NOK);
	}

	// buffer should not be altered after wrong pops
	cr_assert_eq(to_push, to_pop_into);
}

Test(singly_linked_list_suite, push_pop_push_track_length)
{
	List letters {'a'};
	unsigned int list_length = 1;

	for (char c = 'b'; c <= 'z'; ++c)
	{
		cr_assert_eq(RESULT_OK, letters.push_back(c));
		++list_length;
	}

	cr_assert_eq(
		letters.count(),
		list_length,
		"Expected list length of %d, got %d!",
		list_length,
		letters.count()
	);

	for (int i = 0; i < 3; ++i)
	{
		char storage = 0;
		cr_assert_eq(RESULT_OK, letters.pop_back(storage));
		cr_assert_eq(storage, 'z' - i);
		--list_length;
	}

	for (int i = 0; i < 3; ++i)
	{
		cr_assert_eq(RESULT_OK, letters.push_back('x' + i));
		++list_length;
	}

	while(list_length)
	{
		char storage = 0;

		if(list_length & 0x1)
		{
			cr_assert_eq(RESULT_OK, letters.pop_back(storage));;
		}
		else
		{
			cr_assert_eq(RESULT_OK, letters.pop_front(storage));;
		}

		--list_length;
	}

	{
		char storage = 0;
		cr_assert_eq(0, list_length, "0 != %d", list_length);
		cr_assert_eq(RESULT_NOK, letters.pop_back(storage));
		cr_assert_eq(RESULT_NOK, letters.pop_front(storage));
	}
}

Test(singly_linked_list_suite, push_clear_test)
{
	List list {'\0'};
	const unsigned int elements_c = 100;

	for (unsigned int i = 1; i < elements_c ; ++i)
	{
		if(i & 0x1)
		{
			cr_assert_eq(RESULT_OK, list.push_back('\0'));
		}
		else
		{
			cr_assert_eq(RESULT_OK, list.push_front('\0'));
		}
	}

	unsigned int counted = list.count();
	cr_assert_eq(
		counted,
		elements_c,
		"Counted %d != %d",
		counted,
		elements_c
	);

	list.clear();
	cr_assert_eq(0, list.count());
}

Test(singly_linked_list_suite, time_measurements, .disabled = false)
{
	const unsigned int count = 10000;

	cr_log_info("Measure cycles for the push_back "
			    "and pop_back of %d elements:", count);
	clock_t start = clock();
	List list {'\0'};
	for(unsigned int i = 1; i < count; ++i)
	{
		cr_assert_eq(RESULT_OK, list.push_back('\0'));
	}
	for(unsigned int i = 0; i < count; ++i)
	{
		char dummy = 0;
		cr_assert_eq(RESULT_OK, list.pop_back(dummy));
	}
	clock_t end = clock();
	unsigned int ticks = end - start;
	cr_log_info("CPU ticks used: %9d", ticks);

	cr_assert_eq(0, list.count());

	cr_log_info("Measure cycles for the push_front "
			    "and pop_front of %d elements:", count);
	start = clock();
	for(unsigned int i = 0; i < count; ++i)
	{
		cr_assert_eq(RESULT_OK, list.push_front('\0'));
	}
	for(unsigned int i = 0; i < count; ++i)
	{
		char dummy = 0;
		cr_assert_eq(RESULT_OK, list.pop_front(dummy));
	}
	cr_assert_eq(0, list.count());
	end = clock();
	ticks = end - start;
	cr_log_info("CPU ticks used: %9d", ticks);
}
