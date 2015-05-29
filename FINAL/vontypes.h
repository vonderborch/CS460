#ifndef VONTYPES_H
#define VONTYPES_H

//////////////////////////////////////// DEFINITIONS ////////////////////////////////////////
// define null codes
#define null 0

// define exit codes
#define SUCCESS 0
#define FAIL    1

// define buffer sizes
#define INPUTBUFFER 64
#define COMMANDSIZE 256
#define DIRECTSIZE  1024
#define COMMANDNUM  16

// define read/write codes
#define READ  0
#define WRITE 1
#define ERROR 2

// BOOL DEFINITION
typedef enum { false, true } bool;

// COMMAND TABLE
typedef struct command_table{
	char *name;
	char *command;
	int (*f)();
	char *help;
	char *arguments;
} COMMANDTABLE;

//////////////////////////////////////// FUNCTIONS ////////////////////////////////////////
////////// init.c //////////
int Login();
int Parent();

////////// grep.c //////////
bool ClearString(char *str);

////////// login.c //////////
bool CheckCredentials(char user[], char pass[]);

////////// more.c //////////
bool Print(int fd, int lines);

////////// sh.c //////////
int FindCommand(char *name);
bool DisplayShortHelp();
bool DisplayHelp();
bool UpdateInput(char *str, char *strbu);
int BreakApartInput(char *input, char cmd[COMMANDNUM][INPUTBUFFER]);
bool ExecuteInput(char *input, char cmd[COMMANDNUM][INPUTBUFFER], int numargs);
bool Execute(char *str);
void Redirect(char *str, int rv);
int HasRedirect(char *str, char *file);
bool HeadPipe(char *str);
bool TailPipe(char *str);
bool ExecutePipe(char *str);

////////// l2u.c //////////
bool IsLowercaseLetter(char c);

////////// u2l.c //////////
bool IsUppercaseLetter(char c);

//////////////////////////////////////// COMMAND TABLE ////////////////////////////////////////
COMMANDTABLE commands[] = {
	{"Catenate", "cat", 0, "Outputs the contents of a speceific file.", "[file]"},
	{"More", "more", 0, "Views, but doesn't modify, the contents of a file.", "[file]"},
	{"Grep", "grep", 0, "Matches patterns in a file.", "pattern [file]"},
	{"Lower To Upper", "l2u", 0, "Converts all lowercase letters to uppercase.", "[f1 f2]"},
	{"Upper To Lower", "u2l", 0, "Converts all uppercase letters to lowercase.", "[f1 f2]"},
	{"Copy Paste", "cp", 0, "Copys src file to dest.", "src dest"},
	{"Help", "help", 0, "Displays the help screen.", ""},
	{"Kill", "kill", 0, "Kills the process.", ""},
	{0, 0, 0, 0, 0}
};
//	{"", "", 0, "", ""},

#endif
