#ifndef LIST_H
#define LIST_H

#include "colors.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

typedef int list_elem_t;
const int POISON = 0x12345678;

typedef enum error_codes
{
    SUCCESSFUL_RETURN = 0,
    MEMORY_ALLOCATION_ERROR = 10,
    NULL_LIST_MEANING = 11,
    NULL_DATA_MEANING = 12,
    NULL_NEXT_MEANING = 13,
    ZERO_CAPACITY_MEANING = 14,
    SIZE_MORE_THAN_CAPACITY = 15,
    NO_FREE_SPACE = 16,
    NO_POS_FOUND = 17,
    UNAVAILABLE_POS = 18,
    HEAD_FOUND = 19,
    FREE_PLACE = 20
} error_codes;

typedef struct list_t
{
    list_elem_t *data;
    list_elem_t *next;
    list_elem_t *prev;
    size_t free;
    size_t head;
    size_t tail;
    size_t size;
    size_t capacity;
} list_t;

error_codes list_ctor(list_t *list , size_t start_size);
error_codes fill_array(list_elem_t *arr, list_elem_t elem, size_t start, size_t end);
error_codes prepare_array(list_elem_t **arr, list_elem_t zero_elem, list_elem_t fill_elem, size_t size);
error_codes find_free(list_t *list);
error_codes list_push(list_t *list, list_elem_t elem, size_t pos);
error_codes find_pos(list_t *list, size_t *old_pos, size_t pos);
error_codes list_verify(list_t *list);
error_codes list_dtor(list_t *list);
void list_dump(list_t *list, const char *reason, const char *process);
void print_array(list_t *list, list_elem_t *arr, const char *arrname);
void destroy_array(list_elem_t **arr, size_t capacity);
error_codes realloc_arrays(list_t *list);
error_codes realloc_array(list_elem_t **arr, size_t capacity, list_elem_t filler);
error_codes position_analysis(list_t *list, size_t pos);
error_codes common_handle_position(list_t *list, size_t pos);
error_codes handle_init_position(list_t *list);
#endif