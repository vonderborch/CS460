OSSEG  = 0x1000

.globl _main,_running,_scheduler
.globl _proc, _procSize
.globl _tswitch,_resume

jmpi   start,OSSEG

start:	mov  ax,cs
	mov  ds,ax
	mov  ss,ax
        mov  es,ax
	mov  sp,#_proc
	add  sp,_procSize
		
	call _main
	
_tswitch:
SAVE:	
	push bp
	pushf
	push ds
	push ss
	push es
	
	mov  bx,_running
	mov  2[bx],sp

FIND:	call _scheduler

_resume:	
RESUME:
    mov bx,_running
    mov sp,2[bx]
    pop es
    pop ss
    pop ds
    popf
    pop bp
    ret

