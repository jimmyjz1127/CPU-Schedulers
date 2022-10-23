#ifndef SCHEDULER_H
#define SCHEDULER_H

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

#include "sched.h"

/**
 * contains execution/scheduling data (time measurements, state)
 */
typedef struct ReadyQueue  {
    struct PCB *pcb; //the process itself
    int priority;//priority value of process
    int size;
    int terminated;//1 for terminated; 0 otherwise
    double time_spent; //burst time spent
    int num_bursts; //number of CPU bursts spent on process

    //time when process enters ready queue
    double arrival_time_sec;
    double arrival_time_nano;

    //time difference between when process enters ready queue to when it is complete
    double turnaround_time;

    struct ReadyQueue *next; //next process in ReadyQueue
    struct ReadyQueue *prev; //previous process in ReadyQueue
} ReadyQueue;

/**
 * populates ready queue with PCBs
 * @param (pcb_list) : the list of PCBs to populate ReadyQueue with
 */
ReadyQueue *createQueue(PCB *pcb_list);

/**
 * Given a ready queue, makes the queue cyclic by connecting head and tail nodes
 * @param (queue) :the ready queue to make cyclic
 */
void makeQueueCyclic(ReadyQueue *queue);

/**
 * frees memory of ReadyQueue by freeing all nodes
 * @param (queue) : ReadyQueue to free memory ofsdf
 */
void freeQueue(ReadyQueue *queue, size_t size);

/**
 * divides the queue into halves for merge sort
 * @param (queue) : the ready queue object to divide
 * @return : pointer to middle element of ReadyQueue object
 */
ReadyQueue *divideQueue(ReadyQueue *queue);

/**
 * Merges two halves of Ready Queue object after being halved by merge sort
 * @param (first) : the first half of ReadyQueue object
 * @param (second) : the second half of ReadyQueue object
 * @param (mode) : inidicates by what metric to sort by. 1 for priority rating, 2 for process/job size
 * @return : first and second merged in the correct order
 */
 ReadyQueue *mergeQueue(ReadyQueue *first, ReadyQueue *second, int mode);

 /**
  * Sorts ready queue of PCB's using merge sort algorithm
  * @param (queue) : the ready queue to sort
  * @param (mode) : inidicates by what metric to sort by. 1 for priority rating, 2 for process/job size
  * @return : the ReadyQueue sorted
  */
ReadyQueue *mergeSort(ReadyQueue *queue, int mode);

/**
 * Simple Priority Scheduler : execute processes from a ready-queue in order based on priority of processes
 * @param (queue) : ReadyQueue of processes to execute (sorted by priority value)
 */
void simplePriority(ReadyQueue *queue);

/**
 * Shortest Job First Scheduler : execute processes from a ready-queue in order based on process size
 * @param (queue) : ReadyQueue of processes to execute (sorted by priority value)
 */
void shortestJobFirst(ReadyQueue *queue);

/**
 * Executes the processes (PCBs) according to round robin scheduler schema
 * @param (queue) : ReadyQueue of processes to execute according to round robin schema
 * @param (time_quantum) : the round robin time time_quantum
 * @param (size) : the number of PCBs in ready queue
 */
void roundRobin(ReadyQueue *queue, useconds_t time_quantum, size_t size);

/**
 * Prints the scheduling info for all processes in ReadyQueue after being executed
 * info : (path to program, PID, number of CPU bursts, time spent)
 * @param (queue) : queue of completed processes
 * @param (num_elements) : the number of elements in readyqueue
 */
 void printDetails(ReadyQueue *queue, size_t num_processes);
#endif
