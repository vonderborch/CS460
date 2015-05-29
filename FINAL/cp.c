#ifndef CP_C
#define CP_C

#include "ucode.c"
#include "vontypes.h"

int main(int argc, char *argv[])
{
    int src, dest, i;
    char buffer[256];
    
    if (argc < 3)
    {
        printf("cp src dest");
        return 1;
    }
    
    // open files
    src = open(argv[1], O_RDONLY);
    dest = open(argv[2], O_WRONLY | O_CREAT);
    
    // read from source, write to dest
    while ((i = read(src, buffer, 256)) > 0)
        write(dest, buffer, i);
    
    // close files
    close(src);
    close(dest);
    return 0;
}

#endif
