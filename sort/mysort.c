#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "heap/heap.h"

#define NEWLINE_LEN         1U
#define NULL_TERM_LEN       1U
#define MAX_LINE_LEN       (60U + NEWLINE_LEN + NULL_TERM_LEN)
#define PREALLOC_LINES      1000000U

#define EXPECTED_ARGS_STDIN 2U
#define EXPECTED_ARGS_FILE  3U
#define ALGORITHM_FLAG_IDX  1U
#define FILE_PATH_IDX       2U

enum inputs {input_stdin, input_file};

int mystrcmp(const void* p1, const void* p2);

void bubble_sort(void* base, size_t nmemb, size_t size,
    int (*compar)(const void*, const void*));
void insertion_sort(void* base, size_t nmemb, size_t size,
    int (*compar)(const void*, const void*));
void selection_sort(void* base, size_t nmemb, size_t size,
    int (*compar)(const void*, const void*));
void merge_sort(void* base, size_t nmemb, size_t size,
    int (*compar)(const void*, const void*));

static void print_help(void);

/* For comparing complexity */
static size_t swaps;
static size_t compars;

int main(int argc, char* argv[])
{
    enum inputs sel_input;

    switch(argc)
    {
        case EXPECTED_ARGS_STDIN:
            sel_input = input_stdin;
            break;
        case EXPECTED_ARGS_FILE:
            sel_input = input_file;
            break;
        default:
            print_help();
            return -1;
    }

    FILE* file_p = NULL;
    if(sel_input == input_file)
    {
        file_p = fopen(argv[FILE_PATH_IDX], "r"); // malloc inside
        if(!file_p)
        {
            perror("Could not open file");
            return -1;
        }
    }

    /* Place pointer to each line in lines_p array */
    char** lines_p = malloc(PREALLOC_LINES * sizeof(*lines_p));
    size_t max_lines = PREALLOC_LINES;
    FILE* selected_stream = (sel_input == input_file) ? file_p : stdin;
    size_t read_lines = 0;
    while(1)
    {
        char* line_p = malloc(MAX_LINE_LEN * sizeof(*line_p));
        char* retval = fgets(line_p, MAX_LINE_LEN, selected_stream); // malloc inside
        if(retval)
        {
            lines_p[read_lines] = line_p;
            ++read_lines;
        }
        /* We got EOF */
        else
        {
            free(line_p);
            break;
        }

        if(read_lines == max_lines)
        {
            lines_p = realloc(lines_p, (max_lines + PREALLOC_LINES) * sizeof(*lines_p));
            max_lines += PREALLOC_LINES;
        }
    }

    /* Sort based on selected algorithm */
    switch(*argv[ALGORITHM_FLAG_IDX])
    {
        case 'b':
            /* We are moving pointers to lines, not lines themselves */
            bubble_sort(lines_p, read_lines, sizeof(*lines_p), mystrcmp);
            break;

        case 'q':
            qsort(lines_p, read_lines, sizeof(*lines_p), mystrcmp);
            break;

        case 'i':
            insertion_sort(lines_p, read_lines, sizeof(*lines_p), mystrcmp);
            break;

        case 's':
            selection_sort(lines_p, read_lines, sizeof(*lines_p), mystrcmp);
            break;

        case 'm':
            merge_sort(lines_p, read_lines, sizeof(*lines_p), mystrcmp);
            break;

        case 'h':
            heap_sort(lines_p, read_lines, sizeof(*lines_p), mystrcmp);
            break;

        default:
            printf("Incorrect algorithm selection flag!\n");
            return -1;
    }

    /* Check for quiet mode */
    bool is_quiet = false;
    if(*(argv[ALGORITHM_FLAG_IDX] + sizeof(char)) == 'q')
    {
        is_quiet = true;
    }

    /* Print results */
    for(int i = 0; i < read_lines; i++)
    {
        if(!is_quiet)
        {
            printf("%s", lines_p[i]); // malloc inside
        }
        free(lines_p[i]);
    }

    /* For comparing complexity */
    if(is_quiet)
    {
        printf("\n");
        printf("Compars: %lu\n", compars);
        printf("Swaps:   %lu\n", swaps);
        printf("-------- \n");
        printf("Sum:     %lu\n", swaps + compars);
        printf("\n");
    }

    /* Cleanup */
    free(lines_p);
    if(sel_input == input_file)
    {
        fclose(file_p);
    }
    return 0;
}

/* @return -1 - p1 is less than p2
 *          0 - p1 is equal to p2
 *          1 - p1 is greater than p2
 */
int mystrcmp(const void* p1, const void* p2)
{
    /* Convert and dereference into string pointers */
    const char* a = *(const char**)p1;
    const char* b = *(const char**)p2;

    /* For comparing complexity */
    ++compars;

    /* Go along string comparing each character */
    for(size_t idx = 0; idx < MAX_LINE_LEN; ++idx)
    {
        if(a[idx] < b[idx]) return -1;
        if(a[idx] > b[idx]) return 1;

        /* Don't read strings beyond terminator */
        if(a[idx] == '\0') break;
    }

    /* Strings are equal if we're here */
    return 0;
}

static void swap(void* a, void* b, size_t size)
{
    /* For comparing complexity */
    ++swaps;

    void* tmp = malloc(size);
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
    free(tmp);
}

void bubble_sort(void* base, size_t nmemb, size_t size,
    int (*compar)(const void*, const void*))
{
    bool rerun_flag = true;
    size_t num_in_position = 0;
    size_t rerun_idx = 0;

    while(rerun_flag)
    {
        rerun_flag = false;
        for(size_t c = rerun_idx; c + 1 < nmemb - num_in_position; ++c)
        {
            if(compar(base + c * size, base + (c + 1) * size) > 0)
            {
                swap(base + c * size, base + (c + 1) * size, size);
                // no point to rerun if we swapped only 1st elem
                if(c > 0)
                {
                    // skip non-modified elements in front next time
                    if(!rerun_flag) rerun_idx = c - 1;
                    rerun_flag = true;
                }
            }
        }
        // each loop shifts next largest element to the final position
        ++num_in_position;
    }
}

void insertion_sort(void* base, size_t nmemb, size_t size,
    int (*compar)(const void*, const void*))
{
    /* Take first element from the unsorted part (i to nmemb - 1) */
    for(size_t i = 1; i < nmemb; ++i)
    {
        /* Compare it with elements of the sorted part (i - 1 to 0) */
        for(size_t j = i; j > 0; --j)
        {
            if(compar(base + (j - 1) * size, base + j * size) > 0)
            {
                /* Shift it before larger element of the sorted array */
                swap(base + (j - 1) * size, base + j * size, size);
            }
            else
            {
                /* The element is in the right place now */
                break;
            }
        }
    }
}

static size_t find_smallest_lin(void* base, size_t nmemb, size_t size,
    int (*compar)(const void*, const void*))
{
    size_t smallest_idx = 0;
    for(size_t idx = 1; idx < nmemb; ++idx)
    {
        /* New smallest? */
        if(compar(base + idx * size, base + smallest_idx * size) < 0)
        {
            smallest_idx = idx;
        }
    }

    return smallest_idx;
}

void selection_sort(void* base, size_t nmemb, size_t size,
    int (*compar)(const void*, const void*))
{
    /* idx + 1 as there must be at least one more element to compare */
    for(size_t idx = 0; idx + 1 < nmemb; ++idx)
    {
        /* Find smallest element in the sub-array  */
        size_t smallest_idx =
            idx + find_smallest_lin(base + idx * size, nmemb - idx, size, compar);
        if(idx == smallest_idx) continue;
        swap(base + idx * size, base + smallest_idx * size, size);
    }
}

static void merge(void* base, size_t numa, size_t numb, size_t size,
    int (*compar)(const void*, const void*))
{
    void* merged_p = malloc((numa + numb) * size);
    void* a = base;
    void* b = base + numa * size;
    size_t idxa = 0;
    size_t idxb = 0;

    for(size_t i = 0; i < numa + numb; ++i)
    {
        if(numa - idxa == 0)
        {
            // no more items in "a"
            memcpy(merged_p + i * size, b + idxb * size, size);
            ++idxb;
            continue;
        }

        if(numb - idxb == 0)
        {
            // no more items in "b"
            memcpy(merged_p + i * size, a + idxa * size, size);
            ++idxa;
            continue;
        }

        if(compar(a + idxa * size, b + idxb * size) < 0)
        {
            memcpy(merged_p + i * size, a + idxa * size, size);
            ++idxa;
        }
        else
        {
            memcpy(merged_p + i * size, b + idxb * size, size);
            ++idxb;
        }
    }

    memcpy(base, merged_p, (numa + numb) * size);
    free(merged_p);
}

void merge_sort(void* base, size_t nmemb, size_t size,
    int (*compar)(const void*, const void*))
{
    if(nmemb > 2)
    {
        merge_sort(base, nmemb - nmemb / 2, size, compar);
        merge_sort(base + (nmemb - nmemb / 2) * size, nmemb / 2, size, compar);
    }

    merge(base,
          nmemb - nmemb / 2,
          nmemb / 2,
          size,
          compar);
}

static void print_help(void)
{
    printf("Syntax:\n\
    mysort ALGORITHM [FILE]\n\n\
    algorithms:\n\
    b - bubble\n\
    q - quick (stdlib)\n\
    i - insertion\n\
    s - selection\n\
    m - merge\n\
    \n\
    Add 'q' after algorithm for quiet mode, e.g.:\n\
    ./mysort bq lines_to_sort.txt\n");
}

