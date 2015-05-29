#ifndef TYPE_H
#define TYPE_H

//////////////////////////////////////// DEFINITIONS ////////////////////////////////////////

typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define MTXSEG 0x1000

#define NPROC    9
#define SSIZE 1024

#define NULL 0     // defines the value for null.

#define INPUTBUFFER 64

/******* PROC status ********/
#define FREE     0
#define READY    1
#define RUNNING  2
#define STOPPED  3
#define SLEEP    4
#define ZOMBIE   5

/******* Pipe stuffs ********/
#define READ_PIPE  4
#define WRITE_PIPE 5
#define NOFT       20
#define NFD        10
#define PSIZE      10
#define NPIPE      10

typedef struct Oft {
    int   mode;
    int   refCount;
    struct pipe *pipe_ptr;
} OFT;

typedef struct pipe {
    char  buf[PSIZE];
    int   head, tail, data, room;
    int   nreader, nwriter;
    int   busy;
}PIPE;

// PROC definition
typedef struct proc{
    struct proc *next;
    int    *ksp;               // at offset 2
    int    uss, usp;           // at offsets 4,6
    int    pid;                // add pid for identify the proc
    int    status;             // status = FREE|READY|RUNNING|SLEEP|ZOMBIE    
    int    ppid;               // parent pid
    struct proc *parent;
    int    priority;
    int    event;
    int    exitCode;
    char   name[32];
    
    OFT    *fd[NFD];
    
    int    kstack[SSIZE];      // per proc stack area
}PROC;

// Bool definition
typedef enum { false, true } bool;

// COMMAND TABLE
typedef struct command_table{
	char key;
	char *name;
	bool (*f)();
	char *help;
} COMMANDTABLE;

//////////////////////////////////////// VARIABLES ////////////////////////////////////////
PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc = 0;
int color;
char *pname[] = { "Odin", "Thor", "Freya",  "Baldur", "Tyr", "Loki", "Heimdall", "Frigg", "Njordur" };
PIPE pipe[NPIPE];
OFT oft[NOFT];
char *MODE[] = {"READ_PIPE ", "WRITE_PIPE"};

//////////////////////////////////////// FUNCTIONS ////////////////////////////////////////
////////// int.c //////////
int kcinth();
int kgetpid();
int kpd();
int kchname(char *name);
int kkfork();
int ktswitch();
int kkwait(int *status);
int kkexit(int value);
int kkmode();
int kexec(char* filename);

////////// kernal.c //////////
int kfork(char *filename);
bool kcopy(u16 parent, u16 child, u16 size);
u8 get_byte(u16 segment, u16 offset);
int get_word(u16 segment, u16 offset);
int put_byte(u8 byte, u16 segment, u16 offset);
int put_word(u16 word, u16 segment, u16 offset);

////////// t.c //////////
int init();
int scheduler();
int int80h();
int set_vector(u16 vector, u16 handler);

////////// wait.c //////////
int kwait(int *status);
int ksleep(int event);
int kwakeup(int event);
int kexit(int exitvalue);

////////// misc_functions.c //////////
int body();
int help();
int shorthelp();
int get_str(char *str, char *buf, int size);

////////// do_functions.c //////////
int do_kfork();
int do_kforkcustom();
int do_exit();
int do_wait();
int do_switch();
int do_umode();

////////// pipe.c //////////
int show_pipe(PIPE *ptr);
int pfd();
int read_pipe(int ifd, char *buf, int n);
int write_pipe(int ifd, char *buf, int n);
OFT *get_free_fd();
PIPE *get_free_pipe();
int kpipe(int pd[2]);
int close_pipe(int fd);

////////// helpers.c //////////
char* getstring(char *str);

//////////////////////////////////////// COMMAND TABLE ////////////////////////////////////////
COMMANDTABLE commands[] = {
	{ 's', "Switch", do_switch, "Switch to the next ready process." },
	{ 'f', "Fork", do_kfork, "Forks a new process from the free processes." },
	{ 'g', "Fork Custom", do_kforkcustom, "Forks a new process from the free processes with a custom image." },
	{ 'q', "Kill", do_exit, "Makes the running PROC die." },
	{ 'p', "PrintProcs", kpd, "Print pid, ppid, and status of all processes." },
	{ 'w', "Wait", do_wait, "Running proc to sleep on an event value." },
	{ 'u', "UMode", do_umode, "Goes into umode." },
	{ '?', "Help", help, "Brings up this help menu!" },
	{0, 0, 0, 0}
};

#endif
