// #include <stdio.h>
#include <bits/stdc++.h>
const int N = 10000;
int n;
int a[N];
// 求数组a[N] 在区间[b,n)的最大值
int Max(int b)
{
	int ans = a[b];
	for (int i = b; i < n; ++i)
		if (a[i] > ans)
			ans = a[i];
	return ans;
}
int main()
{
	scanf("%d", &n);
	for (int i = 0; i < n; ++i)
		scanf("%d", &a[i]);
	int Min = a[0], ans;
	for (int i = 1; i < n - 1; ++i)
	{
		if (Min < a[i] && a[i] < Max(i + 1))
			ans++;
		if (Min > a[i])
			a[i] = Min;
	}
	printf("%d\n", ans);
}
