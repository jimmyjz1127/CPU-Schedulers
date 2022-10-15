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
 */
void freeStrArr(char **strArr){
    //free all elements of strArr
    for (int i = 0; *(strArr + i); i++){
        free(*(strArr + i));
    }
    //free the array itself (start)
    free(strArr);
}

int main(int argc, char **argv)
{
    char str[] = "2 ./printchars c 12 b";

    size_t size = 0;

    char **strArr = splitStr(str, ' ', &size);

    for (int i = 0; *(strArr + i); i++){
        printf("[%s] ", *(strArr+i));
    }


}//end main()
