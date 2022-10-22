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
        queue->terminated=0;
        queue->time_spent=0;
        queue->num_bursts=0;

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
 */
 ReadyQueue *mergeQueue(ReadyQueue *first, ReadyQueue *second) {
     if (!first) return second;
     if (!second) return first;

     if (first->priority <= second->priority){
         first->next = mergeQueue(first->next, second);
         first->next->prev = first;
         first->prev = NULL;
         return first;
     }
     else {
         second->next = mergeQueue(first, second->next);
         second->next->prev = second;
         second->prev = NULL;
         return second;
     }

 }

/**
 * Sorts ready queue of PCB's using merge sort algorithm
 * @param (queue) : the ready queue to sort
 */
ReadyQueue *mergeSort(ReadyQueue *queue){

    if (!queue || !(queue->next)) return queue;

    ReadyQueue *second = divideQueue(queue);

    queue = mergeSort(queue);
    second = mergeSort(second);

    return mergeQueue(queue, second);
}

/**
 * Simple Priority Scheduler : execute processes from a ready-queue in order based on priority of processes
 * @param (queue) : ReadyQueue of processes to execute (sorted by priority value)
 */
void simplePriority(ReadyQueue *queue) {
    ReadyQueue *head = mergeSort(queue);

    printf("\n---------------------EXECUTING PROCESSES---------------------\n");

    while (head) {
        if (head->terminated == 0){
            pid_t pid = head->pcb->pid;

            printf("\nExecuting CPU burst on [%s] with PID = [%d]\n", head->pcb->path, pid);

            clock_t begin = clock();

            //Execute CPU burst
            kill(pid, SIGCONT);

            int status;
            waitpid(pid, &status, 0);

            clock_t end = clock();
            head->terminated = 1;

            head->time_spent = ((double) end - begin)/CLOCKS_PER_SEC;
            head->num_bursts = 1;
        }

        head=head->next;
    }

    printf("\n-------------------------FINISHED-------------------------\n");
}



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

    printf("\n---------------------EXECUTING PROCESSES---------------------\n");

    //iterate through the queue
    while (elem) {
        //check that process has not terminated
        if (elem->terminated == 0) {
            pid_t pid = elem->pcb->pid;

            printf("\nExecuting CPU burst on [%s] with PID = [%d]\n", elem->pcb->path, pid);

            clock_t begin = clock(); //get start time of process

            //Execute CPU burst on process
            kill(pid, SIGCONT); //resume (start) process
            usleep(time_quantum);//allow process to execute for time quantum
            kill(pid, SIGSTOP); //stop process

            clock_t end = clock();//get end time of process

            double burst = ((double)(end - begin))/CLOCKS_PER_SEC;//time spent on cpu burst

            elem->time_spent+=burst;
            elem->num_bursts+=1;

            //Check if process has terminated
            int status;
            if (waitpid(pid, &status, WNOHANG) != 0){ //use WNOHANG to prevent suspention or waiting
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
 * Prints the scheduling info for all processes in ReadyQueue after being executed
 * info : (path to program, PID, number of CPU bursts, time spent)
 * @param (queue) : queue of completed processes
 * @param (num_elements) : the number of elements in readyqueue
 */
 void printDetails(ReadyQueue *queue, size_t num_processes) {
     ReadyQueue *temp = queue;
     size_t counter = 0;

     double total_time = 0;//total cpu time spent
     double avg_wait_time = 0;//average cpu waiting time

     printf("\n-------------------------DETAILS-------------------------\n");

     while (temp && counter < num_processes){
         total_time += temp->time_spent;

         if (counter != num_processes - 1) avg_wait_time += avg_wait_time + temp->time_spent;

         printf("\nProgram [%s] with PID=[%d] executed for [%d] CPU burst with total time = [%f] \n",
                temp->pcb->path, temp->pcb->pid, temp->num_bursts, temp->time_spent);
        temp = temp->next;
        counter+=1;
     }

     printf("\nTotal CPU Burst Time : [%f]\n", total_time);
     printf("\nAverage CPU Waiting Time : [%f]\n", avg_wait_time/(double)num_processes);
 }
