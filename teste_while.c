#include "stdio.h"
int main(int argc, char const *argv[])
{
    int a[3];
    a[0] = 9;
    a[1] = 12;
    a[2] = 10;
    for (size_t i = 0; a[i] != 10; i++)
    {
        printf("oi\n");
    }

    return 0;
}
