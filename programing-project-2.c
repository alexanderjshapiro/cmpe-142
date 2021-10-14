/* Write a “C” program for Linux that runs an infinite loop sleeping for intervals of 2 
 * seconds. The program starts by printing nothing but if the signal SIGINT is sent to the 
 * program the program writes a short statement with the number of iteration it is executing 
 * (toggle debug on). The program remains printing until another SIGINT is sent to the 
 * process, at that point the print statements are suppressed until it receives a new SIGINT 
 * signal. If the program receives the SIGUSR1 signal it must terminate gracefully returning 
 * the value 0 as exit status. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int debug = 0;

void signalHandler(int signal) {
    if (signal == SIGINT) debug = !debug;               // Toggle debug flag
    else if (signal == SIGUSR1) exit(0);                // Exit with status 0
}

void main() {
    signal(SIGINT, signalHandler);                      // Register signal handlers
    signal(SIGUSR1, signalHandler);
    
    for (int itr = 0; 1; itr++) {
        sleep(2);                                       // Sleep for 2 seconds
        if (debug) printf("iteration: %d\n", itr);      // Print iternation number if debug is on
    }
}
