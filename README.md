# CS3104-P1
CS3104-P1 Practical : Process Schedulers in C

## Files
    sched       : contains implementation for reading configuration file and creating of PCB data structure
    scheduler   : contains implementation for creating ready queue of processes and different schedulers
    main        : contains main method for scheduling processes according to given scheduling scheme
    printchars  : a program that can be scheduled to print chars
    chars.conf  : configuration file for processes/programs to schedule and execute

## Usage
    make clean     : removes all binaries and object files
    make all       : links header files and compiles all files into executable file "run"

    ./run <path to config file> <scheduling type (optional)>

    To run Round-Robin scheduling      :   ./run chars.conf 1
    To run priority-based scheduling   :   ./run chars.conf 2
