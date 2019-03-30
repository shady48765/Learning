
#include "sqr_list.h"

#define DEBUG

/**
 * function is to initialize a list, creat a list head.
 * @param head
 */
void sqrList_init(sqrList **head)
{
    *head = (sqrList *)malloc(sizeof(sqrList));
    if(NULL == *head)
    {
        printf("malloc error\n");
        exit(1);
    }
    else
    {
        (*head)->next = NULL;
    }
}

/**
 * insert a new node to a created list head
 * @param list : define the list which need to be insert
 * @param new : define the node need to be insert
 */
void sqrList_insert_head(sqrList *list, sqrList *new)
{
    // change the pointer frem list->next to new->next;
    new ->next = list ->next;
    list ->next = new;
}
/**
 * insert a new node to a list tail
 * @param list : list need to be insert
 * @param new : node need to be insert
 */
void sqrList_insert_tail(sqrList *list, sqrList *new)
{
    //get list first node
    sqrList *temp = list ->next;
    // find the last node. when next node is empty, current is the last node
    while (NULL != temp ->next)
    {
        temp = temp->next;
    }
    //got the last node ready to inser
    new ->next = temp ->next;
    temp ->next = new;
}
void sqrList_delete(sqrList *list, sqrList *node)
{
    list ->next = node ->next;
    free(node);
    node = NULL;
}
void sqrList_show(sqrList *list)
{
    sqrList *p = list;
    printf("heard: ");
    for(p = list->next; p != NULL; p = p ->next)
    {
        printf("---> %d", p ->data);
    }
    printf("\n");
}
bool sqrList_isEmpty(sqrList *list)
{
    if(NULL == list ->next)
        return true;
    else
        return false;
}
/**
 * sqrList_creat is a function to creat a list with in nodeNumber node.
 * @param list : define the list need to be creat.
 * @param nodeNumber : define how much node need to be creat.
 */
void sqrList_creat(sqrList *list, int nodeNumber)
{
    int m;
    //creat function, list is empty, p has to point to list
    sqrList *p = list;
    sqrList *new;

    for(m = 0; m < nodeNumber; m++)
    {
        new = (sqrList *)malloc(sizeof(sqrList));
        if(NULL == new)
        {
            printf("malloc error\n");
            exit(1);
        }

        printf("please in put data %d :", m + 1);
        scanf("%d", &new->data);

        // index move forward, conform insert to list tail.
        new ->next = p->next;
        p ->next = new;
        //move list index to next node(positon)
        p = p->next;

        #ifdef DEBUG
            sqrList_show(list);
        #endif
    }
}
/**
 *
 * @param list
 */
void sqrList_insertNode(sqrList *list)
{
    sqrList *new = (sqrList *)malloc(sizeof(sqrList));
    int n;
    printf("Input insert number: ");
    scanf("%d", &(new ->data));
    sqrList_insert_tail(list, new);
}

