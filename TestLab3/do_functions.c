#ifndef DOFUNCTIONS_C
#define DOFUNCTIONS_C

#include "types.h"

// switch it!
bool do_tswitch()
{
    running->status = READY;
    tswitch();
    return true;
}

// fork it!
bool do_kfork()
{
    PROC *p;
    p = kfork();
    printf("Forked new process [%d, %d]!", p->pid, p->ppid);
    return true;
}

// kill it!
bool do_exit()
{
    int out;
    char buf[INPUTBUFFER];
    printf("Exit Event Value? ");
    rgets(buf);
    out = kexit(atoi(buf));
    if (out == -1)
    {
        printf("\nThe running process has become a zombie!");
    }
    else
    {
        printf("\nP%d can't be killed while it has active children. At least P%d is active!\n", DEFAULTPROCESS, out);
    }
    return true;
}

// print it!
bool do_ps()
{
    return kps();
}

// sleep it!
bool do_sleep()
{
    char buf[INPUTBUFFER];
    printf("Sleep Event Value? ");
    rgets(buf);
    ksleep(atoi(buf));
    printf("\nThe running process has been put to sleep waiting for event %d!", atoi(buf));
    return true;
}

// wake them!
bool do_wakeup()
{
    int out;
    char buf[INPUTBUFFER];
    printf("Wakeup Event Value? ");
    rgets(buf);
    out = kwakeup(atoi(buf));
    if (out == -1)
    {
        printf("\nNo processes to wake up!");
    }
    else
    {
        printf("\n%d processes have been woken up!", out);
    }
    return true;
}

// wait it!
bool do_wait()
{
    int pid, status;
    pid = kwait(&status);
    if (pid == -1)
    {
        printf("\nProcess has no children to allow it to wait!");
    }
    else
    {
        printf("\npid=%d, status=%d\n", pid, status);
    }
    return true;
}

// help it!
bool do_help()
{
    return help();
}

// resurrect them!
bool do_resurrect()
{
    int out;
    out = kresurrect();
    printf("\nResurrected %d processes!\n", out);
    return true;
}

// free it!
bool do_free()
{
    int out;
    char buf[INPUTBUFFER];
    printf("Exit Event Value? ");
    rgets(buf);
    out = kfree(atoi(buf));
    if (out == -1)
    {
        printf("\nThe running process has become free!");
    }
    else
    {
        printf("\nP%d can't be freed while it has active children. At least P%d is active!\n", DEFAULTPROCESS, out);
    }
}

// free all procs!
bool do_quitprogram()
{
    int i;
    PROC *p;
    // clear the ready queue
    while (readyQueue->pid != 0)
        dequeue(&readyQueue);
    // clear the sleep queue
    while (sleepList)
        dequeue(&sleepList);
    
    // plop everything onto the free list (lets at least do this cleanly)
    for (i = 1; i < NPROC; i++)
    {
        p = &proc[i];
        p->status = FREE;
        put_proc(p);
    }
    // switch to P0
    tswitch();
}

#endif
