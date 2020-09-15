#include<stdio.h>
#include<string.h>

int stringFind(const char *string, const char *dest) {
	if (string == NULL || dest == NULL) return -1;

	int i = 0;
	int j = 0;
	while (string[i] != '\0') {
	    if (string[i] != dest[0]) {
	        i ++;
	        continue;
	    }
	    j = 0;
	    while (string[i+j] != '\0' && dest[j] != '\0') {
	        if (string[i+j] != dest[j]) {
	            break;
	        }
	        j ++;
	    }
	    if (dest[j] == '\0') return j;
	    i ++;
	}
	return -1;
}

int main()
{
	char buf[64]="upgrade uboot done !";
	char buf2[12]="failed";
	int ret=0;
	ret = stringFind(buf,buf2);
	printf("%d %d\n",strlen(buf2),ret);
	return 0;
}
