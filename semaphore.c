//
// Created by Arash Khajelou on 2/28/24.
//

#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include "semaphore.h"
#include "logger.h"

// Utility function to initialize a semaphore
int initSemaphore(int semId, int initialValue) {
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;

    argument.val = initialValue;
    return semctl(semId, 0, SETVAL, argument);
}

// Create a semaphore for each house
int *createSemaphores(int n) {
    int *semIds = (int *) malloc(n * n * sizeof(int));
    for (int i = 0; i < n * n; i++) {
        semIds[i] = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
        if (semIds[i] == -1) {
            char *error = malloc(100 * sizeof(char));;
            sprintf(error, "semget: semget failed for semaphore %d\n", i);
            logError(error);
            exit(1);
        }
        initSemaphore(semIds[i], 1); // Initialize semaphore to 1 (available)
    }
    return semIds;
}

// Function to decrement (lock) a semaphore
void lockHouse(int houseIndex) {
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = -1; // Decrement the semaphore to lock
    operation.sem_flg = 0;

    if (semop(houseIndex, &operation, 1) == -1) {
        char *error = malloc(100 * sizeof(char));;
        sprintf(error, "semop: lock failed for semaphore %d\n", houseIndex);
        logError(error);
        exit(1);
    }
}

// Function to increment (unlock) a semaphore
void unlockHouse(int houseIndex) {
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = 1; // Increment the semaphore to unlock
    operation.sem_flg = 0;

    if (semop(houseIndex, &operation, 1) == -1) {
        char *error = malloc(100 * sizeof(char));;
        sprintf(error, "semop: unlock failed for semaphore %d\n", houseIndex);
        logError(error);
        exit(1);
    }
}

// Cleanup semaphores
void cleanupSemaphores(int *semIds, int n) {
    for (int i = 0; i < n * n; i++) {
        if (semctl(semIds[i], 0, IPC_RMID) == -1) {
            char *error = malloc(100 * sizeof(char));;
            sprintf(error, "semctl: cleanup failed for semaphore %d\n", i);
            logError(error);
        }
    }
    free(semIds);
}