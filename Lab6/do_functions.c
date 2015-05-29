#ifndef DOFUNCTIONS_C
#define DOFUNCTIONS_C

#include "type.h"

// fork it!
int do_kfork()
{
    kfork();
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
    ktswitch();
}

// umode it!
int do_umode()
{
    goUmode();
}

#endif
