# CS460
Labs and Final Project for CS460 (Operating Systems and Computer Architecture) at WSU, taught by K. C. Wang during the Spring 2015 semester.

All code done in C (rare assembly and bash scripting as well) and meant to run with QEMU on a Linux distro. The operating system is a custom one created by the professor called MTX and the goal for the class was to learn how it worked to learn more about operating systems. MTX interacts with an ext2 filesystem. I make no guarentees about whether any of this will work perfectly, as it was/is fairly finicky.

A book for the MTX OS (and the course) is now available: Design and Implementation of the MTX Operating System, KCW, Springer International AG, 2015: http://www.springer.com/us/book/9783319175744  ,  http://school.eecs.wsu.edu/unique_features_design_implemantation_mtx_operating_system

Folders:
- FINAL: The final project for the class, an attempt to create a fully functional (if basic) OS using a base version of the MTX operating system kernal provided by the professor. The base version was used to ensure that everyone started on the same page rather than some people being ahead with their versions of the MTX operating system kernal and some behind, as well as allowing us more functionality to use in regards to system calls.
- Lab1: A booter for the MTX operating system.
  - PreLab1: A basic 'boot' file that asks for a user's name and welcomes the user (but not actually doing any booting of the OS).
  - PreLab2: Reads the directory for the ext2 filesystem.
- Lab2: In this lab, we created and run the low-level processes for the operating system (there are 8 processes in this version, 1 of which is 'reserved' by the OS). This involved writing our own process scheduler, initialization, forker, and process queues.
- Lab3: In this lab we implemented the exit, sleep, wakeup, and wait functions into our version of the MTX kernal.
- Lab4: In this lab we implemented the getpid (process id), ps (process status), chname (change process name), kfork (fork a process), kswitch (switch process), wait (causes the process to wait for a child to die), exit (kill the process), and getc/putc system calls to our MTX kernal. This is also the first time we had a usermode shell to load and run user executables.
- Lab5: In this lab we implemented the fork (fork a child process with an identical usermode image) and exec (change the usermode image to the specified executable file) system calls to our MTX kernal.
- Lab6: In this lab we implemented and tested pipes in our version of the MTX operating system.
- Lab7: In this lab we implemented and tested some simple drivers for devices on our version of the MTX operating system.
- TestLab3: Lab 3 modified for the mid-term. This has a modified (shorter) tswitch assembly statement for switching between processes.
- TestLab4: Lab 4 modified for the mid-term. This has a modified (shorter) int80h assembly statement (used when forking procecsses) and a modified kfork function that works with this new version of the assembly statement. Another modification is that it includes a getMyname system call to return the running processes name and length of the name string. It is also supposed to (although it doesn't exactly) be running a process's user mode in a 32KB segment rather than the standard 64KB one. A final modification (which also doesn't work perfectly) is a hop system call that hops the process between memory segments without modifying them.
- MTXinstallCD.7z: The installation cd for the MTX operating system (provided by the professor). Compressed to save space.
- vdisk.7z: A base version of the virtual disk for the course (provided by the professor). Compressed to save space.
