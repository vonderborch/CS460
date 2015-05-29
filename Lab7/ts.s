                BOOTSEG = 0x1000
                SSP     = 0
!               IMPORTS and EXPORTS
.globl _main,_prints                                 
.globl _tswitch,_running,_scheduler
.globl _int80h,_kcinth
.globl _getcs, _goUmode, _procSize
.globl _lock, _unlock, _int_off, _int_on
.globl _in_byte,_out_byte

!.globl _tinth, _thandler
!.globl _kbinth, _kbhandler
	
.globl _s0inth, _s0handler

start:
        mov     ax,cs                   ! establish segments 
        mov     ds,ax                   ! we know ES,CS=0x1000. Let DS=CS  
        mov     ss,ax                   ! SS = CS ===> all point to 0x1000
        mov     es,ax
        mov     sp,#SSP                 ! SP = 32KB

        call _main                      ! call main[] in C

! if ever return, just hang     
        mov   ax, #msg
        push  ax
        call  _prints
!_exit:
dead:   jmp   dead
msg:    .asciz "BACK TO ASSEMBLY AND HANG\n\r"    

_kreboot:
        jmpi 0,0xFFFF	
!*************************************************************
!     KCW  added functions for MT system
!************************************************************
_tswitch:
          cli
          push   ax
          push   bx
          push   cx
          push   dx
          push   bp
          push   si
          push   di
	  pushf
	  mov	 bx, _running
 	  mov	 2[bx], sp

find:     call	 _scheduler

resume:	  mov	 bx, _running
	  mov	 sp, 2[bx]
	  popf
	  pop    di
          pop    si
          pop    bp
          pop    dx
          pop    cx
          pop    bx
          pop    ax

          sti
          ret

_getcs: mov   ax, cs
        ret

USS =   4
USP =   6
INK =   8

! as86 macro: parameters are ?1 ?2, etc
         MACRO INTH
          push ax
          push bx
          push cx
          push dx
          push bp
          push si
          push di
          push es
          push ds

          push cs              ! we know CS=0x1000 
          pop  ds              ! let     DS = CS  to access Kernel data 
          
 	  mov bx,_running   	! ready to access proc
          inc INK[bx]
          cmp INK[bx],#1
          jg   ?1

          ! was in Umode: save interrupted (SS,SP) into proc
	  mov si,_running   	! ready to access proc
          mov USS[si],ss        ! save SS  in proc.USS
          mov USP[si],sp        ! save SP  in proc.USP

          ! Set kernel sp to proc[i].ksp 
          mov  di,ds            ! stupid !!        
          mov  es,di            ! CS=DS=SS=ES in Kmode
          mov  ss,di
          mov  sp,_running      ! sp -> running's kstack[] high end
          add  sp,_procSize

?1:
          call _?1              ! call handler in C

          br   _ireturn         ! upon return from C, to _iretutn
         MEND


! interrupt handlers; by MACRO INTH(label, handler, parameter)

_int80h: INTH kcinth

!_tinth:  INTH thandler
!_kbinth: INTH kbhandler

_s0inth: INTH (s0handler)

!*===========================================================================*
!*		_ireturn  and  goUmode()       				     *
!*===========================================================================*
! ustack contains    flag,ucs,upc, ax,bx,cx,dx,bp,si,di,es,ds
! uSS and uSP are in proc
_ireturn:                       ! return from INTERRUPTs
_goUmode:                       ! goYmode(): same as return from INTERRUPT
        cli
	mov bx,_running 	! bx -> proc
        dec INK[bx]
        cmp INK[bx],#0
        jg  xkmode

! return to Umode, restore ustack (uSS, uSP)m then pop ustack
	mov si,_running 	! si -> proc
        mov ax,USS[si]
        mov ss,ax               ! restore SS
        mov sp,USP[si]          ! restore SP
xkmode:  
	pop ds
	pop es
	pop di
        pop si
        pop bp
        pop dx
        pop cx
        pop bx
        pop ax 
        iret


_int_off:             ! cli, return old flag register
        pushf
        cli
        pop ax
        ret

_int_on:              ! int_on(int SR)
        push bp
        mov  bp,sp
        mov  ax,4[bp] ! get SR passed in
        push ax
        popf
        pop  bp
        ret

!*===========================================================================*
!*				lock					     *
!*===========================================================================*
_lock:  
	cli			! disable interrupts
	ret			! return to caller

!*===========================================================================*
!*				unlock					     *
!*===========================================================================*
_unlock:
	sti			! enable interrupts
	ret			! return to caller

!*===========================================================================*
!*				in_byte					     *
!*===========================================================================*
! PUBLIC unsigned in_byte[port_t port];
! Read an [unsigned] byte from the i/o port  port  and return it.

_in_byte:
        push    bp
        mov     bp,sp
        mov     dx,4[bp]
	inb     al,dx		! input 1 byte
	subb	ah,ah		! unsign extend
        pop     bp
        ret

!*===========================================================================*
!*				out_byte				     *
!*==============================================================
! out_byte[port_t port, int value];
! Write  value  [cast to a byte]  to the I/O port  port.

_out_byte:
        push    bp
        mov     bp,sp
        mov     dx,4[bp]
        mov     ax,6[bp]
	outb	dx,al   	! output 1 byte
        pop     bp
        ret
