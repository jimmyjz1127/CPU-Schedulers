// CS3104 P1 starter code
// kt54, Oct 2018
//
// This program will start two processes and then toggle them using
// STOP and CONT signals

#include "sched.h"

/**
 * Splits string according to provided delimeter
 * @param (str) : the string the split
 * @param (delimeter) : the delimeter to split the string by
 * @param (size) : size attribute. Used by calling functions to determine size of string array produced
 * source : https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
 */
char **splitStr(char *str, const char delimeter, size_t *size) {
    char** result = 0; //array of strings to return
    size_t count = 0;
    char* temp = str;
    char* last_delimeter = 0;

    char delim[2];
    delim[0] = delimeter;
    delim[1] = '\0';

    //count number of elements to extract
    while (*temp) {
        if (delimeter == *temp){
            count++;
            last_delimeter = temp;
        }
        temp++;
    }
    //Add space for trailing token
    count += last_delimeter < (str + strlen(str) - 1);

    //Add space for terminating null string so caller knows where the list of returned strings end
    count+=1;

    //allocate memory for result
    result = malloc(sizeof(char*) * count);

    //index
    size_t idx = 0;

    //if memory was successfully allocated
    if (result) {
        char* token = strtok(str, delim);//retrieve first token from string

        //iterate through rest of tokens
        while (token) {
            *(result + idx++) = strdup(token);//add token to result array
            token = strtok(0, delim);//retrieve next token
        }
        *(result + idx) = 0;
    }
    //set size attribute to number of elements in produced string array
    *size = idx;

    return result;

}//end splitStr()

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
 * @param (index) : index within PCB List
 * @param (prev) : previous PCB in PCB list
 * @param (next) : next PCB in PCB List
 */
PCB *createPCB(char *path, int priority, pid_t pid, int index, PCB *prev, PCB *next){
    PCB *process = malloc(sizeof(PCB));

    process->path = strdup(path);
    process->priority = priority;
    process->pid = pid;
    process->index = index;
    process->prev = prev;
    process->next = next;

    return process;
}

/**
 * Creates PCB for a child process containing relevant information (PID, priority, index, prev, next)
 * @param (*config_file) : the path to config_file containing programs to be executed through scheduling scheme
 * @param (*pcbQueue) : queue of PCBs to populate
 * @param (num_processes) : number of processes to schedule
 */
PCB *createPCBList(char *config_file, PCB *pcb_list, size_t *num_processes) {
    //open config file with read permissions
    FILE *fp = fopen(config_file, "r");

    //check if opening file was successful
    if (fp == NULL) {
        perror("ERROR : cannot read from file");
        exit(NULL); //NOTE : consider changing to return NULL
    }

    char *line = NULL;
    size_t len = 0;
    size_t read;
    int index = 0;//index of PCBs (NOTE: maybe pass as parameter)

    //read each line from file
    while ((read = getline(&line, &len, fp)) != -1){
        size_t size = 0;

        char **strArr = splitStr(line, ' ', &size);//split line by white space

        /* Create array for program arguments in line read*/
        char **args = malloc(sizeof(char *) * (size - 1));
        char **elem = args;//for iterating while maintaining head pointer of args

        for (int i = 1; (size_t) i < size; i++){
            *elem = (char *) malloc(strlen(strArr[i]));
            strcpy(*elem, strArr[i]);

            elem+=1;
        }

        *elem = NULL;

        //create child process
        pid_t pid = fork();

        if (pid < 0) fprintf(stderr, "Failure to execute process [%d] [%s]\n", pid, strArr[1]);
        else if (pid > 0){ //parent process
            kill(pid, SIGSTOP); //stop the child process

            //Create PCB for process
            PCB *process = createPCB(strArr[1], atoi(strArr[0]), pid, index, NULL, pcb_list);
            index += 1;

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
        freeStrArr(args, size-2);
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
        // free(pcb_list->path);

        kill(pcb_list->pid, SIGTERM);//terminate the process completely


        free(pcb_list);
        pcb_list = next;
    }
}//end freePCBList
