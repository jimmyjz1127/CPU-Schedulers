#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sched.h"
#include "scheduler.h"

int main(int argc, char **argv)
{

    int type = 1;

    if (argc < 2) {
        printf("Usage : ./sched <config file> \n");
    }
    else if (argc == 3){
        type = atoi(argv[2]);
    }

    char* file_path = argv[1];

    size_t num_processes = 0;//number of processes to schedule

    PCB *pcb_list = createPCBList(file_path, NULL, &num_processes);

    ReadyQueue *ready_queue = createQueue(pcb_list);

    if (type == 1){
        roundRobin(ready_queue, 500000, num_processes);
        printDetails(ready_queue, num_processes);

        freePCBList(pcb_list);
        freeQueue(ready_queue, num_processes);
    }
    else if (type == 2) {
        simplePriority(ready_queue);
        printDetails(ready_queue, num_processes);

        freePCBList(pcb_list);
        freeQueue(ready_queue, num_processes);
    }
    else {
        printf("\nType of schedule indicated is not valid\n");
    }

}//end main()
