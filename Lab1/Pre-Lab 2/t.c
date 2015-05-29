/********** t.c file ******************/

#include "ext2.h"  /* download this from LAB1 */

#define BLK 1024

int prints(char *s)
{
	while (*s) putc(*s++);
}

u16 getblk(u16 blk, char buf[ ])
{
   // convert blk to CHS; then call readfd(c,h,s, buf);
   readfd(blk/18, (blk/9)%2, (blk*2)%18, buf);
}

u16 printnewline() { prints("\n\r"); }

GD    *gp;
INODE *ip;
DIR   *dp;

main()
{ 
    u16  i, iblk, good, tempint;
    char buf1[BLK], buf2[BLK];
    char *temp;

    prints("booter start\n\r");  

    /* read blk#2 to get group descriptor 0 */
    getblk((u16)2, buf1);
    gp = (GD *)buf1;
    iblk = (u16)gp->bg_inode_table; // typecast u32 to u16

    prints("inode_block="); putc(iblk+'0'); printnewline(); printnewline(); getc();

    /******** write C code to do these: ********************
    (1).read first inode block into buf1[ ]

    (2).let ip point to root INODE (inode #2)

    (3).For each DIRECT block of / do:
          read data block into buf2[ ];
          step through the data block to print the names of the dir entries 

    (4).prints("\n\rAll done\n\r");
    ******************************************************/
    // (1).read first inode block into buf1[ ]
    getblk(iblk, buf1);

    // let ip point to root INODE (inode #2)
    ip = (INODE *)buf1 + 1;

    // For each DIRECT block of / do...
    for (i = 0; i < 12; i++)
    {
        if (i < 10)
        { prints("Block, i= "); putc(i + '0'); printnewline(); }
        else if (i == 10)
        { prints("Block, i= 10\n\r"); }
        else
        { prints("Block, i= 11\n\r"); }
        
        if (ip->i_block[i] == 0)
        {
            prints("No dir entries in this block!"); printnewline();
        }
        else
        {
            getblk((u16)ip->i_block[i], buf2);
            gp = (GD *)buf2;
            
            dp = (DIR *)buf2;
            good = 0;
            while (good == 0)
            {
                strncpy(temp, dp->name, dp->name_len);
                temp[dp->name_len] = 0;
                if (temp || strlen(temp + '\0') == 0)
                    good = 1;
                else
                {
                    prints(temp); putc(' ');
                    
                    dp = (DIR *)((char *)dp + dp->rec_len);
                }
            }
            printnewline();
        }

    }

    // (4).prints("\n\rAll done\n\r");
    prints("\n\rAll done!\n\r");
}



