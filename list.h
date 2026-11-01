#ifndef LIST_H
#define LIST_H

#include "colors.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

typedef int list_elem_t;
const int POISON = 0x12345678;
const int LCANARY = 0x69696969;
const int RCANARY = 0x52525252;

#define LIST_ON_DEBUG
#ifdef LIST_ON_DEBUG
#define ONDBG(...) __VA_ARGS__
#define LIST_DTOR(list) {\
    check_error((*list), list_dtor((list)), "LIST_DTOR");\
}
#define LIST_POP(list, rtrn_val, pos) {\
    check_error((*list), list_pop((list), (rtrn_val), (pos)), "LIST_POP");\
}
#define LIST_PUSH(list, elem, pos) {\
    check_error((*list), list_push((list), (elem), (pos)), "LIST_PUSH");\
}
#define LIST_CTOR(list, start_size) {\
    check_error((*list), list_ctor((list), (start_size)), "LIST_CTOR");\
}
#else
#define ONDBG(...)
#define LIST_CTOR(list, start_size) list_ctor((list), (start_size))
#define LIST_PUSH(list, elem, pos) list_push((list), (elem), (pos))
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
    size_t free;
    size_t head;
    size_t tail;
    size_t size;
    size_t capacity;
    CDO(list_elem_t rcanary;)
} list_t;

error_codes list_ctor(list_t *list , size_t start_size);
error_codes fill_array(list_elem_t *arr, list_elem_t elem, size_t start, size_t end);
error_codes prepare_array(list_elem_t **arr, list_elem_t zero_elem, list_elem_t fill_elem, size_t size);
error_codes find_free(list_t *list);
error_codes list_push(list_t *list, list_elem_t elem, size_t pos);
error_codes find_pos(list_t *list, size_t *old_pos, size_t pos);
error_codes list_verify(list_t *list);
error_codes list_dtor(list_t *list);
error_codes realloc_arrays(list_t *list);
error_codes realloc_array(list_elem_t **arr, size_t capacity, list_elem_t filler);
error_codes position_analysis(list_t *list, size_t pos);
error_codes common_handle_position(list_t *list, size_t pos);
error_codes handle_init_position(list_t *list);
error_codes check_for_cycle(list_t list);
error_codes check_indexes(list_t list);
error_codes handle_pop(list_t *list, list_elem_t *rtrn_val, size_t pos);
error_codes list_pop(list_t *list, list_elem_t *rtrn_val, size_t pos);
void        check_error(list_t list, error_codes err, const char process[]);
void        destroy_array(list_elem_t **arr, size_t capacity);
#endif