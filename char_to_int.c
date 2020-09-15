#include <stdio.h>
#include <stdlib.h> 

void main(void)
{
	char buf[4]="6400";
	int temp;
	sscanf(buf,"%d",&temp);
	temp = temp/100;
	printf("%d\n",temp);
}