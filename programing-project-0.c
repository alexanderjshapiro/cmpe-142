/*
 * Write a "C" program that creates a child process that prints out its pid and new line and
 * then calls pause() system call. Once it returns from the pause system call the child
 * program must exit with exit code 5. The parent program must wait for the child process
 * and print out the pid of the child process and the exit status with the following format:
 * "childpid=%d,exitstatus=%d\n". Do not print anything else to stdout.
 * Hint: make sure that the child handles SIGINT signal. Make sure that the exit code of the
 * child process is 5 and not any other number.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void signalHandler(int signalNumber){
    // Do something
}

void main() {
    pid_t pid = fork();                         // Create the child process
    if (pid < 0) {                              // Error while forking
        exit(-1);
    } else if (pid == 0) {                      // Child process
        signal(SIGINT, signalHandler);          // Start signal handler listener
        printf("%d\n", getpid());               // Print the child process PID
        pause();                                // Pause the child process (resume with kill -s SIGINT <PID>)
        exit(5);                                // Exit with exit code 5
    } else {                                    // Child process
        int childStatus = 1;
        pid = wait(&childStatus);               // Wait for the child process to stop running 
        printf("childpid=%d,exitstatus=%d\n",   // Print the PID and exit status of the child process
            pid,
            WEXITSTATUS(childStatus)
        );
        exit(0);                                // Exit sucessfully
    }
}
