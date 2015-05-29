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
   return syscall(0,0,0);
}

int ps()
{
   return syscall(1, 0, 0);
}

int chname()
{
    char s[64];
    printf("input new name : ");
    gets(s);
    return syscall(2, s, 0);
}

int kfork()
{   
  int child, pid;
  pid = getpid();
  printf("proc %d enter kernel to kfork a child\n", pid); 
  child = syscall(3, 0, 0);
  printf("proc %d kforked a child %d\n", pid, child);
}    

int kswitch()
{
    return syscall(4,0,0);
}

int wait()
{
    int child, exitValue;
    printf("proc %d enter Kernel to wait for a child to die\n", getpid());
    child = syscall(5, &exitValue, 0);
    printf("proc %d back from wait, dead child=%d", getpid(), child);
    if (child>=0)
        printf("exitValue=%d", exitValue);
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

int getname()
{
    char x[64];
    int i = syscall(12, x, 0);
    
    printf("Proc Name: %s, size: %d", x, i);
    
    return i;
}

int casanova()
{
    int exitValue;
    printf("Enter a new segment (0-7): ");
    exitValue = geti();
    return syscall(20, exitValue, 0, 0);
}

#endif
