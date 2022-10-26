# CS3104-P1
CS3104-P1 Practical : Process Schedulers in C

## Files
    sched.c       : contains implementation for reading configuration file and creating of PCB data structure
    scheduler.c   : contains implementation for creating ready queue of processes and different schedulers
    main.c        : contains main method for scheduling processes according to given scheduling scheme
    printchars.c  : a program that can be scheduled to print chars
    chars.conf    : configuration file for executing printchars program

## Usage
    make clean     : removes all binaries and object files
    make all       : links header files and compiles all files into executable file "run"

    ./run <path to config file> <scheduling type (optional)>

    To run Round-Robin scheduling          :   ./sched chars.conf 1
    To run priority-based scheduling       :   ./sched chars.conf 2
    To run shortest-job-first scheduling   :   ./sched chars.conf 3
