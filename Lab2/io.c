#ifndef IO_C
#define IO_C

#define BASE 10
#define HEX 16
char *table = "0123456789ABCDEF";

int mprintf(char *fmt, ...);
void mprintd(int x);
void mprintx(unsigned long x);
void mprintc(char c);
void mprints(char *s);
void mprintu(unsigned long x);
void mrpd(int x);
void mrpx(unsigned long x);
void mrpu(unsigned long x);

// myprintf: prints a passed string!
int mprintf(char *fmt, ...)
{
	if (strcmp(fmt, "") != 0)
	{
		int i = 0, *ebp = getebp(), *ip = ebp + 4;
		for (i = 0; fmt[i] != '\0'; i++)
		{
			if (fmt[i] == '%')
			{
				i++;
				if (fmt[i] == 'c')
					mprintc(*ip);
				else if (fmt[i] == 'd')
					mprintd(*ip);
				else if (fmt[i] == 'x')
					mprintx(*ip);
				else if (fmt[i] == 's')
					mprints(*ip);
				else if (fmt[i] == '%')
					putc('%');
				else
					return 0;
				ip++;
			}
			else if (fmt[i] == '/')
			{
				i++;
				if (fmt[i] == 'n')
				{
					putc('\n');
					putc('\r');
				}
				i++;
			}
			else if ((int)fmt[i] == 92)
			{
				i++;
				if (fmt[i] == 'n')
				{
					putc('\n');
					putc('\r');
				}
				i++;
			}
			else
				putc(fmt[i]);
		}
	}
	return 1;
}

//// Printing Helper Functions
// print an integer
void mprintd(int x)
{
	if (x == 0)
		putc('0');
	else if (x < 0)
	{
		putc('-');
		mrpd(-x);
	}
	else
		mrpd(x);
	putc(' ');
}

// print a hex value
void mprintx(unsigned long x)
{
	if (x == 0)
		putc('0');
	else
		mrpx(x);
	putc(' ');
}

// print a character
void mprintc(char c)
{
	putc(c);
}

// print a string
void mprints(char *s)
{
	int i = 0;
	for (i = 0; s[i] != '\0'; i++)
		putc(s[i]);
	putc(' ');
}
// prints an unsigned long (non-negative number)
void mprintu(unsigned long x)
{
	if (x == 0)
		putc('0');
	else
		mrpu(x);
	putc(' ');
}

//// Printing Recursive Helper Functions
// recursive helper for printu
void mrpd(int x)
{
	char c;
	if (x)
	{
		c = table[x % BASE];
		mrpu(x / BASE);
		putc(c);
	}
}

// recursive helper for printu
void mrpx(unsigned long x)
{
	char c;
	if (x)
	{
		c = table[x % HEX];
		mrpx(x / HEX);
		putc(c);
	}
}

// recursive helper for printu
void mrpu(unsigned long x)
{
	char c;
	if (x)
	{
		c = table[x % BASE];
		mrpu(x / BASE);
		putc(c);
	}
}

#endif
