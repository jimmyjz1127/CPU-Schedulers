#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sched.h"
#include "scheduler.h"

int main(int argc, char **argv)
{

    if (argc < 2) {
        printf("Usage : ./sched <config file> \n");
    }
    else {
        char* file_path = argv[1];

        size_t num_processes = 0;//number of processes to schedule

        PCB *pcb_list = createPCBList(file_path, NULL, &num_processes);

        ReadyQueue *ready_queue = createQueue(pcb_list);

        roundRobin(ready_queue, 500000, num_processes);

        printDetails(ready_queue, num_processes);

        // freeQueue(ready_queue);
        //
        // freePCBList(pcb_list);
    }

}//end main()
