#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef NO_TEST
#include <criterion.h>
#endif

static inline size_t parent_i(size_t idx)
{
    return idx > 0 ? (idx - 1) >> 1 : 0;
}

static inline size_t left_i(size_t idx)
{
    return (idx << 1) + 1;
}

static inline size_t right_i(size_t idx)
{
    return (idx << 1) + 2;
}

static void swap(void* a, void* b, size_t size);

#ifndef NO_TEST
Test(heap_units, indexing_functions)
{
    cr_assert(parent_i(0) == 0);
    cr_assert(parent_i(1) == 0);
    cr_assert(parent_i(2) == 0);
    cr_assert(parent_i(3) == 1);
    cr_assert(parent_i(4) == 1);
    cr_assert(parent_i(5) == 2);

    cr_assert(left_i(0) == 1);
    cr_assert(left_i(1) == 3);
    cr_assert(left_i(2) == 5);

    cr_assert(right_i(0) == 2);
    cr_assert(right_i(1) == 4);
    cr_assert(right_i(2) == 6);
}
#endif

/* Iterative version
 */
bool is_max_heap(const void* heap, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *))
{
    // check while at least left child exists
    for(size_t i = 0; left_i(i) < nmemb; ++i)
    {
        if(compar(heap + i * size, heap + left_i(i) * size) < 0)
        {
            return false;
        }

        // is there a right child as well?
        if(right_i(i) < nmemb)
        {
            if(compar(heap + i * size, heap + right_i(i) * size) < 0)
            {
                return false;
            }
        }
    }
    // checked the whole heap
    return true;
}

/* Recursive version.
 */
static bool is_max_heap_r_i(const void* heap, size_t idx, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *))
{
    // is there an element on the left?
    if(left_i(idx) < nmemb)
    {
        // check max heap condition, check recursively left subtree
        if((compar(heap +  left_i(idx) * size, heap + idx * size) < 0) &&
           is_max_heap_r_i(heap,  left_i(idx), nmemb, size, compar) == false)
        {
            // condition is not fulfilled for left element or subtree
            return false;
        }
        // is there an element on the right?
        if(right_i(idx) < nmemb)
        {
            // check max heap condition, check recirsively right subtree
            if(!((compar(heap + right_i(idx) * size, heap + idx * size) < 0) &&
                     is_max_heap_r_i(heap, right_i(idx), nmemb, size, compar)))
            {
                // condition is not fulfilled for right element or subtree
                return false;
            }
        }
    }
    // checked the whole heap
    return true;
}

/* Recursive version wrapper.
 */
bool is_max_heap_r(const void* heap, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *))
{
    return is_max_heap_r_i(heap, 0, nmemb, size, compar);
}

/* Max heapify shifts element under "idx" into the position required
 * to preserve max heap property, assuming that left and right elements
 * of "idx" are max-type sub-heaps.
 */
void max_heapify_r(void* heap, size_t idx, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *))
{
    if(left_i(idx) < nmemb)
    {
        size_t largest_i = idx;
        // Check if left child is larger than parent, if so, save index
        if(compar(heap + left_i(idx) * size, heap + idx * size) > 0)
        {
            largest_i = left_i(idx);
        }

        // Compare right child with the larger of two compared above
        if(right_i(idx) < nmemb &&
           compar(heap + right_i(idx) * size, heap + largest_i * size) > 0)
        {
            largest_i = right_i(idx);
        }

        // Is max heap condition not satisfied?
        if(largest_i != idx)
        {
            swap(heap + idx * size, heap + largest_i * size, size);
            // Run recursively on modified sub-heap
            max_heapify_r(heap, largest_i, nmemb, size, compar);
        }
    }
}

/* Same, but iterative.
 */
void max_heapify(void* heap, size_t idx, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *))
{
    size_t i = idx;

    // check while at least left child exists
    while(left_i(i) < nmemb)
    {
        size_t largest_i = i;
        if(compar(heap + i * size, heap + left_i(i) * size) < 0)
        {
            largest_i = left_i(i);
        }

        // is the right child the largest?
        if(right_i(i) < nmemb &&
           compar(heap + largest_i * size, heap + right_i(i) * size) < 0)
        {
            largest_i = right_i(i);
        }

        // Is max heap condition not satisfied?
        if(largest_i != i)
        {
            swap(heap + i * size, heap + largest_i * size, size);
            i = largest_i;
        }
        else
        {
            return;
        }
    }
}

void build_max_heap(void* heap, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *))
{
    if(nmemb == 0) return;

    // start from the parent of the last element
    for(int i = parent_i(nmemb - 1); i >= 0; --i)
    {
        max_heapify(heap, i, nmemb, size, compar);
    }
}

void heap_sort(void* heap, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *))
{
    if(nmemb == 0) return;

    build_max_heap(heap, nmemb, size, compar);

    for(int i = nmemb - 1; i > 0; --i)
    {
        swap(heap, heap + i * size, size);
        max_heapify(heap, 0, i, size, compar);
    }
}

static void swap(void* a, void* b, size_t size)
{
    void* temp = malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}
