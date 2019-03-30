//
// Created by QUAN on 5/31/2018.
//
#include "undir_link_list.h"

bool list_init(_list **ss)
{
	*ss = (_list *)malloc(sizeof(_list));
	if(NULL == *ss)
		return false;
	// 结构体内指针单独初始化或者不单独初始化结果没有区别
	// 是否为整个结构体初始化了所以结构体内指针不用单独初始化？？？？？
//	(*ss)->next_node = (_list *)malloc(sizeof(_list));
//	if(NULL == (*ss)->next_node)
//		return false;

	(*ss)->next_node = NULL;
	return true;
}
bool list_insert(_list *ss)
{
	static int insertCoutner = 1;

	_list *new = (_list *)malloc(sizeof(_list));
	if(NULL == new)
		return false;
//	new->next_node = (_list *)malloc(sizeof(_list));
//	if(NULL == new)
//		return false;

	new->data.data = insertCoutner;
	new->next_node = ss->next_node;
	ss->next_node = new;
	insertCoutner++;
}
bool list_delete_current(_list *ss, _list *currentNode)
{
	ss ->next_node = currentNode ->next_node;
	free(currentNode);
	currentNode = NULL;
}
bool list_delete_node(_list *ss, int nodeID, dataType data)
{

}
/**
 *
 * @param ss
 * @param nodeID : if choose search by data, set nodeID is -1;
 * @param data : if data is null transfer in data param is null;
 * @return
 */
int list_search(_list *ss, int nodeID, dataType data)
{
	_list *tempList = ss ->next_node;
	int nodeIndex = 1;
	bool successFlag = false;

	if(-1 == nodeID)	//search by data
	{
		do{
			if(data == ss->data.data)
				successFlag = true;
			else
				nodeIndex++;
		}while (NULL == tempList->next_node);
	}
	else	//search by id
	{
		while((nodeIndex <= nodeID) && (NULL != tempList ->next_node))
		{
			tempList = tempList ->next_node;
			//???????????????????
		}

	}


}
data_type list_get_item(_list *ss, dataType data)
{

}
/**
 * destory the sepcification whole list keep the list head
 * @param ss : list need to be destroyed.
 * @return  if success return true else return false
 */
bool list_destory(_list *ss)
{
	_list *tempList = ss;


	while (NULL != tempList ->next_node)
	{
		_list *currentNode = tempList ->next_node;
		tempList ->next_node = currentNode ->next_node;
		free(currentNode);
		currentNode = NULL;
	}
	if(true == list_isEmpty(ss))
		return true;
	else
		return false;

}
void list_show(_list *ss)
{
	if(false == list_isEmpty(ss))
	{
		_list *tempList = ss;
		printf("List: ");
		for(tempList = tempList ->next_node; NULL != tempList; tempList = tempList->next_node)
			printf("---> %d", tempList->data.data);
		printf("\n");
	}
	else
		printf("\n ----------- List is empty ------------ \n");

}

bool list_isEmpty(_list *ss)
{
	if(NULL == ss->next_node)
		return true;
	else
		return false;
}


void list_invert(_list *ss)
{
    _list * tempList = ss ->next_node;
    _list * newList = (_list *)malloc(sizeof(_list));
    tempList ->next_node = NULL;
    if(true == list_isEmpty(ss))
    {
        for (newList = ss ->next_node; ss->next_node != NULL ; ss = ss ->next_node) {
            tempList = (_list *)malloc(sizeof(_list));
            tempList = newList ->next_node;
            newList ->next_node = ss;
            free(tempList);
            tempList = NULL;

        }
        ss = newList;
    }
    printf("after invert operate list is: \n");
    list_show(ss);
}