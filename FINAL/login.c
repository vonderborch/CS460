#ifndef LOGIN_C
#define LOGIN_C

#include "ucode.c"
#include "vontypes.h" // draws from ucode.c here

char *tty;
int stdin, stdout, stderr;
bool checkcredentials(char user[], char pass[]);
char uname[INPUTBUFFER], home[INPUTBUFFER], program[INPUTBUFFER];
int uid, gid;

main(int argc, char *argv[])
{
    char user[INPUTBUFFER], pass[INPUTBUFFER];
    tty = argv[1];
    
    close(0); close(1); close(2);
    
    stdin = open(tty, READ);
    stdout = open(tty, WRITE);
    stderr = open(tty, ERROR);
    
    settty(tty);
    
    printf("VONLOGIN : open %s as stdin, stdout, stderr\n", tty);
    
    signal(2, 1);
    
    while (1)
    {
        printf("Username: ");
        gets(user);
        printf("Password: ");
        gets(pass);
        
        if (CheckCredentials(user, pass) == true)
        {
            printf("Valid credentials, welcome %s!\n", user);
            chuid(uid, gid);
            chdir(home);
            exec(program);
        }
        
        printf("Login failed, invalid credentials.\n");
    }
    
    exit(SUCCESS);
}

bool CheckCredentials(char user[], char pass[])
{
    int file = open("/etc/passwd", READ);
    char *token, *dl = ":\n", tmp[INPUTBUFFER], buffer[1024];
    
    if (file == null)
    {
        printf("Can't find password file!\n");
        return false;
    }
    
    if (read(file, buffer, 1024) < 0)
    {
        printf("Can't read password file!\n");
        return false;
    }

    token = strtok(buffer, dl); // grab user for first line
    while (token != null)
    {
        // do we have a valid username?
        if (!strcmp(token, user))
        {
            token = strtok(null, dl); // grab password
            
            // do we have a valid password?
            if (!strcmp(token, pass))
            {
                uid = atoi(strtok(null, dl));       // grab uid
                gid = atoi(strtok(null, dl));       // grab gid
                strcpy(uname, strtok(null, dl));     // grab username
                strcpy(name, user);
                strcpy(home, strtok(null, dl));     // grab home directory
                strcpy(program, strtok(null, dl));  // grab usershell
                
                close(file);
                return true;
            }
        }
        
        token = strtok(null, dl); // grab user on the next line
    }
    
    close(file);
    return false;
}

#endif
