//
// Created by Arash Khajelou on 2/28/24.
//

#ifndef FIRST_CODE_SEMAPHORE_H
#define FIRST_CODE_SEMAPHORE_H

void cleanupSemaphores(int *semIds, int n);

void unlockHouse(int houseIndex);

void lockHouse(int houseIndex);

int *createSemaphores(int n);

int initSemaphore(int semId, int initialValue);

#endif //FIRST_CODE_SEMAPHORE_H
