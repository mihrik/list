#include "list.h"

error_codes list_ctor(list_t *list, size_t start_size)
{
    error_codes err = SUCCESSFUL_RETURN;

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

    if (list->capacity == 0)
        return ZERO_CAPACITY_MEANING;

    if (list->size > list->capacity + 1)
        return SIZE_MORE_THAN_CAPACITY;

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

    position_analysis(list, pos);

    list->tail = pos;
    list_dump(list, "to_check", "push");
    list->size++;

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
        return err;

    destroy_array(&list->data, list->capacity);
    destroy_array(&list->next, list->capacity);

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

void list_dump(list_t *list, const char *reason, const char *process)
{
    assert(list);

    PRINT_COLOR(EXTRA_RED, "=== DATA ===\n\n");

    PRINT_COLOR(BLUE, "size = %zu\n", list->size);
    PRINT_COLOR(BLUE, "capacity = %zu\n", list->capacity);
    PRINT_COLOR(BLUE, "head = %zu\n", list->head);
    PRINT_COLOR(BLUE, "tail = %zu\n", list->tail);
    PRINT_COLOR(BLUE, "free = %zu\n", list->free);
    PRINT_COLOR(BLUE, "data [%p]\n", list->data);
    print_array(list, list->data, "data");
    print_array(list, list->next, "next");
}

void print_array(list_t *list, list_elem_t *arr, const char *arrname)
{
    PRINT_COLOR(BLUE, "%s [%p]\n",arrname, arr);
    for (size_t i = 0; i < list->capacity + 1; i++)
    {
        if (i <= list->size)
        {
            PRINT_COLOR(GREEN, "    *[%zu] = %d\n", i, arr[i]);
        }
        else
        {
            PRINT_COLOR(YELLOW, "     [%zu] = %d\n", i, arr[i]);
        }
    }
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
                                    list->tail = list->free;

                                    break;
                                }
        case        HEAD_FOUND:
                                {
                                    list->next[list->free] = (list_elem_t)list->head;
                                    list->head = list->free;

                                    break;
                                }
        case SUCCESSFUL_RETURN:
                                {
                                    list->next[list->free] = list->next[old_pos];
                                    list->next[old_pos] = (list_elem_t)list->free;

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

    return SUCCESSFUL_RETURN;
}