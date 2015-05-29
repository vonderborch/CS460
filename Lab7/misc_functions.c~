#ifndef MISCFUNCTIONS_C
#define MISCFUNCTIONS_C

#include "type.h"

// the body function of a proc
int body()
{
    int i, out;
    char c;
    printf("Proc[%d] resuming to body()\n", running->pid);
    running->status = RUNNING;
    while(1)
    {
        printf("============================================\n");
        printList("freelist  ", freeList);
        printList("readyQueue", readyQueue);
        printList("sleepList ", sleepList);
        printf("============================================\n");

        printf("\nproc %d[%d] running: priority=%d\n",
        running->pid, running->ppid,running->priority);

        shorthelp();
        c = getc(); printf("%c\n", c);
        
        for (i = 0; commands[i].key; i++)
        {
            if (c == commands[i].key)
            {
                printf("Executing command %s...\n", commands[i].name);
                out = commands[i].f();
                printf("\nCommand finished!\n\n");
            }
        }
    }
}

// prints the help screen
int help()
{
    int i;
    printf("\nAvailable Commands: \n");
    for (i = 0; commands[i].key; i++)
    {
        printf(" - %c: %s: %s\n", commands[i].key, commands[i].name, commands[i].help);
    }
    return 1;
}

// prints the list of commands
int shorthelp()
{
    int i;
    printf("Enter a char [");
    
    for (i = 0; commands[i].key; i++)
    {
        printf(" %c ", commands[i].key);
        if (commands[i + 1].key != NULL)
        {
            printf("|");
        }
    }
    
    printf("] : ");
    return 1;
}

// gets a string
int get_str(char *str, char *buf, int size)
{
    int i = 0;
    
    if (str == 0 || buf == 0 || size < 0 || size > 1024)
        return -1;
    
    while (size == 0 || i < size) {
        *(buf + i) = get_byte(running->uss, str + i);
        if (*(buf + i) == '\0') {
            break;
        }
        i++;
    }
    
    return(i);
}

// initializes the OS!
int init()
{
    PROC *p;
    int i, j;
    
    color = 0x0C;
    printf("init ....\n");
    for (i=0; i<NPROC; i++){   // initialize all procs
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->priority = 0;  
        strcpy(proc[i].name, pname[i]);
	    printf("proc[%d]=%s.\n", i, proc[i].name);
        p->next = &proc[i+1];
        
        // setup the file descripters
        for (j = 0; j < NFD; j++)
        {
            p->fd[j] = 0;
        }
    }
    
    freeList = &proc[0];      // all procs are in freeList
    proc[NPROC-1].next = 0;
    readyQueue = sleepList = 0;
    
    // setup pipe stuff
    for (i = 0; i < NOFT; i++)
    {
        oft[i].refCount = 0;
    }
    for (i = 0; i < NPIPE; i++)
    {
        pipe[i].busy = 0;
    }

    /**** create P0 as running ******/
    p = get_proc(&freeList);
    p->status = RUNNING;
    p->ppid   = 0;
    p->parent = p;
    running = p;
    nproc = 1;
    printf("done\n");
}

#endif
