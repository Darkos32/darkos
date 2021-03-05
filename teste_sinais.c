#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "signal.h"
#include "stdlib.h"
#include "errno.h"
#include "sys/wait.h"
#include "unistd.h"
int shouldKill = 0;

void SIGINT_handler(int sig)
{
    kill(getpid() + 1, SIGSTOP);
}
int main(int argc, char const *argv[])
{
    pid_t pid;
    signal(SIGTSTP, SIGINT_handler);
    if ((pid =fork()) == 0)
    {
       
        int n = 0;
        while (1)
        {
            printf("%d\n", n);
            n++;
            
            sleep(2);
        }
    }
    else
    {
        if (shouldKill)
        {
            kill(pid, SIGKILL);
        }

        waitpid(pid, NULL, WUNTRACED);
        printf("fim");
        //kill(pid, SIGCONT);
        ;
    }
}
