#include <stdio.h>
#include <malloc.h>


struct class1
{
	void (*class1_func1)(void);
	int (*class1_func2)(int a, int b);
	int data;
};

struct class2 {
	struct class1 *ptr;
};

void func1(void)
{
	printf("------ func1 -------\n");
}

int func2(int a, int b)
{
	return (a + b);
}

int pt_func(char *str)
{
	printf("%s\n", str);
	return 1;
}


int main(void)
{
	int a = 10;
	int b = 10;
	int temp, temp2;

	int prtReturn;

	int (*prtFunc)(char *);

	prtFunc = pt_func;

	struct class1 func;

	struct class2 *class2_ptr = (struct class2 *)malloc(sizeof(struct class2));
	class2_ptr->ptr = &func;
	/**
	 * ------------ segment fault --------------------------
	 * 1. class2_ptr ->ptr = NULL;
	 * 2. class2_ptr ->ptr = (struct class1 *)malloc(sizeof(struct class1));
	 */

	func.class1_func1 = func1;
	func.class1_func2 = func2;
	func.data = 10;

	//normally function point
	printf("--- function point func2 return = %d ----\n", (*prtFunc)("----------- check --------"));

	temp = func2(a, b);
	printf("temp = %d\n", temp);
//-------------------------------------------------------------
	temp2 = func.class1_func2(a, b);
	printf("func.class1_func2 = %d\n", temp2);
//-------------------------------------------------------------

	printf("class2_ptr->ptr->data =  %d\n", class2_ptr->ptr->data);
	temp2 = class2_ptr->ptr->class1_func2(a, b);
	printf("class2_ptr->ptr->class1_func2 = %d\n", temp2);

	printf("--------------------- END ----------------------\n");

	return 0;
}

