#include "scheduler.h"

/**
 * populates ready queue with PCBs
 * @param (pcb_list) : the list of PCBs to populate ReadyQueue with
 */
ReadyQueue *createQueue(PCB *pcb_list) {
    ReadyQueue *prev = NULL;
    PCB *elem = pcb_list;
    while (elem){
        ReadyQueue *queue = malloc(sizeof(ReadyQueue));

        queue->pcb = elem;
        queue->priority = elem->priority;
        queue->size = elem->size;
        queue->terminated=0;
        queue->burst_time=0;
        queue->num_bursts=0;

        /* Record arrival time (time when process enters ready-queue)  */
        struct timespec arrival;
        clock_gettime(CLOCK_MONOTONIC, &arrival);
        queue->arrival_time_sec = arrival.tv_sec;
        queue->arrival_time_nano = arrival.tv_nsec;

        queue->next = prev;
        if (prev){
            prev->prev=queue;
        }
        prev = queue;

        elem=elem->next;
    }
    return prev;
}

/**
 * Given a ready queue, makes the queue cyclic by connecting head and tail nodes
 * @param (queue) :the ready queue to make cyclic
 */
void makeQueueCyclic(ReadyQueue *queue) {
    ReadyQueue *elem = queue;

    while (elem->next) {
        elem = elem->next;
    }

    queue->prev = elem;
    elem->next = queue;
}

/**
 * frees memory of ReadyQueue by freeing all nodes
 * @param (queue) : ReadyQueue to free memory of
 */
void freeQueue(ReadyQueue *queue, size_t size) {
    size_t count = 0;
    while (queue && count < size) {
        ReadyQueue *temp = queue->next;
        free(queue);
        queue = temp;
        count++;
    }
}

/**
 * divides the queue into halves for merge sort
 * @param (queue) : the ready queue object to divide
 * @return : pointer to middle element of ReadyQueue object
 */
ReadyQueue *divideQueue(ReadyQueue *queue) {
    ReadyQueue *slow = queue;
    ReadyQueue *fast = queue;

    while (fast->next && fast->next->next){
        fast = fast->next->next;
        slow = slow->next;
    }
    slow->next->prev = NULL;//disconnect the two halves of queue

    ReadyQueue *middle = slow->next;

    slow->next = NULL;

    return middle;
}

/**
 * Merges two halves of Ready Queue object after being halved by merge sort
 * @param (first) : the first half of ReadyQueue object
 * @param (second) : the second half of ReadyQueue object
 * @param (mode) : inidicates by what metric to sort by. 1 for priority rating, 2 for process/job size
 * @return : first and second merged in the correct order
 */
 ReadyQueue *mergeQueue(ReadyQueue *first, ReadyQueue *second, int mode) {
     if (!first) return second;
     if (!second) return first;

     if (mode==1 && first->priority <= second->priority){
         first->next = mergeQueue(first->next, second, mode);
         first->next->prev = first;
         first->prev = NULL;
         return first;
     }
     else if (mode==2 && first->size <= second->size){
         first->next = mergeQueue(first->next, second, mode);
         first->next->prev = first;
         first->prev = NULL;
         return first;
     }
     else {
         second->next = mergeQueue(first, second->next, mode);
         second->next->prev = second;
         second->prev = NULL;
         return second;
     }
 }//end mergeQueue

/**
 * Sorts ready queue of PCB's using merge sort algorithm
 * @param (queue) : the ready queue to sort
 * @param (mode) : inidicates by what metric to sort by. 1 for priority rating, 2 for process/job size
 * @return : the ReadyQueue sorted
 */
ReadyQueue *mergeSort(ReadyQueue *queue, int mode){
    if (!queue || !(queue->next)) return queue;

    ReadyQueue *second = divideQueue(queue);

    queue = mergeSort(queue, mode);
    second = mergeSort(second, mode);

    return mergeQueue(queue, second, mode);
}


/**
 * Simple Priority Scheduler : execute processes from a ready-queue in order based on priority of processes
 * @param (queue) : ReadyQueue of processes to execute (sorted by priority value)
 */
void simplePriority(ReadyQueue *queue) {
    ReadyQueue *head = mergeSort(queue, 1);

    printf("\n--------------------EXECUTING PROCESSES--------------------\n");

    while (head) {
        if (head->terminated == 0){
            pid_t pid = head->pcb->pid;

            printf("\nExecuting CPU burst on [%s] with PID = [%d] and priority = [%d]\n", head->pcb->path, pid, head->pcb->priority);

            //start & end time objects
            struct timespec start, end;

            clock_gettime(CLOCK_MONOTONIC, &start);

            //Execute CPU burst
            kill(pid, SIGCONT);

            int status;
            waitpid(pid, &status, 0);//wait until process has finished

            clock_gettime(CLOCK_MONOTONIC, &end);

            head->turnaround_time = (end.tv_sec - head->arrival_time_sec)
                                    + (double)(end.tv_nsec - head->arrival_time_nano)/1000000000L;

            head->burst_time += (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000L;
            //calculate waiting time as difference between turnaround time and burst time
            head->waiting_time = head->turnaround_time - head->burst_time;

            head->num_bursts = 1;
            head->terminated = 1;
        }
        head=head->next;
    }
    printf("\n-------------------------FINISHED-------------------------\n");
}//end simplePriority()

/**
 * Shortest Job First Scheduler : execute processes from a ready-queue in order based on process size
 * NOTE : only works for "printchars" program
 * @param (queue) : ReadyQueue of processes to execute (sorted by priority value)
 */
void shortestJobFirst(ReadyQueue *queue) {
    ReadyQueue *head = mergeSort(queue, 2);
    printf("\n--------------------EXECUTING PROCESSES--------------------\n");

    while (head) {
        if (head->terminated == 0){
            pid_t pid = head->pcb->pid;

            printf("\nExecuting CPU burst on [%s] with PID = [%d]\n", head->pcb->path, pid);

            //start & end time objects
            struct timespec start, end;

            clock_gettime(CLOCK_MONOTONIC, &start);

            //Execute CPU burst
            kill(pid, SIGCONT);

            int status;
            waitpid(pid, &status, 0);//wait until process has finished

            clock_gettime(CLOCK_MONOTONIC, &end);

            //set turn around time as difference between arrival time and completion time
            head->turnaround_time = (end.tv_sec - head->arrival_time_sec)
                                    + (double)(end.tv_nsec - head->arrival_time_nano)/1000000000L;

            head->burst_time += (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000L;
            //calculate waiting time as difference between turnaround time and burst time
            head->waiting_time = head->turnaround_time - head->burst_time;

            head->num_bursts = 1;
            head->terminated = 1;
        }
        head=head->next;
    }
    printf("\n-------------------------FINISHED-------------------------\n");
}//end shortestJobFirst()

/**
 * Executes the processes (PCBs) according to round robin scheduler schema
 * @param (queue) : ReadyQueue of processes to execute according to round robin schema
 * @param (time_quantum) : the round robin time time_quantum
 * @param (size) : the number of PCBs in ready queue
 */
void roundRobin(ReadyQueue *queue, useconds_t time_quantum, size_t size) {
    makeQueueCyclic(queue);

    ReadyQueue *elem = queue; //the the head of queue

    size_t num_terminated = 0;//number of terminated processes

    printf("\n--------------------EXECUTING PROCESSES--------------------\n");

    //iterate through the queue
    while (elem) {
        //check that process has not terminated
        if (elem->terminated == 0) {
            pid_t pid = elem->pcb->pid;

            printf("\nExecuting CPU burst on [%s] with PID = [%d]\n", elem->pcb->path, pid);

            //start & end time objects
            struct timespec start, end;

            clock_gettime(CLOCK_MONOTONIC, &start);

            //Execute CPU burst on process
            kill(pid, SIGCONT); //resume (start) process
            usleep(time_quantum);//allow process to execute for time quantum
            kill(pid, SIGSTOP); //stop process

            clock_gettime(CLOCK_MONOTONIC, &end);

            elem->burst_time+=(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000L;
            elem->num_bursts+=1;

            //Check if process has terminated
            int status;
            if (waitpid(pid, &status, WNOHANG) != 0){ //use WNOHANG to prevent suspention or waiting
                //set turn around time as difference between arrival time and completion time
                elem->turnaround_time = (end.tv_sec - elem->arrival_time_sec)
                                        + (double)(end.tv_nsec - elem->arrival_time_nano)/1000000000L;
                //calculate waiting time as difference between turnaround time and burst time
                elem->waiting_time = elem->turnaround_time - elem->burst_time;

                elem->terminated = 1;
                num_terminated += 1;
            }
        }//end if
        if (num_terminated >= size) {//if all processes are terminated then break while loop
            break;
        }
        elem = elem->next;
    }//end while
    printf("\n-------------------------FINISHED-------------------------\n");
}//end roundRobin()

/**
 * Prints the scheduling info and time metrics for all processes executed from ready queue
 * @param (queue) : queue of completed processes
 * @param (num_elements) : the number of elements in readyqueue
 */
 void printDetails(ReadyQueue *queue, size_t num_processes) {
     ReadyQueue *temp = queue;
     size_t counter = 0;

     double total_time = 0;//total cpu time spent
     double total_turnaround_time = 0;//total turnaround tim
     double total_wait_time = 0;//total cpu waiting time

     printf("\nDETAILS:\n");

     while (temp && counter < num_processes){
         total_time += temp->burst_time;
         total_turnaround_time += temp->turnaround_time;
         total_wait_time += temp->waiting_time;

         printf("Program [%s] with PID=[%d] executed for [%d] CPU burst with total time = [%lf]\n",
                temp->pcb->path, temp->pcb->pid, temp->num_bursts, temp->burst_time);
        temp = temp->next;
        counter+=1;
     }
     printf("\nTotal CPU Burst Time : [%lf]", total_time);
     printf("\nAverage CPU Turnaround Time : [%lf]", total_turnaround_time/(double)num_processes);
     printf("\nAverage CPU Waiting Time : [%lf]\n", total_wait_time/((double)num_processes));
 }
