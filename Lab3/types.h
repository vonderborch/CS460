#ifndef TYPES_H
#define TYPES_H

//////////////////////////////////////// DEFINITIONS ////////////////////////////////////////

#define NPROC    9 // the number of processes the system can handle...
#define SSIZE 1024 // the size of the proc stack

#define NULL 0     // defines the value for null.
#define null 0     // defines the value for null (alt-form).

#define INPUTBUFFER 64

#define DEFAULTPROCESS 1

/******* PROC statuses ********/
#define FREE     0
#define READY    1
#define RUNNING  2
#define STOPPED  3
#define SLEEP    4
#define ZOMBIE   5

// PROC DEFINITION
typedef struct proc{
    struct proc *next;
    int    *ksp;
    int    pid;                // add pid for identify the proc
    int    status;             // status = FREE|READY|RUNNING|SLEEP|ZOMBIE    
    int    ppid;               // parent pid
  struct proc *parent;
    int    priority;
    int    event;
    int    exitCode;
    int    kstack[SSIZE];      // per proc stack area
}PROC;

// BOOL DEFINITION
typedef enum { false, true } bool;

// COMMAND TABLE
typedef struct command_table{
	char key;
	char *name;
	bool (*f)();
	char *help;
} COMMANDTABLE;

//////////////////////////////////////// VARIABLES ////////////////////////////////////////
////////// Global Variables  //////////
PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc = 0;
int color;

//////////////////////////////////////// FUNCTIONS ////////////////////////////////////////
////////// kernal.c //////////
PROC *kfork();
bool ksleep(int event);
int kwakeup(int event);
int kexit(int exitvalue);
int kwait(int *status);
bool kzombie();
bool kps();
int kfree(int exitvalue);
int kresurrect();

////////// queue.c //////////
int scheduler();
PROC *get_proc();
int put_proc(PROC *p);
int enqueue(PROC **queue, PROC *p);
PROC *dequeue(PROC **queue);
int printQueue(PROC *queue);

////////// do_functions.c //////////
bool do_tswitch();
bool do_kfork();
bool do_exit();
bool do_ps(); 
bool do_sleep();
bool do_wakeup();
bool do_wait();
bool do_help();
bool do_resurrect();
bool do_free();
bool do_quitprogram();

////////// misc_functions.c //////////
bool help();
bool shorthelp();
int init();
int body();

//////////////////////////////////////// COMMAND TABLE ////////////////////////////////////////
COMMANDTABLE commands[] = {
	{ 's', "Switch", do_tswitch, "Switch to the next ready process." },
	{ 'f', "Fork", do_kfork, "Forks a new process from the free processes." },
	{ 'q', "Kill", do_exit, "Makes the running PROC die." },
	{ 'p', "PrintProcs", do_ps, "Print pid, ppid, and status of all processes." },
	{ 'z', "Sleep", do_sleep, "Running proc to sleep on an event value." },
	{ 'a', "Wake", do_wakeup, "Wakeup all procs sleeping on event." },
	{ 'w', "Wait", do_wait, "Wait for a zombie child." },
	{ 'r', "Free", do_free, "Frees the currently running process." },
	{ 'u', "Resurrect", do_resurrect, "Puts all zombie processes on the free list." },
	{ 'e', "Quit", do_quitprogram, "Quits the program by killing all running and ready processes." },
	{ '?', "Help", do_help, "Brings up this help menu!" },
	{0, 0, 0, 0}
};

#endif