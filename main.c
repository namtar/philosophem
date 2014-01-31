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

short forks[3]; // 0 means false, 1 means true. This array represents the forks
int waitingLeft[3];
int waitingRight[3];
short doRun = 0; // if set to 1 the philosophs shall start.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char deadlockMessage[] = {"The death of a clown."};
char starvationMessage[] = {"Starvation."};
char boringMessage[] = {"Boring"};

void doDeadlock();
void doStarvation();
void doBoring();

void *philosoph(void *id);

/*
 * The main function of the project.
 * 
 * Program with 4 threads, where 1 thread is for the observer and 3 threads for the philisophs themselves.
 * The fork array has to be checked with mutex variables.
 * Damit das funktioniert muss dem compiler die Anweisung -pthread übergeben werden.
 */
int main(int argc, char** argv) {

    printf("Start Program\n");
    doDeadlock();
    //    doStarvation();
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

    printf("Created Threads\n");
    sleep(5); // wait 5 seconds before stsarting
    printf("Start Run\n");
    doRun = 1; // give signal to threads to start eating.

    int run = 0;
    while (run == 0) {
        sleep(1); // sleep one second and then check for deadlock.

        printf("Values of waitingLeft: %i, %i, %i\n", waitingLeft[0], waitingLeft[1], waitingLeft[2]);
        printf("Values of waitingRight: %i, %i, %i\n", waitingRight[0], waitingRight[1], waitingRight[2]);
        
        if (waitingLeft[0] == 0 && waitingLeft[1] == 0 && waitingLeft[2] == 0 && waitingRight[0] == 1 && waitingRight[1] == 1 && waitingRight[2] == 1) {
            printf("%s", deadlockMessage);
            break;
        } else if (waitingLeft[0] == 0 && waitingLeft[1] == 0 && waitingLeft[2] == 0 && waitingRight[0] == 1 && waitingRight[1] == 1 && waitingRight[2] == 1) {
            printf("%s", deadlockMessage);
            break;
        }

    }

    void *res; // void pointer für das result
    pthread_join(philosoph1, &res); // wartet auf das Ende eines Threads.
    pthread_join(philosoph2, &res); // wartet auf das Ende eines Threads.
    pthread_join(philosoph3, &res); // wartet auf das Ende eines Threads.

    pthread_exit(NULL);
}

void doStarvation() {

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
    int leftForkIndex = (int *) id; // left fork index equals thread id.
    int rightForkIndex = (leftForkIndex + 1) % 3;

    pthread_yield(NULL);

    while (doRun == 0) {
        // busy waiting.
        printf("BusyWaiting\n");
        sleep(1); // wait one second??
    }
    printf("Thread runs: Id: %i\n", leftForkIndex);
    // now logic after run command has been given

    pthread_mutex_lock(&mutex);
    // take left fork
    waitingLeft[leftForkIndex] = 1;
    while (forks[leftForkIndex] == 1) {
        // it is not possible to take the left fork. Wait a specified time and try again.
        sleep(1);
    }

    forks[leftForkIndex] = 1;
    waitingLeft[leftForkIndex] = 0;
    pthread_yield(NULL); // nach dem erfolgreichen aufnehmen einer Gabel, wird die CPU abgegeben.

    // take right fork if possible
    waitingRight[leftForkIndex] = 1; // left fork index is here used as thread id.
    while (forks[rightForkIndex] == 1) {
        // it is not possible to take the fork. Wait a specified time and try again.
        printf("Wait for right fork: ThreadId: %i", leftForkIndex);
        sleep(1);
    }
    forks[rightForkIndex] = 1;
    waitingRight[leftForkIndex] = 0;

    // eat and think
    sleep(1); // 1 Sekunde

    // release forks
    forks[leftForkIndex] = 0;
    forks[rightForkIndex] = 0;

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}
