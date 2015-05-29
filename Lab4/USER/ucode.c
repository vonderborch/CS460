#ifndef UCODE_C
#define UCODE_C

#include "utypes.h"

int show_menu()
{
    int i;
    printf("***************** Menu *******************\n");

    for (i = 0; commands[i].command; i++)
    {
        printf("  %s  ", commands[i].command);
    }

    printf("\n******************************************\n");
    
    return 1;
}

int help()
{
    int i;
    
    printf("\nAvailable Commands: \n");
    for (i = 0; commands[i].command; i++)
    {
        printf(" - %c: %s: %s\n", commands[i].command, commands[i].name, commands[i].help);
    }
    
    return 1;
}

int find_cmd(char *name)
{
    int i;

    for (i = 0; commands[i].command; i++)
    {
        if (!strcmp(commands[i].command, name))
            return i;
    }
    
    return -1;
}

int getpid()
{
    printf("Entering kernal to get pid...\n");
    return syscall(0,0,0);
}

int ps()
{
    printf("Entering kernal to ps...\n");
    return syscall(1, 0, 0);
}

int chname()
{
    char input[INPUTBUFFER];
    printf("Input new name : ");
    gets(input);
    return syscall(2, input, 0);
}

int kfork()
{   
  int child, pid;
  pid = getpid();
  printf("Proc %d entering kernal to fork a child...\n", pid); 
  child = syscall(3, 0, 0);
  printf("Proc %d forked a child (%d)!\n", pid, child);
}

int kswitch()
{
    printf("Entering kernal to switch procs...\n");
    return syscall(4,0,0);
}

int wait()
{
    int child, exitCode;
    printf("proc %d enter Kernel to wait for a child to die\n", getpid());
    child = syscall(5, &exitCode, 0);
    printf("proc %d back from wait, dead child=%d", getpid(), child);
    if (child>=0)
        printf("exitValue=%d", exitCode);
    printf("\n"); 
} 

int geti()
{
  char s[16];
  return atoi(gets(s));
}

int exit()
{
   int exitValue;
   printf("enter an exitValue: ");
   exitValue = geti();
   printf("exitvalue=%d\n", exitValue);
   printf("enter kernel to die with exitValue=%d\n", exitValue);
   _exit(exitValue);
}

int _exit(int exitValue)
{
  return syscall(6,exitValue,0);
}


int getc()
{
  return syscall(90,0,0) & 0x7F;
}

int putc(char c)
{
  return syscall(91,c,0,0);
}

int invalid(char *name)
{
    printf("Invalid command : %s\n", name);
}

#endif
