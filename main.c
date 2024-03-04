#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

#include "world.h"
#include "semaphore.h"
#include "monitor.h"

int main() {
    int n, damagedHouses;
    printf("Please give us the world's N size: ");
    scanf("%d", &n);
    printf("Please input the count of repair needed houses: ");
    scanf("%d", &damagedHouses);

    int shmid = shmget(IPC_PRIVATE, sizeof(World), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    World *world = (World *) shmat(shmid, NULL, 0);
    if (world == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    initWorld(world, n, damagedHouses);

    int *semIds = createSemaphores(n);
    for (int i = 0; i < 4; i++) {
        startRepairProcess(world, i, semIds);
    }

    initializeMonitor();
    monitorMainLoop(world);

    for (int i = 0; i < 5; i++) {
        wait(NULL); // Wait for all worker processes to finish
    }

    cleanupMonitor();
    printFinalResults(world);
    shmctl(shmid, IPC_RMID, NULL); // Cleanup shared memory
    cleanupSemaphores(semIds, n);
    return 0;
}
