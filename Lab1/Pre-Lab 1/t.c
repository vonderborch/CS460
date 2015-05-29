/************** t.c file **************************/
int prints(char *s)
{
	while (*s) putc(*s++);
}

int gets(char s[ ])
{
	while ((*s = getc()) != '\r') putc(*s++);
	*s = 0;
}

main()
{
   char name[64];
   while(1){
     prints("What's your name? ");
     gets(name);
     if (name[0]==0)
        break;
     prints("Welcome "); prints(name); prints("\n\r");
   }
   prints("return to assembly and hang\n\r");
}
