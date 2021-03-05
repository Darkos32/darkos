#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"
int main(int argc, char const *argv[])
{
    
    if (fork() == 0)
    {
        printf("Ok");
        execl("/bin/ls", "ls", NULL);
    }
    else
    {
        printf("Ko");
    }

    return 0;
}
