#include "sqr_list.h"



int main(void)
{
    int num;
    sqrList *List;

    printf("Please input node: ");
    scanf("%d", &num);

    sqrList_init(&List);
    sqrList_creat(List, num);
    sqrList_insertNode(List);
    sqrList_show(List);



    return 0;
}