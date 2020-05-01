#ifndef NO_TEST
#include <criterion.h>
#include "heap.h"

#define nmemb(arr) (sizeof(arr)/sizeof(arr[0]))

static int compar_uint(const void* a, const void* b)
{
    int result = *(unsigned int*)a - *(unsigned int*)b;
    return result;
}

Test(heap_functionals, is_max_heap)
{
    const unsigned int heap[] = {15, 11, 10, 8, 7, 9};
/*
           (15)
          /    \
       (11)    (10)
       /  \    /
     (8)  (7) (9)
*/
    cr_assert(is_max_heap(heap, nmemb(heap), sizeof(unsigned), compar_uint)
              == true);

    const unsigned int* bad_max_heap = &heap[3];
/*
        (8)
       /   \
     (7)   (9)
*/
    cr_assert(is_max_heap(bad_max_heap, 3, sizeof(unsigned int), compar_uint)
              == false);

// Single element "heap" is always a max heap

    cr_assert(is_max_heap(heap, 1, sizeof(unsigned), compar_uint) == true);
}

Test(heap_functionals, max_heapify)
{
    // not a max heap, 18 and 16 are in the wrong place
    int array[] = {11, 15, 18, 16, 7, 5};
/*
             (11)
            /    \
        (15)     (18)
        /  \     /
     (16)  (7)  (5)
 */
    cr_assert(is_max_heap(array, nmemb(array), sizeof(unsigned), compar_uint)
              == false);

    max_heapify(array, 0, nmemb(array), sizeof(int), compar_uint);

    cr_assert(is_max_heap(array, nmemb(array), sizeof(unsigned), compar_uint)
              == false);

    // this is the expected array after first max_heapify
    const int still_not_max_heap[] = {18, 15, 11, 16, 7, 5};
/*
             (18)
            /    \
        (15)     (11)
        /  \     /
     (16)  (7)  (5)
 */
    cr_assert(memcmp(array, still_not_max_heap, sizeof(array)) == 0);

    max_heapify(array, 1, nmemb(array), sizeof(int), compar_uint);

    cr_assert(is_max_heap(array, nmemb(array), sizeof(unsigned), compar_uint)
              == true);

    // this is the expected heap after second max_heapify
    const int finally_max_heap[] = {18, 16, 11, 15, 7, 5};
/*
             (18)
            /    \
        (16)     (11)
        /  \     /
     (15)  (7)  (5)
 */
    cr_assert(memcmp(array, finally_max_heap, sizeof(array)) == 0);
}

Test(heap_functionals, build_max_heap)
{
    // not a max heap
    int array[] = {11, 9, 18, 15, 7, 5, 3, 22, 4};
/*
              (11)
             /    \
           (9)    (18)
          /  \    /  \
       (15) (7) (5) (3)
       /  \
    (22)  (4)
 */
    cr_assert(is_max_heap(array, nmemb(array), sizeof(unsigned), compar_uint)
              == false);

    build_max_heap(array, nmemb(array), sizeof(int), compar_uint);

    cr_assert(is_max_heap(array, nmemb(array), sizeof(unsigned), compar_uint)
              == true);
}

Test(heap_functionals, heap_sort)
{
    int array[] = {11, 9, 18, 15, 7, 5, 3, 22, 4};
    heap_sort(array, nmemb(array), sizeof(int), compar_uint);

    int sorted[] = {3, 4, 5, 7, 9, 11, 15, 18, 22};
    cr_assert(memcmp(array, sorted, sizeof(array)) == 0);
}
#endif
