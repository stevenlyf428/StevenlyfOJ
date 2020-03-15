#include <stdio.h>
#include <stdlib.h>
// #include <bits/stdc++.h>
int main()
{
    int count = 0; //记录出现9的次数
    for (int n = 1; n <= 2019; n++)
    {
        if (n % 10 == 9)
        { //个位数，满足条件，次数加一
            count++;
            continue;
        }
        if (n % 100 / 10 == 9)
        { //十位数，满足条件，次数加一
            count++;
            continue;
        }
        if (n % 1000 / 100 == 9)
        {
            count++;
            continue;
        }
        if (n / 1000 == 9)
        {
            count++;
            continue;
        }
    }
    printf("%d\n", count);
    //system("pause");
    return 0;
}
