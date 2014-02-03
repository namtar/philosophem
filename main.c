/* 
 * File:   main.c
 * Author: Matthias Drummer <s0542834>
 *
 * Created on 23. Januar 2014, 20:34
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

short forks[3]; // 0 means false, 1 means true. This array represents the forks
int waitingLeft[3];
int waitingRight[3];
short doRun = 0; // if set to 1 the philosophs shall start.
short threadsFinished[3]; // 1 means running, 0 means not running

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char deadlockMessage[] = {"The death of a clown."};
char starvationMessage[] = {"Starvation."};
char boringMessage[] = {"Boring"};

void doDeadlock();
void doStarvation();
void doBoring();

/**
 * Logic for deadlock philosophs.
 * 
 * @param id
 * @return 
 */
void *philosoph(void *id);

/**
 * Logic for starving philosophs.
 * 
 * @param id
 * @return 
 */
void *starvingPhilosoph(void *id);

/*
 * The main function of the project.
 * 
 * Program with 4 threads, where 1 thread is for the observer and 3 threads for the philisophs themselves.
 * The fork array has to be checked with mutex variables.
 * Damit das funktioniert muss dem compiler die Anweisung -pthread übergeben werden.
 */
int main(int argc, char** argv) {

    printf("Start Program\n");
    //    doDeadlock();
    doStarvation();
    //    doBoring();

    return (EXIT_SUCCESS);
}

void doDeadlock() {

    pthread_t philosoph1;
    pthread_t philosoph2;
    pthread_t philosoph3;

    pthread_create(&philosoph1, NULL, philosoph, (void *) 0); // wir haben hier den pointer auf eine Funktion
    pthread_create(&philosoph2, NULL, philosoph, (void *) 1); // wir haben hier den pointer auf eine Funktion
    pthread_create(&philosoph3, NULL, philosoph, (void *) 2); // wir haben hier den pointer auf eine Funktion

    threadsFinished[0] = 1;
    threadsFinished[1] = 1;
    threadsFinished[2] = 1;

    printf("Created Threads\n");
    sleep(2); // wait 2 seconds before stsarting
    printf("Start Run\n");
    doRun = 1; // give signal to threads to start eating.

    int run = 0;
    while (run == 0) {
        sleep(1); // sleep one second and then check for deadlock.

        printf("Values of waitingLeft: %i, %i, %i\n", waitingLeft[0], waitingLeft[1], waitingLeft[2]);
        printf("Values of waitingRight: %i, %i, %i\n", waitingRight[0], waitingRight[1], waitingRight[2]);
        printf("Values of Forks: %i, %i, %i\n", forks[0], forks[1], forks[2]);

        if (waitingLeft[0] == 0 && waitingLeft[1] == 0 && waitingLeft[2] == 0 && waitingRight[0] == 1 && waitingRight[1] == 1 && waitingRight[2] == 1) {
            printf("%s\n", deadlockMessage);
            pthread_cancel(philosoph1);
            pthread_cancel(philosoph2);
            pthread_cancel(philosoph3);
            break;
        } else if (waitingLeft[0] == 1 && waitingLeft[1] == 1 && waitingLeft[2] == 1 && waitingRight[0] == 0 && waitingRight[1] == 0 && waitingRight[2] == 0) {
            printf("%s\n", deadlockMessage);
            pthread_cancel(philosoph1);
            pthread_cancel(philosoph2);
            pthread_cancel(philosoph3);
            break;
        }

        if (threadsFinished[0] == 0 && threadsFinished[1] == 0 && threadsFinished[2] == 0) {
            // all threads are finished exit loop.
            run = 1;
        }
    }

    pthread_exit(NULL);
}

void doStarvation() {

    pthread_t philosoph1;
    pthread_t philosoph2;
    pthread_t philosoph3;

    pthread_create(&philosoph1, NULL, starvingPhilosoph, (void *) 0); // wir haben hier den pointer auf eine Funktion
    pthread_create(&philosoph2, NULL, starvingPhilosoph, (void *) 1); // wir haben hier den pointer auf eine Funktion
    pthread_create(&philosoph3, NULL, starvingPhilosoph, (void *) 2); // wir haben hier den pointer auf eine Funktion

    threadsFinished[0] = 1;
    threadsFinished[1] = 1;
    threadsFinished[2] = 1;

    printf("Created Threads\n");
    sleep(2); // wait 2 seconds before stsarting
    printf("Start Run\n");
    doRun = 1; // give signal to threads to start eating.

    int run = 0;
    while (run == 0) {
        sleep(1); // wait one second and then check

        printf("Values of waitingLeft: %i, %i, %i\n", waitingLeft[0], waitingLeft[1], waitingLeft[2]);
        printf("Values of waitingRight: %i, %i, %i\n", waitingRight[0], waitingRight[1], waitingRight[2]);
        printf("Values of Forks: %i, %i, %i\n", forks[0], forks[1], forks[2]);
        if (waitingLeft[0] > 15 || waitingLeft[1] > 15 || waitingLeft[2] > 15 || waitingRight[0] > 15 || waitingRight[1] > 15 || waitingRight[2] > 15) {
            printf("%s\n", starvationMessage);
            pthread_cancel(philosoph1);
            pthread_cancel(philosoph2);
            pthread_cancel(philosoph3);
            break;
        }

    }

    pthread_exit(NULL);
}

void doBoring() {

    pthread_t philosoph1;
    pthread_t philosoph2;
    pthread_t philosoph3;

    pthread_create(&philosoph1, NULL, philosoph, (void *) 0); // wir haben hier den pointer auf eine Funktion
    pthread_create(&philosoph2, NULL, philosoph, (void *) 1); // wir haben hier den pointer auf eine Funktion
    pthread_create(&philosoph3, NULL, philosoph, (void *) 2); // wir haben hier den pointer auf eine Funktion

    void *res; // void pointer für das result
    pthread_join(philosoph1, &res); // wartet auf das Ende eines Threads.

    pthread_exit(NULL);
}

void *philosoph(void *id) {
    // do something.
    int leftForkIndex = (int) ((int *) id); // left fork index equals thread id.
    int threadId = leftForkIndex;
    int rightForkIndex = (leftForkIndex + 1) % 3;
    int trueVal = 0;
    printf("ThreadId. %i, RightForkIndex: %i\n", threadId, rightForkIndex);

    // thread config
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    pthread_yield(NULL);

    while (doRun == 0) {
        // busy waiting.
        printf("BusyWaiting\n");
        sleep(1); // wait one second??
    }
    // now logic after run command has been given
    waitingLeft[threadId] = 1;
    waitingRight[threadId] = 1;

    // take left fork
    while (trueVal == 0) {

        pthread_mutex_lock(&mutex); // critical path
        if (forks[leftForkIndex] == 0) {
            // take left fork
            printf("Take left fork. Id: %i\n", threadId);
            forks[leftForkIndex] = 1;
            waitingLeft[threadId] = 0;
        }
        pthread_mutex_unlock(&mutex);
    }
    trueVal = 0; // reset variable for next loop.

    pthread_yield(NULL); // nach dem erfolgreichen aufnehmen einer Gabel, wird die CPU abgegeben.

    //    sleep(0.05);

    // take right fork if possible    
    while (trueVal == 0) {
        pthread_mutex_lock(&mutex);
        // it is not possible to take the fork. Wait a specified time and try again.
        if (forks[rightForkIndex] == 0) {
            // take fork
            printf("Take right fork. Id: %i\n", threadId);
            forks[rightForkIndex] = 1;
            // and leave loop
            waitingRight[leftForkIndex] = 0;
            trueVal = 1;
        }
        pthread_mutex_unlock(&mutex);
        pthread_yield(NULL);
    }
    printf("Eat and Think: Id: %i\n", threadId);
    sleep(3);
    pthread_mutex_lock(&mutex);
    // release forks after successful eating
    forks[leftForkIndex] = 0;
    forks[rightForkIndex] = 0;
    pthread_mutex_unlock(&mutex); // leave critical path

    pthread_yield(NULL);


    threadsFinished[threadId] = 0;

    pthread_exit(NULL);
}

void *starvingPhilosoph(void* id) {

    int leftForkIndex = (int) ((int *) id); // left fork index equals thread id.
    int threadId = leftForkIndex;
    int rightForkIndex = (leftForkIndex + 1) % 3;
    int trueVal = 0;
    printf("ThreadId. %i, RightForkIndex: %i\n", threadId, rightForkIndex);

    // thread config
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    pthread_yield(NULL);

    while (doRun == 0) {
        // busy waiting.
        printf("BusyWaiting\n");
        sleep(1); // wait one second??
    }

    // enter infinite loop.
    int infinite = 0;
    while (infinite == 0) {
        trueVal = 0;
        // take left fork        
        while (trueVal == 0) {

            if (forks[leftForkIndex] == 1) {
                break;
            }
            waitingLeft[threadId]++;
            //            printf("Incremented Waiting Left: ThreadId: %i, %i\n", waitingLeft[threadId], threadId);
            if (pthread_mutex_trylock(&mutex) == EBUSY) {
                // wait some time and try again. Only proceed if left fork has been taken successfully
                sleep(0.5);
            } else {
                forks[leftForkIndex] = 1;
                trueVal = 1;
                pthread_mutex_unlock(&mutex);
            }
        }

//        pthread_yield(NULL);

        // reset trueVal
        trueVal = 0;

        // take right fork if possible, otherwise drop left fork, wait and try again.
        waitingRight[threadId]++;
        if (pthread_mutex_trylock(&mutex) == EBUSY) {
            // drop left fork
            while (trueVal == 0) {

                //  printf("Incremented Waiting Right: ThreadId: %i, %i\n", waitingRight[threadId], threadId);
                if (pthread_mutex_trylock(&mutex) == EBUSY) {
                    // wait some time and try again.
                    sleep(0.5);
                } else {
                    forks[leftForkIndex] = 0;
                    trueVal = 1;
                    pthread_mutex_unlock(&mutex);
                    pthread_yield(NULL);
                }
            }
        } else {
            // take right fork and eat
            forks[rightForkIndex] = 1;
            sleep(3); // eat 2 seconds
            pthread_mutex_unlock(&mutex);
            pthread_yield(NULL);
            // drop left and right fork
            trueVal = 0;
            while (trueVal == 0) {
                if (pthread_mutex_trylock(&mutex) == EBUSY) {
                    // sleep and true again
                    sleep(1);
                } else {
                    forks[leftForkIndex] = 0;
                    forks[rightForkIndex] = 0;
                    waitingLeft[threadId] = 0;
                    waitingRight[threadId] = 0;
                    trueVal = 1;
                    pthread_mutex_unlock(&mutex);
                    pthread_yield(NULL);
                }
            }
        }
        pthread_yield(NULL);

        // think
//        sleep(2);
    }

    pthread_exit(NULL);
}



