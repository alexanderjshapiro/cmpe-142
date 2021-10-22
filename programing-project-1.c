/*
 * Write a “C” program that creates a number of zombies (1 through n) processes as
 * specified in the command line arguments to the program: zombiefier –n 10 creates 10
 * zombies. Make sure the program cleans up the zombies upon completion. Zombies must
 * remain in the system until a signal (SIGCONT) is sent to zombifier.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

void signalHandler(int signalNumber) {
    // Do nothing
}

void main(int argc, char *argv[]) {
    int opt;
    int n = -1;

    // Get all arguments
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n': // -n <num>
                n = atoi(optarg);
                if (n > 0) break;
            default:
                // Usage message
                fprintf(stderr, "Usage: %s -n num\n", argv[0]);
                exit(1);
        }
    }

    // If no arguments were provided
    if (n < 1) {
        // Usage message
        fprintf(stderr, "Usage: %s -n num\n", argv[0]);
        exit(1);
    }

    // Start signal handler
    if(signal(SIGCONT, signalHandler) == SIG_ERR) {
        fprintf(stderr, "Error %d while registering signal handler: %s\n", errno, strerror(errno));
        exit(errno);
    }

    // Create array to hold child PIDs
    pid_t *pids = malloc(n * sizeof(pid_t));
    if (pids == NULL) {
        if (errno == ENOMEM) {
            fprintf(stderr, "Error %d while allocating memory: %s\n", errno, strerror(errno));
            exit(errno);
        } else {
            fprintf(stderr, "Error while allocating memory: Failed to allocate memory\n");
            exit(1);
        }
    }

    // Create n child processes
    for (int i = 0; i < n; i++) {
        pids[i] = fork();
        if (pids[i] > 0) {
            // Parent process
            // Do nothing
        } else if (pids[i] == 0) {
            // Child process
            // Exit immediately
            exit(0);
        } else {
            // Error while forking
            fprintf(stderr, "Error %d while forking: %s\n", errno, strerror(errno));
            exit(errno);
        }
    }

    if (pause() == -1) {
        // fprintf(stderr, "Error %d while pausing: %s\n", errno, strerror(errno));
        // exit(errno);
    }

    // Use "kill -s SIGCONT <pid>" to continue
    
    // Wait for each child
    for (int i = 0; i < n; i++) {
        int childStatus = 1;
        if(waitpid(pids[i], &childStatus, WNOHANG) == -1) {
            fprintf(stderr, "Error %d while waiting: %s\n", errno, strerror(errno));
            exit(errno);
        }

        if (childStatus != 0) {
            // Child exited with abnormal exit code
            fprintf(stderr, "Error while exiting child: %d\n", WEXITSTATUS(childStatus));
            exit(WEXITSTATUS(childStatus));
        }
    }

    free(pids);
    exit(0);
}
