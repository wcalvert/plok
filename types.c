#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vm.h"
#include "types.h"

int_obj * int_factory(int initial) {
	int_obj *ptr = malloc(sizeof(int_obj));
	if(ptr == NULL) {
		vm_error(ERR_NO_MEM);
	}
	ptr->o_type = INTEGER;
	ptr->val = initial;
	ptr->destructor = int_destructor;
	return ptr;
}

void int_destructor(int_obj **n) {
	if(*n != NULL) {
		free(*n);
		*n = NULL;
	}
}

float_obj * float_factory(float initial) {
	float_obj *ptr = malloc(sizeof(float_obj));
	if(ptr == NULL) {
		vm_error(ERR_NO_MEM);
	}
	ptr->o_type = FLOAT;
	ptr->val = initial;
	ptr->destructor = float_destructor;
	return ptr;
}

void float_destructor(float_obj **n) {
	if(*n != NULL) {
		free(*n);
		*n = NULL;
	}
}

string_obj * string_factory(const char * initial) {
	string_obj *ptr = malloc(sizeof(string_obj));
	ptr->str = malloc(strlen(initial) + 1);
	if(ptr == NULL || ptr->str == NULL) {
		vm_error(ERR_NO_MEM);
	}
	ptr->o_type = STRING;
	strcpy(ptr->str, initial);
	ptr->destructor = string_destructor;
	return ptr;
}

void string_destructor(string_obj **s) {
	if((*s)->str != NULL) {
		free((*s)->str);
		(*s)->str = NULL;
	}
	if(*s != NULL) {
		free(*s);
		*s = NULL;
	}
}

/*int main()
{
	string_obj 	*s = string_factory("y helo thar");
	int_obj *i = int_factory(123);
	float_obj *f = float_factory(152.38f);

	printf("string: %s\n", 	obj_to_string_raw((object *)s));
	printf("integer: %s\n", obj_to_string_raw((object *)i));
	printf("float: %s\n", 	obj_to_string_raw((object *)f));

	string_destructor(&s);
	int_destructor(&i);
	float_destructor(&f);
}*/
