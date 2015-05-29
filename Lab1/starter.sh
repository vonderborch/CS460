#!/bin/bash

as86 -o bs.o  bs.s
bcc -c -ansi  t.c
ld86 -d bs.o t.o /usr/lib/bcc/libc.a

FILENAME=a.out
FILESIZE=$(stat -c%s "$FILENAME")
echo "Size of $FILENAME = $FILESIZE bytes."

if [[ FILESIZE -lt 1024 ]]; then

    echo "File fits in bootspace, do you want to boot with it (Y = yes)?"
    
    read yesno
    
    if [[ "$yesno" == "Y" ]]; then
        dd if=a.out of=mtximage bs=1024 count=1 conv=notrunc
        qemu -fda mtximage -no-fd-bootchk
    fi
else
    echo "a.out is too big to replace the booter!"
fi
