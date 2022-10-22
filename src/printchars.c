// This is a simple loop which will print some chars
// It is a useful way to test that the scheduler is working

#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * Usage : takes two arguments (m,n)
 * @arg (m) : the char to print
 * @arg (n) : the number of char (m) to print
 * If args are not specified, printchar will by default print "*" 30 times 
 */
int main(int argc, char** argv)
{
    char c;
    int n;

    if(argc < 2){
        c = '*';
        n = 30;
    }
    else if (argc < 3){
        c = *argv[1];
        n = 30;
    }
    else{
        c = *argv[1];
        n = atoi(argv[2]);
    }

    for(int i=0; i<n; i++)
    {
        printf("%c",c);
        fflush(stdout);
        usleep(100000);
    }

    return 0;
}
