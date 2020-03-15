#include <stdio.h>
#include <cmath>
#define PI atan(1.0) * 4
int main()
{
	int a;
	double b;
	scanf("%d", &a);
	b = a * PI * a;
	printf("%.7lf", b);
	return 0;
}
//test