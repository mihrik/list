#include "list.h"
#include "dump.h"

error_codes list_ctor(list_t *list, size_t start_size)
{
    error_codes err = SUCCESSFUL_RETURN;

    if (list == NULL)
        return NULL_LIST_MEANING;

    list->capacity = start_size;
    list->size = 1;

    if ((err = prepare_array(&list->data, POISON, POISON, start_size)) != SUCCESSFUL_RETURN)
        return err;

    if ((err = prepare_array(&list->next, 0, -1, start_size)) != SUCCESSFUL_RETURN)
        return err;

    if ((err = prepare_array(&list->prev, 0, -1, start_size)) != SUCCESSFUL_RETURN)
        return err;

    return SUCCESSFUL_RETURN;
}

error_codes fill_array(list_elem_t *arr, list_elem_t elem, size_t start, size_t end)
{
    for (size_t i = start; i <= end; i++)
    {
        arr[i] = elem;
    }

    return SUCCESSFUL_RETURN;
}

error_codes prepare_array(list_elem_t **arr, list_elem_t zero_elem, list_elem_t fill_elem, size_t size)
{
    *arr = (list_elem_t *) malloc((size + 1) * sizeof(list_elem_t));
    if (*arr == NULL)
        return MEMORY_ALLOCATION_ERROR;
    fill_array(*arr, fill_elem, 1, size);
    (*arr)[0] = zero_elem;

    return SUCCESSFUL_RETURN;
}

error_codes list_verify(list_t *list)
{
    if (list == NULL)
        return NULL_LIST_MEANING;

    if (list->data == NULL)
        return NULL_DATA_MEANING;

    if (list->next == NULL)
        return NULL_NEXT_MEANING;

    if (list->prev == NULL)
        return NULL_PREV_MEANING;

    if (list->capacity == 0)
        return ZERO_CAPACITY_MEANING;

    if (list->size > list->capacity + 1)
        return SIZE_MORE_THAN_CAPACITY;

    error_codes err = SUCCESSFUL_RETURN;
    if ((err = check_indexes(*list)) != SUCCESSFUL_RETURN)
        return err;

    if (check_for_cycle(*list) != SUCCESSFUL_RETURN)
        return UNEXPECTED_CYCLE;

    CDO(
    if (list->lcanary != LCANARY)
        return LEFT_LIST_CANARY_LOST;

    if (list->rcanary != RCANARY)
        return RIGHT_LIST_CANARY_LOST;
    )

    return SUCCESSFUL_RETURN;
}

error_codes list_push(list_t *list, list_elem_t elem, size_t pos)
{
    error_codes err = SUCCESSFUL_RETURN;
    if ((err = list_verify(list)) != SUCCESSFUL_RETURN)
        return err;

    if ((err = realloc_arrays(list)) != SUCCESSFUL_RETURN)
        return err;

    if (pos <= 0 || pos >list->capacity)
        return UNAVAILABLE_POS;

    list->data[list->free] = elem;

    if ((err = position_analysis(list, pos)) != SUCCESSFUL_RETURN)
        return err;

    list->size++;

    return SUCCESSFUL_RETURN;
}

error_codes list_pop(list_t *list, list_elem_t *rtrn_val, size_t pos)
{
    error_codes err = SUCCESSFUL_RETURN;
    if ((err = list_verify(list)) != SUCCESSFUL_RETURN)
        return err;

    if (rtrn_val == NULL)
        return NULL_RTRN_VAL_MEANING;

    if (pos <= 0 || pos >list->capacity)
        return UNAVAILABLE_POS;

    if (list->next[pos] == -1)
        return EMPTY_POP;

    handle_pop(list, rtrn_val, pos);

    list->size--;

    return SUCCESSFUL_RETURN;
}

error_codes find_free(list_t *list)
{
    for (size_t i = 0; i < list->capacity + 1; i++)
    {
        if (list->next[i] == -1)
        {
            list->free = i;
            return SUCCESSFUL_RETURN;
        }
    }

    return NO_FREE_SPACE;
}

error_codes find_pos(list_t *list, size_t *old_pos, size_t pos)
{
    for (size_t i = 0; i < list->capacity + 1; i++)
    {
        if (list->next[i] == (list_elem_t)pos)
        {
            *old_pos = i;
            return SUCCESSFUL_RETURN;
        }
    }

    if (pos == list->head)
        return HEAD_FOUND;
    return FREE_PLACE;
}

error_codes list_dtor(list_t *list)
{
    error_codes err = SUCCESSFUL_RETURN;
    if ((err = list_verify(list)) != SUCCESSFUL_RETURN)
    {
        free(list->data);
        free(list->next);
        free(list->prev);

        return SUCCESSFUL_RETURN;
    }

    destroy_array(&list->data, list->capacity);
    destroy_array(&list->next, list->capacity);
    destroy_array(&list->prev, list->capacity);

    list->capacity = 0;
    list->free = 0;
    list->tail = 0;
    list->size = 0;
    list->head = 0;

    list = NULL;

    return err;
}


void destroy_array(list_elem_t **arr, size_t capacity)
{
    fill_array(*arr, POISON, 0, capacity);
    free(*arr);
    *arr = NULL;
}

error_codes realloc_arrays(list_t *list)
{
    error_codes err = SUCCESSFUL_RETURN;

    if ((err = find_free(list)) != SUCCESSFUL_RETURN)
    {
        list->capacity *= 2;

        if((err = realloc_array(&list->data, list->capacity, POISON)) != SUCCESSFUL_RETURN)
            return err;
        if((err = realloc_array(&list->next, list->capacity, -1)) != SUCCESSFUL_RETURN)
            return err;
        if((err = realloc_array(&list->prev, list->capacity, -1)) != SUCCESSFUL_RETURN)
            return err;

        find_free(list);
    }

    return SUCCESSFUL_RETURN;
}

error_codes realloc_array(list_elem_t **arr, size_t capacity, list_elem_t filler)
{
    list_elem_t *tmp = (list_elem_t *) realloc(*arr, (capacity + 1) * sizeof(list_elem_t));
    if (tmp == NULL)
        return MEMORY_ALLOCATION_ERROR;

    *arr = tmp;
    fill_array(*arr, filler, capacity / 2 + 1, capacity);

    return SUCCESSFUL_RETURN;
}

error_codes position_analysis(list_t *list, size_t pos)
{
    error_codes err = SUCCESSFUL_RETURN;

    if (list->size > 1)
    {
        if ((err = common_handle_position(list, pos)) != SUCCESSFUL_RETURN)
            return err;
    }
    else
    {
        handle_init_position(list);
    }

    return SUCCESSFUL_RETURN;
}

error_codes common_handle_position(list_t *list, size_t pos)
{
    size_t old_pos = 0;

    error_codes err = find_pos(list, &old_pos, pos);

    switch(err)
    {
        case        FREE_PLACE:
                                {
                                    list->next[list->tail] = (list_elem_t)list->free;
                                    list->next[list->free] = 0;
                                    list->prev[list->free] = (list_elem_t)list->tail;
                                    list->tail = list->free;

                                    break;
                                }
        case        HEAD_FOUND:
                                {
                                    list->next[list->free] = (list_elem_t)list->head;
                                    list->prev[list->free] = 0;
                                    list->prev[list->head] = (list_elem_t)list->free;
                                    list->head = list->free;

                                    break;
                                }
        case SUCCESSFUL_RETURN:
                                {
                                    list->next[list->free] = list->next[old_pos];
                                    list->next[old_pos] = (list_elem_t)list->free;
                                    list->prev[list->free] = (list_elem_t)old_pos;
                                    list->prev[pos] = (list_elem_t)list->free;

                                    break;
                                }
        default               :
                                {
                                    return UNAVAILABLE_POS;
                                }
    }

    return SUCCESSFUL_RETURN;
}

error_codes handle_init_position(list_t *list)
{
    list->next[list->free] = 0;
    list->head = list->free;
    list->tail = list->free;
    list->prev[list->free] = 0;

    return SUCCESSFUL_RETURN;
}

error_codes handle_pop(list_t *list, list_elem_t *rtrn_val, size_t pos)
{
    *rtrn_val = list->data[pos];
    if (pos == list->tail)
    {
        list->tail = (size_t)list->prev[pos];
        if (list->tail != 0)
            list->next[list->tail] = 0;
        else
            list->head = 0;
    }
    else if (pos == list->head)
    {
        list->head = (size_t) list->next[pos];
        list->prev[list->head] = 0;
    }
    else
    {
        list->next[list->prev[pos]] = list->next[pos];
        list->prev[list->next[pos]] = list->prev[pos];
    }
    list->prev[pos] = -1;
    list->next[pos] = -1;
    list->data[pos] = POISON;

    return SUCCESSFUL_RETURN;
}

error_codes check_for_cycle(list_t list)
{
    size_t i = list.head;
    size_t counter = 0;

    while (i != 0)
    {
        if (counter++ >= list.size)
            return UNEXPECTED_CYCLE;

        i = (size_t)list.next[i];
    }

    return SUCCESSFUL_RETURN;
}

error_codes check_indexes(list_t list)
{
    for (size_t i = 0; i <= list.capacity; i++)
    {
        if (list.next[i] < -1 || ((size_t)list.next[i] > list.capacity && list.next[i] > 0))
            return WRONG_NEXT_INDEX;

        if (list.prev[i] < -1 || ((size_t)list.prev[i] > list.capacity && list.prev[i] > 0))
            return WRONG_PREV_INDEX;
    }

    return SUCCESSFUL_RETURN;
}

void check_error(list_t list, error_codes err, const char process[])
{
    ONDBG(
    switch(err)
    {
        case MEMORY_ALLOCATION_ERROR:
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "MEMORY ALLOCATION ERROR (error_code = %d), while doing %s\n", MEMORY_ALLOCATION_ERROR, process);

                                        break;
                                     }
        case NULL_LIST_MEANING      :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "NULL LIST MEANING (error_code = %d), while doing %s\n", NULL_LIST_MEANING, process);

                                        break;
                                     }
        case NULL_DATA_MEANING      :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "NULL DATA MEANING (error_code = %d), while doing %s\n", NULL_DATA_MEANING, process);

                                        break;
                                     }
        case NULL_NEXT_MEANING      :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "NULL NEXT MEANING (error_code = %d), while doing %s\n", NULL_NEXT_MEANING, process);

                                        break;
                                     }
        case NULL_PREV_MEANING      :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "NULL PREV MEANING (error_code = %d), while doing %s\n", NULL_PREV_MEANING, process);

                                        break;
                                     }
        case RIGHT_LIST_CANARY_LOST      :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "RIGHT LIST CANARY LOST (error_code = %d), while doing %s\n", RIGHT_LIST_CANARY_LOST, process);

                                        break;
                                     }
        case LEFT_LIST_CANARY_LOST       :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "LEFT LIST CANARY LOST (error_code = %d), while doing %s\n", LEFT_LIST_CANARY_LOST, process);

                                        break;
                                     }
        case ZERO_CAPACITY_MEANING  :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "ZERO_CAPACITY_MEANING (error_code = %d), while doing %s\n", ZERO_CAPACITY_MEANING, process);

                                        break;
                                     }
        case SIZE_MORE_THAN_CAPACITY:
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "SIZE_MORE_THAN_CAPACITY (error_code = %d), while doing %s\n", SIZE_MORE_THAN_CAPACITY, process);

                                        break;
                                     }
        case UNAVAILABLE_POS        :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "SIZE_MORE_THAN_CAPACITY (error_code = %d), while doing %s\n", UNAVAILABLE_POS, process);

                                        break;
                                     }
        case EMPTY_POP              :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "EMPTY_POP (error_code = %d), while doing %s, check: <%s> to get more information\n", EMPTY_POP, process, FNAME);

                                        print_html(list, FNAME, process, PIC_NAME, "EMPTY POP");

                                        break;
                                     }
        case NULL_RTRN_VAL_MEANING  :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "NULL_RTRN_VAL_MEANING (error_code = %d), while doing %s, check: <%s> to get more information\n", NULL_RTRN_VAL_MEANING, process, FNAME);

                                        print_html(list, FNAME, process, PIC_NAME, "NULL_RTRN_VAL_MEANING");

                                        break;
                                     }
        case UNEXPECTED_CYCLE       :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "UNEXPECTED_CYCLE (error_code = %d), while doing %s, check: <%s> to get more information\n", UNEXPECTED_CYCLE, process, FNAME);

                                        print_html(list, FNAME, process, PIC_NAME, "UNEXPECTED_CYCLE");

                                        break;
                                     }
        case WRONG_NEXT_INDEX       :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "WRONG_NEXT_INDEX (error_code = %d), while doing %s, check: <%s> to get more information\n", WRONG_NEXT_INDEX, process, FNAME);

                                        print_html(list, FNAME, process, PIC_NAME, "WRONG_NEXT_INDEX");

                                        break;
                                     }
        case WRONG_PREV_INDEX       :
                                     {
                                        PRINT_COLOR(EXTRA_RED, "list: <%s> created in <%s:%d> in function <%s>\n", list.name, list.file, list.line, list.func);
                                        PRINT_COLOR(EXTRA_RED, "WRONG_PREV_INDEX (error_code = %d), while doing %s, check: <%s> to get more information\n", WRONG_PREV_INDEX, process, FNAME);

                                        print_html(list, FNAME, process, PIC_NAME, "WRONG_PREV_INDEX");

                                        break;
                                     }
        default                     :   break;
    }
    )

}

