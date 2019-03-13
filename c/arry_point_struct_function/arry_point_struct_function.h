//
// Created by QUAN on 6/28/2018.
//

#ifndef ARRY_POINT_FUNCTION_ARRY_POINT_STRUCT_FUNCTION_H
#define ARRY_POINT_FUNCTION_ARRY_POINT_STRUCT_FUNCTION_H

#define BUFF_SIZE 100

typedef struct _class1{
	char *version;
	int data;
	char name[BUFF_SIZE];
}_class1_typedef;

typedef struct _class2{
	_class1_typedef param;
	_class1_typedef *ptr_param;

	int (*function)(char *, int);
}_class2_typedef;











#endif //ARRY_POINT_FUNCTION_ARRY_POINT_STRUCT_FUNCTION_H
