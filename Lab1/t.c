/************** t.c file **************************/

#include "ext2.h"

#define BLK 1024

// prints: basic output.
u16 prints(char *s)
{
    // while there is a character to print, print it.
	while (*s) putc(*s++);
}

// gets: basic input.
u16 gets(char s[ ])
{
    // while we haven't gotten to a new line, get the input.
	while ((*s = getc()) != '\r') putc(*s++);
	// add a null char at the end.
	*s = 0;
}

// getblk: gets the asked for block and stores it in buf[].
u16 getblk(u16 blk, char buf[ ])
{
   // convert blk to CHS; then call readfd(c,h,s, buf);
   readfd(blk/18, (blk/9)%2, (blk*2)%18, buf);
}

// finder: finds the inode of the asked for file.
INODE *finder(char *str, INODE *ip, u16 iblk)
{
    // Variables!
    u8 i;
    DIR   *dp;
    char *cp, buf[BLK], temp[64];
    
    // look through each block...
    for (i = 0; i < 12; i++)
    {
        // if the i block doesn't exit, don't bother checking any further. the file doesn't exist :(
        if (ip->i_block[i] == 0) break;
        
        // get the block and store it in the buf...
        getblk((u16)ip->i_block[i], buf);
        // get the block's directory...
        dp = (DIR *)buf;
        // store in string so we can figure out when to end...
        cp = buf;
        
        // while there are still files to check!
        while (cp < buf + BLK)
        {
            // copy the string into the temporary array...
            strncpy(temp, dp->name, dp->name_len);
            temp[dp->name_len] = 0;
            
            // check if the string matches the one we want...
            if (!strcmp(str, temp))
            {
                // if it does, return it's inode!
                i = dp->inode - 1;
                getblk((i / 8) + iblk, buf);
                return (INODE *)buf + (i % 8);
            }
            
            // if not, check the next file...
            cp += dp->rec_len;
            dp = (DIR*)cp;
        }
    }
    
    // return failure :(
    return -1;
}

main()
{
    // Variables!
    GD    *gp;
    INODE *ip;
    DIR   *dp;
    u16  i, iblk;
    u32 *iblk2;
    char buf1[BLK], buf2[BLK], file[64];
        
    /* read blk#2 to get group descriptor 0 */
    getblk((u16)2, buf1);
    gp = (GD *)buf1;
    iblk = (u16)gp->bg_inode_table; // typecast u32 to u16
    getblk(iblk, buf1); // read the first inode into buf1[]
    ip = (INODE *)buf1 + 1; // let ip point to the root inode
    
    // (1). Prompt for filename to boot.
    prints("Boot: "); // ask
    gets(file); // get answer
    
    // (2). Find the file! assume everything is in /boot
    // first we find the boot directory...
    ip = finder("boot", ip, iblk);
    if (ip == -1) error();
    // then we find the file the user asked for...
    ip = finder(file, ip, iblk);
    if (ip == -1) error();
    
    // (3). From the file's inode, get the block info...
    i = ip->i_block[12];
    getblk(i, buf2);
    iblk2 = buf2;
    
    // (4). load the blocks into memory at 0x1000.
    setes(0x1000); // set segment 0x1000 up...
    
    // load direct blocks...
    for (i = 0; i < 12; i++)
    {
        getblk(ip->i_block[i], 0);
        inces();
    }
    
    // load indirect blocks...
    while (*iblk2 != 0)
    {
        getblk(*iblk2++, 0);
        inces();
    }
}

    /*
(1). Prompt for filename to boot, e.g. mtx or image, etc. You may assume
 that all bootable files are in the /boot directory.
(2). Find the file. Recall that "finding a file amounts to finding its 
 inode". 
(3). From the file's inode, find the disk blocks of the file:
       i_block[0] to i_block[11] are DIRECT blocks, and
       i_blokc[12] points to INDIRECT blocks.
 MTX kerenl has at most 64 (1KB) blocks, so no double-indirect blocks.

(4). Load the blocks of /boot/mtx into memory at the segment 0x1000.
(5). Any errro condition, call error() in assembly.
==========================================================================  
(6). If YOUR main() loads the disk blocks of mtx successfully, it returns to
 bs.s, which jumps to (0x1000, 0) to start up MTX.
    */
    
    
    
    
