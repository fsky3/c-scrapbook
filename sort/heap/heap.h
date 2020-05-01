#include <stdbool.h>

bool is_max_heap(const void* heap, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *));

void max_heapify(void* heap, size_t idx, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *));

void build_max_heap(void* heap, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *));

void heap_sort(void* heap, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *));
