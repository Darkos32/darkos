#include "stdio.h"
#include "unistd.h"
int main(int argc, char const *argv[])
{
    sleep(30);
    printf("%s", argv[1]);
    return 0;
}
