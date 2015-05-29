#ifndef T_C
#define T_C

#include "types.h"
            
main()
{
    printf("MTX starts in main()\n");
    init();      // initialize and create P0 as running
    kfork();     // P0 kfork() P1
    while(1){
      printf("P0 running\n");
      
      if (proc[1].status != READY)
	    printf("no runable process, system halts\n");
	    
      while(!readyQueue);
      printf("P0 switch process\n");
      do_tswitch();   // P0 switch to run P1
   }
   printf("Exiting MTX!\n");
}

#endif

