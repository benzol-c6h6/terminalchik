#include "header.h"

int main(int argc, char *argv[])
{
    printf(LINE "\t\tTERMINALCHIK\n" LINE);
    
    //sig.sa_handler = &kill_parent;

    sigaction(SIGINT, &sig, NULL);

    while(1)
    {
        shell_chik();
        //printf("Hello world\n");
    }
    return 0;
}