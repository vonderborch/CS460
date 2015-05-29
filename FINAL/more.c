#ifndef MORE_C
#define MORE_C

#include "ucode.c"
#include "vontypes.h" // draws from ucode.c here

#define PAGEROWS 20
#define COLUMNS  80

int main(int argc, char *argv[])
{
    char c, tty[64];
    int fd;
    
    // get stdin
    if (argc == 1)
    {
        fd = dup(0);
        close(0);
        gettty(tty);
        open(tty, O_RDONLY);
    }
    // get fd
    else
    {
        fd = open(argv[1], O_RDONLY);
    }
    
    //// FD CHECK
    if (fd < 0)
    {
        printf("More couldn't find the asked for file.\n");
        return -1;
    }
    
    while (1)
    {
        Print(fd, PAGEROWS);
        
        while (1)
        {
            c = getc();
            
            switch (c)
            {
                // has the user pressed enter?
                case '\r':
                case '\n':
                    Print(fd, 1);
                    break;
                // has the user pressed space?
                case ' ':
                    Print(fd, PAGEROWS);
                    break;
                // has the user decided to quit while they're ahead?
                case 'q':
                    putc('\n');
                    return 0;
                    break;
            }
        }
    }

    return 0;
}

// prints some number of lines
bool Print(int fd, int lines)
{
    int i, j, size;
    char c;
    
    for (i = 0; i < lines; i++)
    {
        for (j = 0; j < COLUMNS; j++)
        {
            size = read(fd, &c, 1);
            putc(c);
            
            if (size < 1)
            {
                exit(0);
            }
            
            if (c == '\n' || c == '\r')
                break;
        }
    }
    
    putc('\n');
}

#endif
