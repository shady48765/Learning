#include "undir_link_list.h"

int main(void)
{
	_list *List;
	list_init(&List);
	for (int counter = 0; counter < 10; counter++) {
		list_insert(List);
	}
	list_show(List);
    printf("---------- list invert ----------------\n");
    list_invert(List);

	printf("\n-------------- FINISHED ----------------\n");

	bool listStatus = list_destory(List);
	if(true == listStatus)
		printf("list has been destroyed.\n");
	else
		printf("list destroy operation failed.\n");

	list_show(List);
	return 0;
}