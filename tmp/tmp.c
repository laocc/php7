#include <stdio.h>
#include <string.h>

int main() {
	char *a, *b,*d;
	int c, i;

	a = "AZaz";
	b = "b";
	for (i = 0; i < strlen(a); i++) {
//		c = &a[i]+32;
		d[i]=a[i];
//		if(c>=65 && c<=90)c+=32;
//		printf("[%d],", c);
	}

	c = strcmp(b, a);
	printf("%d\n", c);
}
