#ifndef DOFUNCTIONS_C
#define DOFUNCTIONS_C

#include "type.h"

// fork it!
int do_kfork()
{
    kfork("/bin/u1");
}

// custom fork it!
int do_kforkcustom()
{
    char filename[INPUTBUFFER];
    printf("Filename? ");
    gets(filename);
    kfork(filename);
}

// exit it!
int do_exit()
{
	char exitval[INPUTBUFFER];
	printf("Exit value? ");
	gets(exitval);
	kexit(atoi(exitval));
}

// wait it!
int do_wait()
{
	int pid, status;
	pid = kwait(&status);
	printf("pid = %d, exitVal = %d\n", pid, status);
}

// switch it!
int do_switch()
{
    tswitch();
}

// umode it!
int do_umode()
{
    goUmode();
}

// hop it!
int do_hop()
{
    char segment[INPUTBUFFER];
    printf("New Segment Number (0-7)? ");
    gets(segment);
    return hop(atoi(segment));
}

#endif
