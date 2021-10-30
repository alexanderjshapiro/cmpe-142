/* Write a “C” program that creates a pipe and forks a child process. The parent then sends 
 * the following message on his side of the pipe “I am your daddy! and my name is <pid-of-
 * the-parent-process>\n”, the child receives this message and prints it to its stdout verbatim. 
 * The parent then blocks reading on the pipe from the child. The child writes back to its 
 * parent through its side of the pipe stating “Daddy, my name is <pid-of-the-child>”. The parent 
 * then writes the message received from the child to its stdout. Make sure the parent waits on 
 * the child exit as to not creating orphans or zombies.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    const int BUFFER_SIZE = 100;

    // Create the pipe
    int pipefd[2];
    if(pipe(pipefd) == -1) {
        fprintf(stderr, "Error %d while creating pipe: %s\n", errno, strerror(errno));
        exit(errno);
    }

    // Create the child process
    pid_t pid = fork();
    if(pid > 0) {
        // Parent process
        char buffer[BUFFER_SIZE];

        // Write to buffer
        int sprintfReturned = sprintf(buffer, "I am your daddy! and my name is %d", getpid());
        if (sprintfReturned < 0) {
            fprintf(stderr, "Error while preparing buffer: An output error occurred");
            exit(-1);
        } else if (sprintfReturned > BUFFER_SIZE - 1) {
            fprintf(stderr, "Warning while preparing buffer: Output was truncated");
        }

        // Write to pipe from buffer
        if(write(pipefd[1], buffer, BUFFER_SIZE) == -1) {
            fprintf(stderr, "Error %d while writing: %s\n", errno, strerror(errno));
            exit(errno);
        }
        sleep(1);

        // Read from pipe to buffer
        if(read(pipefd[0], buffer, BUFFER_SIZE) == -1) {
            fprintf(stderr, "Error %d while reading: %s\n", errno, strerror(errno));
            exit(errno);
        }
        printf("%s\n", buffer);

        int childStatus;
        if(wait(&childStatus) == -1) {
            fprintf(stderr, "Error %d while waiting: %s\n", errno, strerror(errno));
            exit(errno);
        }
    } else if(pid == 0) {
        // Child process
        char buffer[BUFFER_SIZE];

        // Read from pipe to buffer
        if(read(pipefd[0], buffer, BUFFER_SIZE) == -1) {
            fprintf(stderr, "Error %d while reading: %s\n", errno, strerror(errno));
            exit(errno);
        }
        printf("%s\n", buffer);

        // Write to buffer
        int sprintfReturned = sprintf(buffer, "Daddy, my name is %d", getpid());
        if (sprintfReturned < 0) {
            fprintf(stderr, "Error while preparing buffer: An output error occurred");
            exit(-1);
        } else if (sprintfReturned > BUFFER_SIZE - 1) {
            fprintf(stderr, "Warning while preparing buffer: Output was truncated");
        }

        // Write to pipe from buffer
        if(write(pipefd[1], buffer, BUFFER_SIZE) == -1) {
            fprintf(stderr, "Error %d while writing: %s\n", errno, strerror(errno));
            exit(errno);
        }
        exit(0);
    } else {
        // Error while forking
        fprintf(stderr, "Error %d while forking: %s\n", errno, strerror(errno));
        exit(errno);
    }

    exit(0);
}
