#include "soft_list.h"

int main()
{
    struct list list;
    list_init(&list);
    list_delete(&list);
    return 0;
}
