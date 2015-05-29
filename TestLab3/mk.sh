# clear the screen
printf "\ec"
echo -en "\ec"

# compile the assembly and c code
echo compiling
   as86 -o  s.o  ts.s
   bcc  -c -ansi  t.c
   bcc  -c -ansi  do_functions.c
   bcc  -c -ansi  kernal.c
   bcc  -c -ansi  misc_functions.c
   bcc  -c -ansi  queue.c
   
# link it all into an a.out file
echo linking
   ld86 -d s.o t.o do_functions.o kernal.o misc_functions.o queue.o mtxlib mylib /usr/lib/bcc/libc.a

# copy the a.out file into the os
echo cp a.out to mtximage/boot/mtx
   mount -o loop mtximage /mnt
   cp a.out /mnt/boot/mtx
   umount /mnt
echo done

# boot the os!
echo booting...
qemu-system-i386 -fda mtximage -no-fd-bootchk

