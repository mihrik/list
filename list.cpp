#include "list.h"

int main(void)
{
    list_elem_t rtrn_val = 0;

    list_t list = {CDO(.lcanary = LCANARY, )
        ONDBG(.name = "list", .file = __FILE__, .func = __func__, .line = __LINE__ - 1,)
        .data = NULL, .next = NULL, .prev = NULL, .free = 0, .head = 0, .tail = 0, .size = 0, .capacity = 0
        CDO(, .rcanary = RCANARY)};
    LIST_CTOR(&list, 3)

    LIST_PUSH(&list, 10, 1);
    LIST_PUSH(&list, 20, 2);
    LIST_PUSH(&list, 30, 3);

    list.next[2] = 1;

    LIST_PUSH(&list, 40, 3);

    LIST_DTOR(&list);

    return 0;
}