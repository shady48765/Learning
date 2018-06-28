//
// Created by QUAN on 6/28/2018.
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "arry_point_struct_function.h"

#if 0
/*
 * no neet to add .h file
 * */
struct sss{
	char *pstr;
	int data;
	char arry_str[50];
};

int main(void)
{
	struct sss class = {
			.pstr = "version_0.1",
			.data = 100,
			.arry_str = "arry_string",
	};

	struct sss *ptr_class = &class;

	printf("class.pstr = %s\n", class.pstr);
	printf("class.data = d\n", class.data);
	printf("class.arry_str = %s\n", class.arry_str);
	printf("---------------------------------\n");
	printf("ptr_class -> pstr = %s\n", ptr_class->pstr);
	printf("ptr_class -> data = d\n", ptr_class->data);
	printf("ptr_class -> arry_str = %s\n", ptr_class->arry_str);
	return 0;
}

#else

int printf_func(char *str, int size)
{
	char *buff = (char *)malloc(sizeof(char));
	printf("function point using:\n");
	snprintf(buff, size, "%s", str);
	printf("%s\n", buff);
	printf("------------------------------------------------------------\n");
	return 1;
}

int main(void)
{
	_class1_typedef *class1;
	_class2_typedef *class2;

	class1 = (_class1_typedef *)malloc(sizeof(_class1_typedef));
	class2 = (_class2_typedef *)malloc(sizeof(_class2_typedef));

	class1 ->version = "Version 0.1";
	class1 ->data = 100;
	strcpy(class1->name, "quan");

	class2 ->function = printf_func;
	// class ->param is a class1 type, don't have suitable decription
	//class2 ->param = class1;
	class2 ->ptr_param = class1;


	printf("class1 address = %p\n", &class1);
	printf("class1 ->version = %s\n", class1->version);
	printf("class1 ->data = %d\n", class1 ->data);
	printf("class1 ->name = %s\n", class1 ->name);
	printf("------------------------------------------------------------------\n");
	printf("class2 ->ptr_param point address =  %p\n", &class2 ->ptr_param);
	printf("class2 ->ptr_param pointed addr =  %p\n", class2 ->ptr_param);
	printf("class2 ->function point using answer = %d\n", class2 ->function("function_point", strlen("function_point")));

	return  0;
}

#endif