/* 
 * File:   main.c
 * Author: Matthias Drummer <s0542834>
 *
 * Created on 23. Januar 2014, 20:34
 */

#include <stdio.h>
#include <stdlib.h>

short forks[3]; // 0 means false, 1 means true. This array represents the forks
int waitingLeft[];
int waitingRight[];

char deadlockMessage[] = {"The death of a clown."};
char starvationMessage[] = {"Starvation."};
char boringMessage[] = {"Boring"};

void doDeadlock();
void doStarvation();
void doBoring();

/*
 * The main function of the project.
 * 
 * Program with 4 threads, where 1 thread is for the observer and 3 threads for the philisophs themselves.
 * The fork array has to be checked with mutex variables.
 */
int main(int argc, char** argv) {

    return (EXIT_SUCCESS);
}

void doDeadlock() {
    
}

void doStarvation() {
    
}

void doBoring() {
    
}
