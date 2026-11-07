#include "h_files/list.h"

int main(void)
{
    list_elem_t rtrn_val = 0;

    list_t list = {CDO(.lcanary = LCANARY, )
        ONDBG(.name = "list", .file = __FILE__, .func = __func__, .line = __LINE__ - 1,)
        .data = NULL, .next = NULL, .prev = NULL, .free = 0, .size = 0, .capacity = 0
        CDO(, .rcanary = RCANARY)};
    LIST_CTOR(&list, 3)

    LIST_INSERT_AFTER(&list, 10, 0);
    LIST_INSERT_AFTER(&list, 20, 1);
    LIST_INSERT_AFTER(&list, 30, 2);
    LIST_INSERT_AFTER(&list, 40, 3);
    LIST_POP(&list, &rtrn_val, 1);
    LIST_POP(&list, &rtrn_val, 2);
    LIST_POP(&list, &rtrn_val, 3);
    LIST_INSERT_BEFORE(&list, 20, 4);
    LIST_INSERT_AFTER(&list, 30, 3);
    LIST_INSERT_AFTER(&list, 40, 2);
    list.next[3] = 6;
    LIST_INSERT_BEFORE(&list, 4, 110);

    LIST_DTOR(&list);

    return 0;
}