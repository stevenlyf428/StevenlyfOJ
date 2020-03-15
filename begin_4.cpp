#include <stdio.h>

int main()
{
	int a = 1;
	int b = 1;
	int n;
	int c;
	int i;
	scanf("%d", &n);
	for (i = 3; i <= n; i++)
	{
		c = (a + b) % 10007;
		a = b;
		b = c;
	}
	if (n == 1)
		printf("1");
	else if (n == 2)
		printf("1\n");
	else
		printf("%d", c);
	return 0;
}
