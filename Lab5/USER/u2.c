#ifndef U2_C
#define U2_C

#include "utypes.h"

main()
{ 
    char name[64];
    int pid, cmd, out;
    printf("in u2.c\n");

    while(1)
    {
        pid = getpid();

        color = 0x0C;

        printf("----------------------------------------------\n");
        printf("I am proc %d in U2 mode: running segment=%x\n", getpid(), getcs());
        show_menu();
        printf("Your Order ? ");
        gets(name); 
        if (name[0]==0) 
            continue;

        cmd = find_cmd(name);

        if (cmd >= 0)
        {
            out = commands[cmd].f();
        }
        else
        {
            invalid(name);
        }
    }
}

#endif
