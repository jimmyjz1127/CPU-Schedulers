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
    int index; //index of process in list of PCBs


    struct PCB *prev; //next PCB
    struct PCB *next; //previous PCB
} PCB;

/**
 * Creates PCB for a child process containing relevant information (PID, priority, index, prev, next)
 * @param (*config_file) : the path to config_file containing programs to be executed through scheduling scheme
 * @param (*pcbQueue) : queue of PCBs to populate
 * @param (num_processes) : number of processes to schedule
 */
PCB *createPCBList(char *config_file, PCB *pcb_list, size_t *num_processes);

/**
 * Creates a new PCB for a process
 * @param (path) : path to program to execute
 * @param (priority) : priority of program to execute
 * @param (pid) : process ID of program to execute
 * @param (index) : index within PCB List
 * @param (prev) : previous PCB in PCB list
 * @param (next) : next PCB in PCB List
 */
PCB *createPCB(char *path, int priority, pid_t pid, int index, PCB *prev, PCB *next);

/**
 * Frees memory of PCB list
 * @param (pcb_list) : the list of PCBs to free the memory of
 */
void freePCBList(PCB *pcb_list);


/**
 * Splits string according to provided delimeter
 * @param (str) : the string the split
 * @param (delimeter) : the delimeter to split the string by
 */
char **splitStr(char *str, size_t *size);

/**
 * Frees a given string array
 * @param (strArr) : the string array to free
 * @param (num_elements) : the number of elements in string array
 */
void freeStrArr(char **strArr, size_t num_elements);
#endif
