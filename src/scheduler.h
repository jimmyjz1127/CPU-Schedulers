#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "sched.h";

//contains execution/scheduling data (time measurements, state)
typedef struct readyQueue  {
    PBC *pcb; //the process itself

    int terminated;//1 for terminated; 0 otherwise
    double time_spent; //burst time spent
    struct readyQueue next; //next process in ReadyQueue
    struct readyQueue prev; //previous process in ReadyQueue
} ReadyQueue;

/**
 * populates ready queue with PCBs
 * @param (pcb_list) : the list of PCBs to populate ReadyQueue with
 */
ReadyQueue *createQueue(PCB *pcb_list);

/**
 * frees memory of ReadyQueue by freeing all nodes
 * @param (queue) : ReadyQueue to free memory of
 */
void freeQueue(ReadyQueue *queue);

/**
 * Executes the processes (PCBs) according to round robin scheduler schema
 * @param (queue) : ReadyQueue of processes to execute according to round robin schema
 */
void roundRobin(ReadyQueue *queue);

/**
 * Prints the scheduling info for all processes in ReadyQueue after being executed (path to program, PID, time spent)
 * @param (queue) : queue of completed processes
 */
 void printDetails(ReadyQueue *queue);
#endif
