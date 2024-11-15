#include "sched.h"

/**
 * Splits string into array using spaces as delimeter
 * @param (str) : the string to split
 * @param (size) : size attribute. Used by calling functions to determine size of string array produced
 * @return : the array of strings
 */
char **splitStr(char *str, size_t *size){
    char delim[] = " ";//set delimeter as white space

    //count number of elements in line
    int count = 1;
    int i = 0;
    while (str[i]){
        if (str[i] == delim[0]) count++;
        i++;
    }
    i = 0;

    char **result = malloc((sizeof(char *)) * count);//the array of strings to return

    char *token = strtok(str, delim);

    while (token){
        result[i] = strdup(token);
        token = strtok(NULL, delim);
        i++;
    }
    *size = i;
    return result;
}

/**
 * Frees a given string array
 * @param (strArr) : the string array to free
 * @param (num_elements) : the number of elements in string array
 */
void freeStrArr(char **strArr, size_t num_elements){
    //free all elements of strArr
    for (size_t i = 0; i < num_elements; i++){
        free(strArr[i]);
    }
    //free the array itself (start)
    free(strArr);
}

/**
 * Creates a new PCB for a process
 * @param (path) : path to program to execute
 * @param (priority) : priority of program to execute
 * @param (pid) : process ID of program to execute
 * @param (size) : the size of the process (for shortest-job-first scheduler)
 * @param (prev) : previous PCB in PCB list
 * @param (next) : next PCB in PCB List
 * @return : PCB object
 */
PCB *createPCB(char *path, int priority, pid_t pid, int size, PCB *prev, PCB *next){
    PCB *process = malloc(sizeof(PCB));

    process->path = strdup(path);
    process->priority = priority;
    process->pid = pid;
    process->size = size;
    process->prev = prev;
    process->next = next;

    return process;
}

/**
 * Creates PCB for a child process containing relevant information (PID, priority, index, prev, next)
 * @param (config_file) : the path to config_file containing programs to be executed through scheduling scheme
 * @param (pcb_list) : queue of PCBs to populate
 * @param (num_processes) : number of processes to schedule
 * @return : head to linked list of PCB objects
 */
PCB *createPCBList(char *config_file, PCB *pcb_list, size_t *num_processes) {
    //open config file with read permissions
    FILE *fp = fopen(config_file, "r");

    //check if opening file was successful
    if (fp == NULL) {
        perror("ERROR : cannot read from file");
        return NULL; //NOTE : consider changing to return NULL
    }

    char *line = NULL;
    size_t length = 0;
    int read;

    //read each line from file
    while ((read = getline(&line, &length, fp)) != -1){
        size_t size = 0;

        char **strArr = splitStr(line, &size);//split line by white space

        /* Create array for program arguments in line read*/
        char **args = malloc(sizeof(char *) * size);

        for (int i = 1; (size_t) i < size; i++){
          *(args+i-1) = (char *) malloc(strlen(strArr[i]) + 1);
          strcpy(*(args+i-1), strArr[i]);
        }

        //create child process
        pid_t pid = fork();

        if (pid < 0) fprintf(stderr, "Failure to execute process [%d] [%s]\n", pid, strArr[1]);
        else if (pid > 0){ //parent process
            kill(pid, SIGSTOP); //stop the child process

            //for shortest job first scheduling
            int process_size = 0;
            if (strcmp(strArr[1], "./printchars") == 0){
                process_size = atoi(strArr[3]);
            }

            //Create PCB for process
            PCB *process = createPCB(strArr[1], atoi(strArr[0]), pid, process_size, NULL, pcb_list);

            if (pcb_list) {
                pcb_list->prev = process;
            }
            pcb_list = process;
        }
        else {//child process
            //Make system call to execute program from child process
            execv(strArr[1], args);
        }

        //free memory used for args string array
        for (int i = 0; i < (int)size; i++){
           free(*(args + i));
        }
        free(args);

        //free memory used for string array of line read from file
        freeStrArr(strArr, size);

        //count number of processes
        *num_processes+=1;

    }//end while (line iteration)

    fclose(fp);
    return pcb_list;

}//end createPCBList



/**
 * Frees memory of PCB list
 * @param (pcb_list) : the list of PCBs to free the memory of
 */
void freePCBList(PCB *pcb_list){
    while (pcb_list) {
        PCB *next = pcb_list->next;
        free(pcb_list->path);

        kill(pcb_list->pid, SIGTERM);//terminate the process completely


        free(pcb_list);
        pcb_list = next;
    }
}//end freePCBList
