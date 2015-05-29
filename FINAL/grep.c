#ifndef GREP_C
#define GREP_C

#include "ucode.c"
#include "vontypes.h"

#define NUMLINES 128

int main(int argc, char *argv[])
{
    int fd, patternsize, i, reader, count = 0, linenum = 0;
    char tty[INPUTBUFFER], line[INPUTBUFFER];
    bool searching = true;
    
    if (argc == 1)
    {
        printf("Usage: grep pattern [file]");
        exit(FAIL);
    }
    else if (argc == 2)
    {
        fd = 0;
    }
    else
    {
        fd = open(argv[2], O_RDONLY);
    }
    
    if (fd >= 0)
    {
        patternsize = strlen(argv[1]);
        printf("Grep looking for %s at beginning of line...\n", argv[1]);
        
        while (searching == true)
        {
            ClearString(&line);
            
            // build the input buffer
            for (i = 0; i < NUMLINES - 1; i++)
            {
                reader = read(fd, &line[i], 1);
                
                if (reader < 1)
                {
                    searching = false;
                    break;
                }
                if (line[i] == '\n')
                {
                    line[i + 1] = 0;
                    break;
                }
                if (line[i] == '\r')
                {
                    line[i + 1] = 0;
                    break;
                }
                if (line[i] == 0)
                {
                    searching = false;
                    break;
                }
            }
            
            linenum++;
            // check for match
            if (searching == true && CheckLine(line, argv[1], patternsize))
            {
                count++;
                printf("Found match at line %d (%s)\n", linenum, line);
            }
        }
    }
    else
    {
        printf("Grep couldn't find the asked for file!\n");
        exit(FAIL);
    }
    
    printf("\nFound %d total matches!\n", count);

    close(fd);
    exit(SUCCESS);
}

// clears a string!
bool ClearString(char *str)
{
    int i;
    for (i = 0; i < NUMLINES; i++)
        str[i] = 0;
        
    return true;
}

bool CheckLine(char *str, char *pattern, int patternsize)
{
    if (!strncmp(str, pattern, patternsize))
    {
        return true;
    }
    
    return false;
}

#endif
