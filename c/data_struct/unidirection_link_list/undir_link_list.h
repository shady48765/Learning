//
// Created by QUAN on 5/31/2018.
//

#ifndef __UNDIR_LINK_LIST_H
#define __UNDIR_LINK_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int dataType;
typedef struct _data_type{
	dataType data;
}data_type;

typedef struct _node{
	data_type data;
	struct _node *next_node;
}_list;

bool list_init(_list **ss);
bool list_insert(_list *ss);
bool list_delete_current(_list *ss, _list *currentNode);
bool list_delete_node(_list *ss, int nodeID, dataType data);
int list_search(_list *ss, int nodeID, dataType data);
data_type list_get_item(_list *ss, dataType data);
bool list_destory(_list *ss);
void list_show(_list *ss);
bool list_isEmpty(_list *ss);
void list_invert(_list *ss);


#endif //UNIDIRECTION_LINK_LIST_UNIDIRECTION_LINK_LIST_H
