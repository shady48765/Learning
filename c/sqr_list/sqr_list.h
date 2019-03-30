
#ifndef SQR_LIST_SQR_LIST_H
#define SQR_LIST_SQR_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct _sqrList{
    int data;
    struct _sqrList *next;
}sqrList;

extern void sqrList_init(sqrList **head);

extern void sqrList_insert_head(sqrList *list, sqrList *new);
extern void sqrList_insert_tail(sqrList *list, sqrList *new);
extern void sqrList_delete(sqrList *list, sqrList *node);
extern void sqrList_show(sqrList *list);
extern bool sqrList_isEmpty(sqrList *list);

extern void sqrList_creat(sqrList *list, int nodeNumber);
extern void sqrList_insertNode(sqrList *list);


#endif //SQR_LIST_SQR_LIST_H
