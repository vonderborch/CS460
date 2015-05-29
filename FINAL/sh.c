#ifndef VSH_C
#define VSH_C

#include "ucode.c"
#include "vontypes.h"

char directorateOfHome[DIRECTSIZE];

main(int argc, char *argv[])
{
    char input[COMMANDSIZE], bu[COMMANDSIZE];
    char cmd[COMMANDSIZE][INPUTBUFFER];
    int tmp;
    
    getcwd(directorateOfHome);
    
    while (1)
    {
    
        // 1. get the input!
        UpdateInput(input, bu);
        // 2. parse the input!
        tmp = BreakApartInput(bu, cmd);
        // 3. execute the input!
        if (tmp > 0)
        {
            printf("\n------------- EXECUTING COMMAND -------------\n\n");
            ExecuteInput(input, cmd, tmp);
            printf("-------------- COMMAND EXECUTED -------------");
        }
    }
    
    return 0;
}

int FindCommand(char *name)
{
    int i;

    for (i = 0; commands[i].name; i++)
    {
        if (!strcmp(commands[i].command, name))
            return i;
    }
    
    return -1;
}

bool DisplayShortHelp()
{
    int i;
    
    printf("\n---------------------------------------------\n");
    for (i = 0; commands[i].name; i++)
    {
        printf("  %s  ", commands[i].command);
    }
    printf("\n---------------------------------------------\n");
    
    return true;
}

bool DisplayHelp()
{
    int i;
    
    for (i = 0; commands[i].name; i++)
    {
        printf(" - %s %s: %s: %s\n", commands[i].command, commands[i].arguments, commands[i].name, commands[i].help);
        if (i % 8 == 7)
            gets();
    }
    
    return true;
}

// get the latest input!
bool UpdateInput(char *str, char *strbu)
{
    char directory[DIRECTSIZE];
    int length;
    
    // clear the input string
    for (length = 0; length < COMMANDSIZE; length++)
    {
        str[length] = 0;
        strbu[length] = 0;
    }
    
    // display the help screen
    DisplayShortHelp();
    
    // get the input!
    getcwd(directory);
    printf("User@vonsh: %s$ ", directory); // display a nice command query
    gets(str); // grab input
    
    // chop off the new line char at the end
    length = strlen(str);
    if (length > 1)
    {
        if (str[length - 1] == '\n')
            str[length - 1] = 0;
    }
    
    // make the backup!
    strcpy(strbu, str);
    
    return true;
}

// break apart the input!
int BreakApartInput(char *input, char cmd[COMMANDSIZE][INPUTBUFFER])
{
    int i, j;
    char *dl = " ", *token = strtok(input, dl);
    
    // clear the command buffer
    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < INPUTBUFFER; j++)
            cmd[i][j] = 0;
    }
    
    // break apart the input string
    i = 0;
    while (token)
    {
        strcpy(cmd[i], token);        // copy the string to a spot on the command buffer
        i++;                        // iterate to next spot
        token = strtok(null, dl);   // tokenize next spot for the list
    }
    cmd[i][0] = 0;
    
    return i;
}

// execute the input!
bool ExecuteInput(char *input, char cmd[COMMANDSIZE][INPUTBUFFER], int numargs)
{
    int pid, i, status = 0;
    bool haspipe = false;

    if (!strcmp(cmd[0], ""))
    {
        printf("Enter ? or help for a list of commands!\n");
        return true;
    }
    else if (!strcmp(cmd[0], "cd"))
    {
        if (numargs > 1)
            chdir(cmd[1]);
        else
            chdir(directorateOfHome);
    }
    else if (!strcmp(cmd[0], "kill"))
    {
        exit(1);
    }
    else if (!strcmp(cmd[0], "help") || !strcmp(cmd[0], "?"))
    {
        DisplayHelp();
    }
    else
    {
        // fork a child!
        pid = fork();
        
        if (pid < 0)
        {
            printf("Couldn't fork a child :(\n");
            exit(1);
        }
        
        // if pid is 1, wait
        if (pid)
        {
            printf("Parent %d forked child %d, but has to wait.\n", getpid(), pid);
            pid = wait(&status);
            printf("Parent %d found a dead child %d!\n", getpid(), pid);
        }
        // otherwise, lets look into actually running this thing...
        else
        {
            // first we look for a pipe
            i = 0;
            while (input[i] != 0)
            {
                if (input[i] == '|')
                {
                    haspipe = true;
                    break;
                }
                i++;
            }
        
            // if we don't have a pipe...
            if (haspipe == false)
                Execute(input);
            // if we have a pipe...
            else
                ExecutePipe(input);
        }
    }
    
    return true;
}

// executes a command
bool Execute(char *str)
{
    int rv = 0;
    char file[INPUTBUFFER];
    
    rv = HasRedirect(str, file);
    if (rv)
    {
        Redirect(file, rv);
    }
    
    exec(str);
    
    return true;
}

// starts a redirection
void Redirect(char *str, int rv)
{
    switch (rv)
    {
        case 1:
            close(0);
            open(str, O_RDONLY);
            break;
        case 2:
            close(1);
            open(str, O_WRONLY|O_CREAT, 0644);
            break;
        case 3:
            close(1);
            open(str, O_APPEND|O_WRONLY|O_CREAT, 0644);
            break;
        default:
            printf("Bad redirect!\n");
            break;
    }
}

// checks if the commandstring has a redirection
int HasRedirect(char *str, char *file)
{
    int i = 1, rv = 0;
    char tmp[INPUTBUFFER], tmp2[INPUTBUFFER];
    
    while (!rv && str[i])
    {
        // CASE 1: input redirection
        if (str[i] == '<')
        {
            rv = 1;
        }
        // CASE 2: output redirection
        else if (str[i] == '>')
        {
            if (str[i + 1] == '>')
                rv = 2;
            else
                rv = 3;
        }
        
        if (rv)
        {
            str[i - 1] = 0;
            str[i++] = 0;
            str[i++] = 0;
            
            if (str[i] == ' ')
                str[i++] = 0;
            
            strcpy(file, str + i);
        }
        
        i++;
    }
    
    return rv;
}

bool HeadPipe(char *str)
{
    int i = 0;
    while (str[i] != '|')
        i++;
    str[i - 1] = 0;
    
    return true;
}

bool TailPipe(char *str)
{
    int i = 0, j = 0;
    char tmp[256];
    
    // make a backup!
    strcpy(tmp, str);
    
    while (tmp[i] != '|')
        i++;
    i += 2;
    
    while (tmp[i])
        str[j++] = tmp[i++];
        
    str[j] = 0;
    
    return true;
}

bool ExecutePipe(char *str)
{
    int pd[2], pid, i;
    bool haspipe = false;
    
    // create the pipe!
    pipe(pd);
    
    // fork a child!
    pid = fork();
    
    // CASE 1: Child process runs head
    if (!pid)
    {
        HeadPipe(str);
        
        close(1);
        dup2(pd[1], 1);
        close(pd[0]);
        
        Execute(str);
    }
    // CASE 2: Parent process runs tail
    else
    {
        TailPipe(str);
        
        close(0);
        dup2(pd[0], 0);
        close(pd[1]);
        
        i = 0;
        while (str[i] != 0)
        {
            if (str[i] == '|')
            {
                haspipe = true;
                break;
            }
            i++;
        }
        
        if (haspipe == true)
        {
            ExecutePipe(str);
        }
        else
        {
            close(1);
            open("/dev/tty0", O_WRONLY);
            Execute(str);
        }
    }
}

#endif
