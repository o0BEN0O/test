#include <stdlib.h>
#include <stdio.h>

void main(void)
{
	unsigned int a=0;
	unsigned int b=0x010;
	unsigned int c=0x100;

	a |=b;
	a |=c;
	printf("a %x\n",a);
}

