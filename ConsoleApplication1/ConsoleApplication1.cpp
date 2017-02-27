// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#pragma pack(8)

#include "stdafx.h"
#include <stdlib.h>

struct student
{
	int i;
	int a[2];
} *p;

const int HIGH = 5;


void Fun(int *varp)
{
	printf("实参p%d\n", &varp);
}

int main()
{
	/*int *p = NULL;
	printf("%d\n",sizeof(p));
	printf("%d\n", sizeof(*p));
	int a[100];
	printf("%d\n", sizeof(a[100]));
	printf("%ld\n", sizeof(&a));
	printf("%ld\n", sizeof(&a[0]));

	unsigned long address = (unsigned long)&HIGH;

	printf("address of a[1] is: %lu\n", (unsigned long)&a[1]);
	printf("address of a is: %lu\n", (unsigned long)&a);
	printf("address of a + 1 is: %lu\n", (unsigned long)(&a+1));
	printf("address between of a and a + 1 is: %lu\n", (unsigned long)((&a + 1) - &a));
	printf("the value between address of a and a + 1 is: %lu\n", (unsigned long)(&a + 1) - (unsigned long)&a);
*/


	/*float num = 10000000000.00f + 0.00000000001f;
	printf("%f\n", num);*/

	/*printf("%d\n", sizeof(student));
	p->i = 5;*/
//#define NUM(a,b,c) a##b##c
//#define STR(a,b,c) a##b##c
//
//	printf("%d\n", NUM(1, 2, 3));
//	printf("%s\n", STR("aa", "bb", "cc"));

//#define PCHAR int*;
//	
//	int* p3, p4;
//	int i = 5;
//	p3 = &i;
	/*p4 = p3;
	*p4 = 0;*/

	//int i = 5;
	//int *p = &i;
	//*p = NULL;
	//p = NULL;
	//printf("%d\n", &i);

	int i = 8;
	int *p = &i;
	printf("形参p%d\n", &p);
	Fun(p);
	system("pause");

    return 0;
}

