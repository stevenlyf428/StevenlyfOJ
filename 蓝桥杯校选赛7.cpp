// #include <stdio.h>
// #include <string.h>
#include <bits/stdc++.h>
using namespace std;
char s[100];
int i = 0;
//判断字符是否为元音字母 a, e, i, o, u
bool judeg(char c)
{
	return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}
//判断辅音段的个数
int f1()
{
	int ans = 0;
	for (; i < strlen(s); ++i)
		if (judeg(s[i]) == false)
			ans++;
		else
			break;
	return ans;
}
// @brief 判断元音段的个数
int f2()
{
	int ans = 0;
	for (; i < strlen(s); ++i)
		if (judeg(s[i]) == true)
			ans++;
		else
			break;
	return ans;
}
int main()
{
	scanf("%s", s);
	if (f1() > 0 && f2() > 0 && f1() > 0 && f2() > 0 && i == strlen(s))
		printf("yes\n");
	else
		printf("no\n"); //短路性质
}
