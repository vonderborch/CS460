#ifndef KERNEL_C
#define KERNEL_C

#include "type.h"

// function to create a process DYNAMICALLY
int kfork(char *filename)
{
    // Variables
    int i;
    u16 segment;
    
    // Get the new proc
    PROC *p = get_proc(&freeList);
    if (p == 0)
    {
        // if there were no procs, report kfork's failure
        printf("No available free procs!\n");
        return 0;
    }
    
    // initialize the proc...
    p->status = READY;
    p->priority = 1;
    p->ppid = running->pid;
    p->parent = running;
    for (i = 1; i < 10; i++)
        p->kstack[SSIZE - i] = 0;
    
    p->kstack[SSIZE - 1] = (int)do_umode;
    p->ksp = &(p->kstack[SSIZE - 9]);
    
    segment = (p->pid + 1) * 0x1000;
    
    if (filename)
    {
        load(filename, segment);
        for (i = 0; i < 13; i++)
            put_word(0, segment, segment - i * 2);
        put_word(0x0200, segment, segment - 2);
    }
    else
        kcopy(running->uss, segment, 32 * 1024);
        
    put_word(segment, segment, segment - 4);
    put_word(segment, segment, segment - 22);
    put_word(segment, segment, segment - 24);
    
    // copy file descriptors
    for (i = 0; i < NFD; i++)
    {
	    p->fd[i] = running->fd[i];
	    if (p->fd[i] != 0)
	    {
	        p->fd[i]->refCount++;
	        if (p->fd[i]->mode == READ_PIPE)
	        {
		        p->fd[i]->pipe_ptr->nreader++;
	        }
	        if (p->fd[i]->mode == WRITE_PIPE)
	        {
		        p->fd[i]->pipe_ptr->nwriter++;
	        }
	    }
    }
    
    p->uss = segment;
    p->usp = segment - 24;
    
    enqueue(&readyQueue, p);
    
    printf("Proc[%d] forked Proc[%d] at segment 0x%x!\n", running->pid, p->pid, segment);
    return p->pid;
}

bool kcopy(u16 parent, u16 child, u16 size)
{
    int i;
    
    for (i = 0; i < size; i += 2)
        put_word(get_word(parent, i), child, i);
    
    return true;
}

u8 get_byte(u16 segment, u16 offset){
 	 u8 byte;
 	 setds(segment);
 	 byte = *(u8 *)offset;
 	 setds(MTXSEG);
 	 return byte;
}

int get_word(u16 segment, u16 offset){
	u16 word;
	setds(segment);
	word = *(u16 *)offset;
	setds(MTXSEG);
	return word;
}

int put_byte(u8 byte, u16 segment, u16 offset){
  	setds(segment);
  	*(u8 *)offset = byte;
  	setds(MTXSEG);
}

int put_word(u16 word, u16 segment, u16 offset){
	setds(segment);
	*(u16 *)offset = word;
	setds(MTXSEG);
}

#endif
