#ifndef INIT_C
#define INIT_C

#include "ucode.c"
#include "vontypes.h" // draws from ucode.c here

int pid, child, status;
int stdin, stdout;

// main program logic
main(int argc, char *argv[])
{
    // open /dev/tty0 as 0 (READ) and 1 (WRTIE) in order to display messages
    stdin = open("/dev/tty0", READ);
    stdout = open("/dev/tty0", WRITE);

    // Now we can use printf, which calls putc(), which writes to stdout
    printf("VONINIT : fork a login task on console\n"); 
    child = fork();

    if (!child)
        Login();
        
    Parent();
}

// execute login code!
int Login()
{
    exec("login /dev/tty0");
}
      
// execute waiting code!
int Parent()
{
    while(1)
    {
        printf("VONINIT : waiting .....\n");

        pid = wait(&status);

        if (pid == child)
        {
            printf("VONINIT : buried an orphan child %d, forking another child ");
            child = fork();
            printf("%d...\n", child);
            if (!child)
                Login();
        }
        else
        {
            // bury a child :(
            printf("VONINIT : buried an orphan child %d :(\n");
        }
    }
}

#endif
