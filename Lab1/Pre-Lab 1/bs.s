!==================== bs.s file ===================================
        .globl _main             ! IMPORT symbols from C code
        .globl _getc,_putc       ! EXPORT symbols to C code

!--------------------------------------------------------------------
! Only one SECTOR loaded at (0000,7C00). Load entire block to 0x90000
!--------------------------------------------------------------------
        mov  ax,#0x9000    ! set ES to 0x9000
        mov  es,ax

        xor  bx,bx          ! clear BX = 0
!--------------------------------------------------------------------
!  Call BIOS INT-13 to read BOOT BLOCK to (segment,offset)=(0x9000,0)
!--------------------------------------------------------------------
        xor  dx,dx          ! DH=head=0,   DL=drive=0
        xor  cx,cx          ! CL=cylinder, CL=sector
        incb cl             ! BIOS counts sector from 1
        mov  ax, #0x0202    ! AH=READ      AL=2 sectors
        int  0x13           ! call BIOS INT-13

        jmpi start,0x9000   ! CS=0x9000, IP=start

start:                    
        mov  ax,cs          ! Set segment registers for CPU
        mov  ds,ax          ! we know ES,CS=0x9000. Let DS=CS  
        mov  ss,ax          ! SS = CS ===> all point at 0x9000
        mov  es,ax
        mov  sp,#8192       ! SP = 8192 above SS=0x9000

!--------------- OPTIONAL --------------------------------------
        mov  ax,#0x0012     ! Call BIOS for 640x480 color mode     
	int  0x10           ! 
!--------------------------------------------------------------	
        call _main          ! call main() in C

        jmpi 0,0x1000
 

!======================== I/O functions =================================
		
!---------------------------------------------
!  char getc()   function: returns a char
!---------------------------------------------
_getc:
        xorb   ah,ah           ! clear ah
        int    0x16            ! call BIOS to get a char in AX
        ret 

!----------------------------------------------
! void putc(char c)  function: print a char
!----------------------------------------------
_putc:           
        push   bp
	mov    bp,sp
	
        movb   al,4[bp]        ! get the char into aL
        movb   ah,#14          ! aH = 14
        movb   bl,#0x0D        ! bL = color 
        int    0x10            ! call BIOS to display the char

        pop    bp
	ret
