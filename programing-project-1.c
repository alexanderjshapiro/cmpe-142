/*
 * Write a “C” program that creates a number of zombies (1 through n) processes as
 * specified in the command line arguments to the program: zombiefier –n 10 creates 10
 * zombies. Make sure the program cleans up the zombies upon completion. Zombies must
 * remain in the system until a signal (SIGCONT) is sent to zombifier.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>

void signalHandler(int signalNumber) {
    // Do nothing
}

void main(int argc, char *argv[]) {
    int opt, n;
    while ((opt = getopt(argc, argv, "n:")) != -1) {                    // Get arguments until there are no more
        switch (opt) {                                                  // Choose action depending on argument
            case 'n':                                                   // -n <num>
                n = atoi(optarg);                                       // Convert num to integer and save to n
                if (n > 0) break;                                       // If num is 0 or not an integer
            default:                                                    // If arguments were incorrect
                fprintf(stderr, "Usage: %s -n <num>\n", argv[0]);       // Usage message
                exit(1);
        }
    }

    signal(SIGCONT, signalHandler);                                     // Start signal handler listener in parent

    pid_t *pids = malloc(n * sizeof(pid_t));
    for (int i = 0; i < n; i++) {                                       // For n
        pids[i] = fork();                                               // Create the child process
        if (pids[i] < 0) {                                              // If there was an error while forking
            exit(1);
        } else if (pids[i] == 0) {                                      // If inside the child process
            exit(0);                                                    // Exit immediately
        } else if (pids[i] > 0) {                                       // If inside the parent process
            // Do nothing
        } else {                                                        // If fork returned an unknown value
            exit(-1);
        }
    }
    pause();                                                            // Pause before waiting to create zombies
    // Use "kill -s SIGCONT <pid>" to continue
    for (int i = 0; i < n; i++) {                                       // For each child process
        int childStatus = 1;
        waitpid(pids[i], &childStatus, WNOHANG);                        // Wait for the child process to stop running
        if (childStatus != 0) {                                         // If child exited with abnormal exit code
            fprintf(stderr, "Code %d\n", WEXITSTATUS(childStatus));
        }
    }
    exit(0);                                                            // Exit sucessfully
}
