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

typedef struct pcb {
    char *path; //path to program
    int priority; //priority rating
    pid_t pid; //Process ID of process to execute program
    int index; //index of process in list of PCBs


    struct pcb *prev; //next PCB
    struct pcb *next; //previous PCB
} PCB;

/**
 * Creates PCB for a child process containing relevant information (PID, priority, index, prev, next)
 * @param (*config_file) : the path to config_file containing programs to be executed through scheduling scheme
 * @param (*pcbQueue) : queue of PCBs to populate
 */
PCB createProcessPCB(char *config_file, PCB *pcbQueue);

/**
 * Frees memory of PCB list
 * @param (pcb_list) : the list of PCBs to free the memory of
 */
void freePCBList(PCB *pcb_list);

/**
 * Splits string according to provided delimeter
 * @param (str) : the string the split
 * @param (delimeter) : the delimeter to split the string by
 * source : https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
 */
char **splitStr(char *str, const char delimeter, size_t *size);

/**
 * Frees a given string array
 * @param (strArr) : the string array to free
 */
void freeStrArr(char **strArr);
#endif
