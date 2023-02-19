#include "soft_list.h"
#include "log.h"

int main()
{
    struct list list;
    list_init(&list);
    // list_print(get_log_file(), &list);
    list_push(&list, 0, 35);
    //list_print(get_log_file(), &list);
    list_delete(&list);
    return 0;
}
