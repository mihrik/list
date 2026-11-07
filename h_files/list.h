#ifndef LIST_H
#define LIST_H

#include "colors.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef int list_elem_t;
const int POISON = 0x12345678;
const int LCANARY = 0x69696969;
const int RCANARY = 0x52525252;

#define LIST_ON_DEBUG
#ifdef LIST_ON_DEBUG
#define ONDBG(...) __VA_ARGS__
#define LIST_DTOR(list) {\
    error_codes err0r = SUCCESSFUL_RETURN;\
    \
    err0r = check_error((*list), list_dtor((list)), "LIST_DTOR");\
    if (err0r != SUCCESSFUL_RETURN && err0r != NULL_LIST_MEANING)\
    {\
        free((list)->data);\
        free((list)->prev);\
        free((list)->next);\
        return err0r;\
    }\
}
#define LIST_POP(list, rtrn_val, pos) {\
    error_codes err0r = SUCCESSFUL_RETURN;\
    \
    err0r = check_error((*list), list_pop((list), (rtrn_val), (pos)), "LIST_POP");\
    if (err0r != SUCCESSFUL_RETURN)\
    {\
        LIST_DTOR(list);\
        return err0r;\
    }\
}
#define LIST_INSERT_AFTER(list, elem, pos) {\
    error_codes err0r = SUCCESSFUL_RETURN;\
    \
    err0r = check_error((*list), list_insert_after((list), (elem), (pos)), "list_insert_after");\
    if (err0r != SUCCESSFUL_RETURN)\
    {\
        LIST_DTOR(list);\
        return err0r;\
    }\
}
#define LIST_INSERT_BEFORE(list, elem, pos) {\
    error_codes err0r = SUCCESSFUL_RETURN;\
    \
    check_error((*list), list_insert_before((list), (elem), (pos)), "list_insert_before");\
    if (err0r != SUCCESSFUL_RETURN)\
    {\
        LIST_DTOR(list);\
        return err0r;\
    }\
}
#define LIST_CTOR(list, start_size) {\
    error_codes err0r = SUCCESSFUL_RETURN;\
    \
    check_error((*list), list_ctor((list), (start_size)), "LIST_CTOR");\
    if (err0r != SUCCESSFUL_RETURN)\
    {\
        LIST_DTOR(list);\
        return err0r;\
    }\
}
#else
#define ONDBG(...)
#define LIST_CTOR(list, start_size) list_ctor((list), (start_size))
#define list_insert_after(list, elem, pos) list_insert_after((list), (elem), (pos))
#define list_insert_before(list, elem, pos) list_insert_before((list), (elem), (pos))
#define LIST_POP(list, rtrn_val, pos) list_pop((list), (rtrn_val), (pos))
#define LIST_DTOR(list) list_dtor((list))
#endif

#define CANARY_DEBUG_ON
#ifdef CANARY_DEBUG_ON
#define CDO(...) __VA_ARGS__
#else
#define CDO(...)
#endif

typedef enum error_codes
{
    SUCCESSFUL_RETURN       = 0,
    MEMORY_ALLOCATION_ERROR = 10,
    NULL_LIST_MEANING       = 11,
    NULL_DATA_MEANING       = 12,
    NULL_NEXT_MEANING       = 13,
    ZERO_CAPACITY_MEANING   = 14,
    SIZE_MORE_THAN_CAPACITY = 15,
    NO_FREE_SPACE           = 16,
    UNAVAILABLE_POS         = 17,
    HEAD_FOUND              = 18,
    FREE_PLACE              = 19,
    NULL_PREV_MEANING       = 20,
    EMPTY_POP               = 21,
    NULL_RTRN_VAL_MEANING   = 22,
    LEFT_LIST_CANARY_LOST   = 23,
    RIGHT_LIST_CANARY_LOST  = 24,
    UNEXPECTED_CYCLE        = 25,
    WRONG_NEXT_INDEX        = 26,
    WRONG_PREV_INDEX        = 27
} error_codes;

typedef struct list_t
{
    CDO(list_elem_t lcanary;)
    ONDBG(
    const char *name;
    const char *file;
    const char *func;
    int line;
    )
    list_elem_t *data;
    list_elem_t *next;
    list_elem_t *prev;
    list_elem_t free;
    size_t size;
    size_t capacity;
    CDO(list_elem_t rcanary;)
} list_t;

list_elem_t get_head(list_t list);
list_elem_t get_tail(list_t list);
list_elem_t get_next(list_t list, size_t index);
list_elem_t get_prev(list_t list, size_t index);
list_elem_t get_free(list_t list);
list_elem_t next_free(list_t list);
error_codes list_insert_after(list_t *list, list_elem_t elem, list_elem_t pos);
error_codes list_insert_before(list_t *list, list_elem_t elem, list_elem_t pos);
error_codes set_free(list_t *list);
error_codes list_init(list_t *list, list_elem_t elem);
error_codes common_push_after(list_t *list, list_elem_t pos);
error_codes common_push_before(list_t *list, list_elem_t pos);
error_codes list_ctor(list_t *list , size_t start_size);
error_codes fill_array(list_elem_t *arr, list_elem_t elem, size_t start, size_t end);
error_codes prepare_array(list_elem_t **arr, list_elem_t zero_elem, list_elem_t fill_elem, size_t size);
error_codes list_verify(list_t *list);
error_codes list_dtor(list_t *list);
error_codes realloc_arrays(list_t *list);
error_codes realloc_array(list_elem_t **arr, size_t capacity, list_elem_t filler);
error_codes check_for_cycle(list_t list);
error_codes check_indexes(list_t list);
error_codes handle_pop(list_t *list, list_elem_t *rtrn_val, list_elem_t pos);
error_codes list_pop(list_t *list, list_elem_t *rtrn_val, list_elem_t pos);
error_codes check_error(list_t list, error_codes err, const char process[]);
void        destroy_array(list_elem_t **arr, size_t capacity);
#endif