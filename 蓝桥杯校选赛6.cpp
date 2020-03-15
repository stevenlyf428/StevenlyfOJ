// #include <stdio.h>
#include <bits/stdc++.h>
int n;
int count;
void fun(int num, int temp)
{
    if (num > n)
    {
        return;
    }
    else
    {
        count++;
    }
    for (int i = temp; i < 10; i++)
    {
        fun(num * 10 + i, i);
    }
}
int main()
{
    scanf("%d", &n);
    fun(0, 1);
    printf("%d", count - 1);
    return 0;
}
