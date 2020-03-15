
#include <stdio.h>

void func2()
{
    printf("C语言小白变怪兽");
}

void func1()
{
    printf("http://c.biancheng.net");
    func2();
}

int main()
{
    func1();
    return 0;
}