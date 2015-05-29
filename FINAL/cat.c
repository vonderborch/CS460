#ifndef CAT_C
#define CAT_C

#include "ucode.c"
#include "vontypes.h" // draws from ucode.c here

int main(int argc, char *argv[])
{
    char c, lc = 0;
    int fd;
    
    //// OPEN FD
    // CASE 1: read from stdin
    if (argc == 1)
    {
        fd = null;
    }
    // CASE 2: read from file (or files)
    else
    {
        fd = open(argv[1], O_RDONLY);
    }
    
    //// FD CHECK
    if (fd < 0)
    {
        printf("Cat couldn't find the asked for file.\n");
        return -1;
    }
    
    // read the input!
    while (read(fd, &c, 1) > 0)
    {
        putc(c);
        
        // if we're reading a file, we look for new line chars
        if (fd == 0)
        {
            if (c == '\n' && (lc != '\n' && lc != '\r'))
                putc('\r');
        }
        // if we're reading from stdin, we look for return chars
        else
        {
            if (c == '\r' && (lc != '\n' && lc != '\r'))
            {
                putc('\n');
                putc('\r');
            }
        }
        
        lc = c;
    }

    // cat is complete!
    printf("\n");
    close(fd);
    exit(SUCCESS);
}

#endif
