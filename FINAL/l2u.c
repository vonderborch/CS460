#ifndef LOWERTOUPPER_C
#define LOWERTOUPPER_C

#include "ucode.c"
#include "vontypes.h"

int main(int argc, char *argv[])
{
    char buffer[1024], newbuffer[1024];
    int length = 0, fd1, fd2, i;
    
    // CASE 1: no files
    if (argc == 1)
    {
        fd1 = 0;
        fd2 = 1;
    }
    // CASE 2: file-in only
    else if (argc == 2)
    {
        fd1 = open(argv[1], O_RDONLY);
        fd2 = 1;
    }
    // CASE 3: both files
    else if (argc == 3)
    {
        fd1 = open(argv[1], O_RDONLY);
        fd2 = open(argv[2], O_WRONLY | O_CREAT);
    }
    
    // FD Check
    if (fd1 < 0 || fd2 < 0)
    {
        printf("l2u can't read file(s)!\n");
        return 0;
    }
    
    do
    {
        length = read(fd1, buffer, 1024);
        
        for (i = 0; i < length; i++)
        {
            if (IsLowercaseLetter(buffer[i]) == true)
                newbuffer[i] = (buffer[i] - 32);
            else
                newbuffer[i] = buffer[i];
        }
        
        if (fd2 != 1)
            write(fd2, newbuffer, length);
        else
        {
            for (i = 0; i < length; i++)
            {
                if (newbuffer[i] == '\n')
                    printf("\n");
                else
                    printf("%c", newbuffer[i]);
            }
        }
        
    } while (length == 1024);
    
    // cleanup!
    close(fd1);
    close(fd2);
    return 0;
}

// returns true if the passed character is a lowercase letter.
bool IsLowercaseLetter(char c)
{
    int i;
    char *letters = "abcdefghijklmnopqrstuvwxyz";
    
    for (i = 0; i < 26; i++)
    {
        if (c == letters[i])
            return true;
    }
    
    return false;
}

#endif
