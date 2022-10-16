#include "scheduler.h"

/**
 * populates ready queue with PCBs
 * @param (pcb_list) : the list of PCBs to populate ReadyQueue with
 */
ReadyQueue *createQueue(PCB *pcb_list) {
    ReadyQueue *prev = NULL;
    while (pcb_list){
        ReadyQueue *queue = malloc(sizeof(ReadyQueue));

        queue->pcb = pcb_list;
        queue->terminated=0;
        queue->time_spent=0;
        queue->num_bursts=0;

        queue->next = prev;
        if (prev){
            prev->prev=queue;
        }

        prev = queue;

        pcb_list=pcb_list->next;
    }

    return prev;
}

/**
 * frees memory of ReadyQueue by freeing all nodes
 * @param (queue) : ReadyQueue to free memory of
 */
void freeQueue(ReadyQueue *queue) {
    while (queue) {
        ReadyQueue *temp = queue->next;
        free(queue);
        queue = temp;
    }
}


/**
 * Simple Priority Scheduler : execute processes from a ready-queue in order based on priority of processes
 * @param (queue) : ReadyQueue of processes to execute (sorted by priority value)
 */
void simplePriority(ReadyQueue *queue){

}


/**
 * Executes the processes (PCBs) according to round robin scheduler schema
 * @param (queue) : ReadyQueue of processes to execute according to round robin schema
 * @param (time_quantum) : the round robin time time_quantum
 * @param (size) : the number of PCBs in ready queue
 */
void roundRobin(ReadyQueue *queue, useconds_t time_quantum, size_t size) {
    ReadyQueue *head = queue; //the the head of queue

    size_t num_terminated = 0;//number of terminated processes

    //iterate through the queue
    while (queue) {
        //check that process has not terminated
        if (queue->terminated == 0) {
            pid_t pid = queue->pcb->pid;

            printf("\nExecuting CPU burst on [%s] with PID = [%d]\n", queue->pcb->path, pid);

            clock_t begin = clock(); //get start time of process

            //Execute CPU burst on process
            kill(pid, SIGCONT); //resume (start) process
            usleep(time_quantum);//allow process to execute for time quantum
            kill(pid, SIGSTOP); //stop process

            clock_t end = clock();//get end time of process

            double burst = ((double)(end - begin))/CLOCKS_PER_SEC;//time spent on cpu burst

            queue->time_spent+=burst;
            queue->num_bursts+=1;

            //Check if process has terminated
            if (waitpid(pid, NULL, WNOHANG) != 0){ //use WNOHANG to prevent suspention or waiting
                queue->terminated = 1;
                num_terminated += 1;
            }
        }//end if
        if (num_terminated == size) {//if all processes are terminated then break while loop
            break;
        }
        queue = queue->next;
    }//end while
}//end roundRobin()

/**
 * Prints the scheduling info for all processes in ReadyQueue after being executed
 * info : (path to program, PID, number of CPU bursts, time spent)
 * @param (queue) : queue of completed processes
 */
 void printDetails(ReadyQueue *queue) {
     ReadyQueue *temp = queue;

     while (temp){
         printf("\nProgram [%s] with PID=[%d] executed for [%d] CPU burst with total time = [%f] \n",
                queue->pcb->path, queue->pcb->pid, queue->num_bursts, queue->time_spent);
        temp = temp->next;
     }
 }
