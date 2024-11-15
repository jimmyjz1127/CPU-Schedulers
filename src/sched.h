#ifndef SCHED_H
#define SCHED_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>

/**
 * PCB linked list struct
 */
typedef struct PCB {
    char *path; //path to program
    int priority; //priority rating
    pid_t pid; //Process ID of process to execute program
    int size; //size of process

    struct PCB *prev; //next PCB
    struct PCB *next; //previous PCB
} PCB;

/**
 * Creates PCB for a child process containing relevant information (PID, priority, index, prev, next)
 * @param (config_file) : the path to config_file containing programs to be executed through scheduling scheme
 * @param (pcb_list) : queue of PCBs to populate
 * @param (num_processes) : number of processes to schedule
 * @return : head to linked list of PCB objects
 */
PCB *createPCBList(char *config_file, PCB *pcb_list, size_t *num_processes);

/**
 * Creates a new PCB for a process
 * @param (path) : path to program to execute
 * @param (priority) : priority of program to execute
 * @param (pid) : process ID of program to execute
 * @param (size) : the size of the process (for shortest-job-first scheduler)
 * @param (prev) : previous PCB in PCB list
 * @param (next) : next PCB in PCB List
 * @return : PCB object
 */
PCB *createPCB(char *path, int priority, pid_t pid, int size, PCB *prev, PCB *next);

/**
 * Frees memory of PCB list
 * @param (pcb_list) : the list of PCBs to free the memory of
 */
void freePCBList(PCB *pcb_list);

/**
 * Splits string into array using spaces as delimeter
 * @param (str) : the string to split
 * @param (size) : size attribute. Used by calling functions to determine size of string array produced
 * @return : the array of strings
 */
char **splitStr(char *str, size_t *size);

/**
 * Frees a given string array
 * @param (strArr) : the string array to free
 * @param (num_elements) : the number of elements in string array
 */
void freeStrArr(char **strArr, size_t num_elements);
#endif
