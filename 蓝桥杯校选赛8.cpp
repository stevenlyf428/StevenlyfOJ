#include <bits/stdc++.h>
// #include<cstdlib>
/*
　1. 第一项为 n；
　2. 第二项不超过 n；
　3. 从第三项开始，每一项小于前两项的差的绝对值
*/
int ans = 0;
const int MOD = 1e4;
void mod(int &d)
{
	if (d > MOD)
		d -= MOD;
}

//该方法可能只有一部分
void recursion(int a, int b)
{
	int z = abs(a - b);
	ans++;
	mod(ans);
	for (int i = 1; i < z; ++i)
	{
		recursion(b, i);
	}
}

int main()
{
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i)
		recursion(4, i);
	printf("%d\n", ans);
}
