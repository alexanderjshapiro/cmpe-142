/* Write a “C” program that creates two threads thread 1 and thread 2. Thread 1 prints 
 * exactly the following text in one and only one line (including a new line at the end of the 
 * text): "thread 1: ping thread 2". Thread 1 then uses conditional variable functions to 
 * signal thread 2 and blocks on another conditional variable waiting to be signaled by 
 * thread 2. Thread 2, upon being unblocked by thread 1 signal will print exactly the 
 * following text: "thread 2: pong! thread 1 ping received" including a newline at the end. 
 * Thread 2 will then proceed to print "thread 2: ping thread 1" and signal thread 1, upon 
 * receiving this signal on its condition variable, thread 1 will print exactly: "thread 1: pong! 
 * thread 2 ping received". From now on the sequence will repeat until the user of the 
 * program issues Ctrl-C or send SIGINT to the process.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

int run = 1;
int thread = 0;

pthread_cond_t condition1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t condition2 = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void signalHandler(int signal) {
	if (signal = SIGINT) run = 0;
}

void *func(void *val) {
    while(run) {
        if (pthread_mutex_lock(&lock) != 0) {
            fprintf(stderr, "Error %d while acquiring lock: %s\n", errno, strerror(errno));
            exit(errno);
        }
        
        if(thread == 0) {
            // Thread 1
            printf("thread 1: ping thread 2\n");
            
            thread = 1;
            
            if (pthread_cond_signal(&condition2) != 0) {
                fprintf(stderr, "Error %d while signaling thread 2: %s\n", errno, strerror(errno));
                exit(errno);
            }

            if (pthread_cond_wait(&condition1, &lock) != 0) {
                fprintf(stderr, "Error %d while waiting for thread 2: %s\n", errno, strerror(errno));
                exit(errno);
            }
            
            printf("thread 1: pong! thread 2 ping received\n");
        } else {
            // Thread 2
            printf("thread 2: pong! thread 1 ping received\n");
            
            thread = 0;
            
            printf("thread 2: ping thread 1\n");
            
            if (pthread_cond_signal(&condition1) != 0) {
                fprintf(stderr, "Error %d while signaling thread 2: %s\n", errno, strerror(errno));
                exit(errno);
            }
        }

        if (pthread_mutex_unlock(&lock) != 0) {
            fprintf(stderr, "Error %d while releasing lock: %s\n", errno, strerror(errno));
            exit(errno);
        }
    }
    
    return NULL;
}

void main() {
    signal(SIGINT, signalHandler);
    
    pthread_t thread1, thread2;
	
    if (pthread_create(&thread1, NULL, func, NULL) != 0) {
        fprintf(stderr, "Error %d while creating thread 1: %s\n", errno, strerror(errno));
        exit(errno);
    }

    if (pthread_create(&thread2, NULL, func, NULL) != 0) {
        fprintf(stderr, "Error %d while creating thread 2: %s\n", errno, strerror(errno));
        exit(errno);
    }
    
    if (pthread_join(thread1, NULL) != 0) {
        fprintf(stderr, "Error %d while waiting for thread 1: %s\n", errno, strerror(errno));
        exit(errno);
    }

    if (pthread_join(thread2, NULL) != 0) {
        fprintf(stderr, "Error %d while creating thread 2: %s\n", errno, strerror(errno));
        exit(errno);
    }

	exit(0);
}
