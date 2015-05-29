#ifndef HELPERS_C
#define HELPERS_C

#include "type.h"

// gets a string and returns it!
char* getstring(char *str)
{
    // vars
    char temp[INPUTBUFFER];
    int i;
	u16 segment = (running->pid +1) * 0x1000;
    
    // while we've not yet reached the end of the string, cycle...
    for (i = 0; i < INPUTBUFFER; i++)
    {
        // get the next char and make sure we've not reached the end of the string.
        temp[i] = get_byte(segment, str + i);
        if (temp[i] == '\0')
            break;
    }
    
    // make sure that we end on a nullchar and return the string
    temp[i] = '\0';
    return temp;
}

#endif
