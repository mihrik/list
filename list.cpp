#include "h_files/list.h"

int main(void)
{
    list_elem_t rtrn_val = 0;

    list_t list = {CDO(.lcanary = LCANARY, )
        ONDBG(.name = "list", .file = __FILE__, .func = __func__, .line = __LINE__ - 1,)
        .data = NULL, .next = NULL, .prev = NULL, .free = 0, .size = 0, .capacity = 0
        CDO(, .rcanary = RCANARY)};
    LIST_CTOR(&list, 3)

    for (int i = 1; i <= 4; i++)
    {
        LIST_INSERT_AFTER(&list, i * 10, get_tail(list));
    }

    for (int i = 1; i <= 2; i++)
    {
        LIST_INSERT_BEFORE(&list, -i * 10, get_head(list));
    }

    list_elem_t pos = get_head(list);

    for (int i = 0; i < 2; i++)
        pos = list.next[pos];

    LIST_INSERT_AFTER(&list, 777, pos);

    LIST_INSERT_BEFORE(&list, 888, pos);

    LIST_INSERT_AFTER(&list, 999, get_tail(list));

    LIST_DTOR(&list);

    return 0;
}