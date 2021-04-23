#include "header.h"

void handler(int signum)
{
    printf("\nПринудительное завершение терминала\n");
    chik_exit(NULL);
} 

int main(int argc, char *argv[])
{
    printf(LINE "\t\tTERMINALCHIK\n" LINE);
    struct sigaction sig;
 //   sig.sa_flags = SA_RESTART;
    sig.sa_handler = &handler;

    sigaction(SIGINT, &sig, NULL);
    //sigaction(SIGINT,);
    while(1)
    {
        shell_chik();
        //printf("Hello world\n");
    }
    return 0;
}