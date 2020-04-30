/*  
给出一条长度为 L 的线段，除了头和尾两个点以外，
上面还有 n 个整数点，需要在上面再放 k 个新的点，
使得相邻的两个点之间的最大距离最小，求这个最小的距离。
*/
//二分法
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int L, n, k;
    scanf("%d%d%d", &L, &n, &k);
    vector<int> a(n + 2, 0); //学习，构造向量数组
    a[0] = 1;
    for (int i = 1; i < n; i++)
    {
        scanf("%d", &a[i]);
    }
    a[n + 1] = L;         //数组最后一个是长度L，第一个为0
    int l = 1, r = L - 1; //
    while (l < r)
    {
        int m = l + (r - l) / 2;
        int cnt = 0;
        for (int i = 1; i <= n + 1; i++)
        {
            cnt += (a[i] - a[i - 1] - 1) / m;
        }
        if (cnt > k)
            l = m + 1;
        else
            r = m;
    }
    cout << l << endl;
    return 0;
}