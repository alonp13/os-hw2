#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>
#define READ 0
#define WRITE 1
#define MAX_COUNTER 5

static int fd[2];
static int parent_pid;
static int child_pid;
static int recieved;


void parent_handler(int sig);
void child_handler(int sig);

int main()
{
    

    pipe(fd);

    //printf("%d\n",getpid());
    int pid = fork();
    //printf("%d\n",getpid());

    if(pid < 0)
    {
        printf("Error\n");
        exit(0);
    } 

    parent_pid = getppid();
    child_pid = pid;


    if(pid == 0)
    {
        if(signal(SIGUSR1,child_handler) == SIG_ERR)
        {
            printf("Error\n");
        }

        int start = 0;
        write(fd[WRITE],&start,sizeof(start));
        kill(parent_pid,SIGUSR1);

        while(recieved != MAX_COUNTER)
        {

            sleep(1);
            kill(parent_pid,SIGUSR1);
        }

    } 
    else 
    {
        signal(SIGUSR1,parent_handler);

        while(recieved != MAX_COUNTER)
        {
            sleep(1);
        }
        int status = 0;
        wait(&status);
       // sleep(15);

        printf("Parent terminated\n");
    }

    return 0;
}


void parent_handler(int sig)
{

    read(fd[READ],&recieved,sizeof(recieved));
    printf("%d\n",recieved);
    if(recieved >= MAX_COUNTER)
    {
        close(fd[READ]);
        close(fd[WRITE]);
        return;
    }

    recieved++;

    write(fd[WRITE],&recieved,sizeof(recieved));
    kill(child_pid,SIGUSR1);
}

void child_handler(int sig)
{
    read(fd[READ],&recieved,sizeof(recieved));
    printf("%d\n",recieved);
    if(recieved >= MAX_COUNTER)
    {
       // sleep(15);
        printf("Child terminated\n");
        exit(0);
    }

    recieved++;

    write(fd[WRITE],&recieved,sizeof(recieved));
    kill(parent_pid,SIGUSR1);
}