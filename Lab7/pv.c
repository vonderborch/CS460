#ifndef PV_C
#define PV_C

#include "type.h"

int P(struct semaphore *s)
{
    // write YOUR C code for P()
    int dasvalue = int_off();
    
    s->value--; // decrement semaphore value
    
    // if the value is less than 0, add the running process to the waiting queue
    if (s->value < 0)
    {
        running->status = BLOCK;
        enqueue(&s->queue, running);
        tswitch();
    }
    
    int_on(dasvalue);
}

int V(struct semaphore *s)
{
    // write YOUR C code for V()
    PROC *p;
    int dasvalue = int_off();
    
    s->value++; // increment semaphore value
    
    // if the value is less than or equal to 0, remove a process from the ready queue and wake it
    if (s->value <= 0)
    {
        p = dequeue(&s->queue);
        p->status = READY;
        enqueue(&readyQueue, p);
    }
    
    int_on(dasvalue);
}

#endif

