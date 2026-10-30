#include "list.h"

int main(void)
{
    error_codes err = SUCCESSFUL_RETURN;

    list_elem_t *rtrn_val = 0;

    list_t list = {.data = NULL, .next = NULL, .prev = NULL, .free = 0, .head = 0, .tail = 0, .size = 0, .capacity = 0};
    if ((err = list_ctor(&list, 3)) != SUCCESSFUL_RETURN)
        return err;

    list_push(&list, 1, 1);
    list_push(&list, 2, 2);
    list_push(&list, 3, 3);
    err = list_push(&list, 4, 4);
    printf("err = %d\n", err);
    list_push(&list, 5, 5);
    list_push(&list, 6, 6);
    list_push(&list, 10, 3);
    list_push(&list, 122, 1);
    list_push(&list, 7, 6);
    list_push(&list, 8, 6);


    printf("elem = %d\n", list.data[1]);

    if ((err = list_dtor(&list)) != SUCCESSFUL_RETURN)
        return err;

    return 0;
}