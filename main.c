/* 
 * File:   main.c
 * Author: Matthias Drummer <s0542834>
 *
 * Created on 23. Januar 2014, 20:34
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

short forks[3]; // 0 means false, 1 means true. This array represents the forks
int waitingLeft[3];
int waitingRight[3];

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

    pthread_t philosoph1;
    pthread_t philosoph2;
    pthread_t philosoph3;
    
    pthread_create(&philosoph1, NULL, philosoph, (void *)0); // wir haben hier den pointer auf eine Funktion
    pthread_create(&philosoph2, NULL, philosoph, (void *)1); // wir haben hier den pointer auf eine Funktion
    pthread_create(&philosoph3, NULL, philosoph, (void *)2); // wir haben hier den pointer auf eine Funktion
    
    void *res; // void pointer für das result
    pthread_join($philosoph1, &res); // wartet auf das Ende eines Threads.
    
    pthread_exit(NULL);
    return (EXIT_SUCCESS);
}

void doDeadlock() {
    
}

void doStarvation() {
    
}

void doBoring() {
    
}

void *philosoph(void *id) {
    // do something.
    pthread_exit(NULL);
}
