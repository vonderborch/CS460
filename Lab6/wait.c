#ifndef WAIT_C
#define WAIT_C

#include "type.h"

// wait for child procs to die
int kwait(int *status)
{
	int i;
	
    // first lets figure out if the process has any children...
	for (i = 0; i < NPROC; i++)
	{
		if (proc[i].ppid == running->pid && proc[i].status != FREE && proc[i].status != ZOMBIE)
			break;
	}
	if (i == NPROC)
	{
	    // if it has no children, stop before we start
		printf("The process has no children to take care of it!\n");
		return -1;
	}
	
	while (1)
	{
	    // find a zombie!
		for (i = 0; i < NPROC; i++)
		{
            // if a zombie exists, stop waiting!
			if (proc[i].status == ZOMBIE && proc[i].ppid == running->pid)
			{
				*status = proc[i].exitCode;
				proc[i].status = FREE;
				enqueue(&freeList, &proc[i]);
				return proc[i].pid;
			}
		}
		// sleep!
		ksleep(running);
	}
}

// sleep the running process and give it the event!
int ksleep(int event)
{
    // set running process event to event, set running to sleep,
    //     enqueue it onto the sleep list and then switch procs 
    //     to a running one.
    running->event = event;
    running->status = SLEEP;
    enqueue(&sleepList, running);
    
    proc[running->ppid].status = READY;
    
    tswitch();
    return 1;
}

// wakeup a process!
int kwakeup(int event)
{
	int i, counter = 0;
	PROC *p = sleepList;
	
	// CASE 1: no sleep list...ergo, no processes to wake up!
	if (p == 0)
	{
		printf("There are no sleeping procs to wakeup.\n");
		return -1;
	}
    // CASE 2: there is a sleep list. go through each process in the list and wake up ones with the same event
	else while(p)
	{
		if (p->event == event)
		{
			p = dequeue(&sleepList);
			p->status = READY;
			enqueue(&readyQueue, p);
			counter++;
		}
		else
		{
			p = p->next;
		}
	}
	return counter;
}

// murder teh running process!
int kexit(int exitvalue)
{
	int i;
	bool good = true;
	
	// Are we trying to kill P1?
	if(running->pid == proc[1].pid)
	{
	    for (i = 2; i < NPROC; i++)
	    {
	        if (proc[i].status != FREE)
	        {
	            good = false;
	            break;
	        }
	    }
	
	    if (good == false)
	    {
	        printf("We can't kill P1 while there are still other processes!\n");
	        return -1;
	    }
	}
	
	// if we're gonna murder a process...
	running->exitCode = exitvalue;
	running->status = ZOMBIE;
	
	// adoption process
	for (i = 2; i < NPROC; i++)
	{
	    // is this proc one of the running procs children?
	    if (proc[i].ppid == running->pid)
	    {
	        proc[i].ppid = proc[1].pid; // let P1 adopt the proc!
	    }
	}
	
	// kill those descriptors...
	for (i = 0; i < NFD; i++)
	{
	    if (running->fd[i] != 0)
	    {
	        close_pipe(i);
	    }
	}
	
	// wake the proc's guardian and switch to it!
	i = kwakeup((int)&proc[running->ppid]);
	
	ktswitch();
}

#endif
