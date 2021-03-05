#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"
int main(int argc, char const *argv[])
{
    pid_t pid;
    int status;
    if ((pid = fork()) == 0)
    {
        sleep(30);
    }
    else
    {
        sleep(100);
        pid_t pid2 = waitpid(pid, &status, WNOHANG);
        
        if (WIFSTOPPED(status))
        {
            printf("S");
        }
        waitpid(pid, &status, WUNTRACED);
        if (WIFCONTINUED(status))
        {
            puts("C");
        }

        printf("%d", pid2);
    }

    return 0;
}
