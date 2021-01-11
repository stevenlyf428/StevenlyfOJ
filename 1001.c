#include <stdio.h>
// int main()
// {
//     // int a = 1;
//     // int b = a + 1;
//     // printf("111\n");
//     // printf("222\n");
//     // return 0;
//     int i;
//     for (i = 0; i < 3; i++)
//     {
//         printf("%d", i);
//     }
//     printf("%d", i);
//     return 0;
// }
double *weight(double m, double r)
{
    static double s;
    // s = m * r;
    return &s;
}
int main()
{
    double *s1, *s2;
    s1 = weight(80.0, 0.4);
    s2 = weight(90.0, 0.6);
    printf("%.1f\n", (*s1) + (*s2));
    return 0;
}
