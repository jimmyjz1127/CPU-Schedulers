#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sched.h"
#include "scheduler.h"

int main(int argc, char **argv)
{
    /**
     * Type of scheduling scheme to use (default is round robin)
     * type = 1 : round robin
     * type = 2 : priority scheduling
     */
    int type = 1;

    if (argc < 2) {
        printf("Usage : ./sched <config file> <mode>\n");
    }
    else if (argc == 3){
        type = atoi(argv[2]);
    }

    char* file_path = argv[1];

    size_t num_processes = 0;//number of processes to schedule

    PCB *pcb_list = createPCBList(file_path, NULL, &num_processes);//create linked list of PCBs

    ReadyQueue *ready_queue = createQueue(pcb_list);//create ready queue of processes (PCBs)

    if (type == 1){//round robin scheduling
        useconds_t time_quantum = 500000;//time quantum

        roundRobin(ready_queue, time_quantum, num_processes);//execute processes according to round robin scheduling
        printDetails(ready_queue, num_processes);//print runtime details

        freePCBList(pcb_list);
        freeQueue(ready_queue, num_processes);
    }
    else if (type == 2) {//priority scheduling
        simplePriority(ready_queue);//execute processes according to priority scheduling
        printDetails(ready_queue, num_processes);//print runtime details

        freePCBList(pcb_list);
        freeQueue(ready_queue, num_processes);
    }
    else {
        printf("\nType of schedule indicated is not valid\n");
    }

}//end main()
