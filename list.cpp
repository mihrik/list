#include "list.h"

int main(void)
{
    list_elem_t rtrn_val = 0;

    list_t list = {CDO(.lcanary = LCANARY, )
        ONDBG(.name = "list", .file = __FILE__, .func = __func__, .line = __LINE__,)
        .data = NULL, .next = NULL, .prev = NULL, .free = 0, .head = 0, .tail = 0, .size = 0, .capacity = 0
        CDO(, .rcanary = RCANARY)};
    LIST_CTOR(&list, 3)

    LIST_PUSH(&list, 1, 1);
    LIST_PUSH(&list, 2, 2);
    LIST_PUSH(&list, 3, 3);
    LIST_PUSH(&list, 4, 4);
    LIST_PUSH(&list, 5, 5);
    LIST_PUSH(&list, 6, 6);
    LIST_POP(&list, &rtrn_val, 5);
    LIST_PUSH(&list, 10, 3);
    LIST_PUSH(&list, 122, 1);

    printf("elem = %d\n", list.data[1]);

    LIST_DTOR(&list);

    return 0;
}