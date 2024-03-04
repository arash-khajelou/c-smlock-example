//
// Created by Arash Khajelou on 2/27/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include "world.h"
#include "semaphore.h"
#include "logger.h"


void initWorld(World *world, int n, int damagedHouses) {
    world->n = n;
    world->totalDamaged = damagedHouses;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            initHouse(&world->houses[i][j], i, j);
        }
    }

    // Randomly distribute damaged houses
    srand(time(NULL));
    for (int i = 0; i < damagedHouses; i++) {
        int x, y;
        do {
            x = rand() % n;
            y = rand() % n;
        } while (world->houses[x][y].isDamaged);
        world->houses[x][y].isDamaged = true;
    }

    // Randomly position workers
    for (int i = 0; i < 4; i++) {
        initWorker(&world->workers[i], rand() % n, rand() % n, i);
    }
}

void workerLogic(World *world, int workerId, int *semIds) {
    Worker *worker = &world->workers[workerId];
    Point *position = &worker->position;

    char *message = malloc(100 * sizeof(char));
    sprintf(message, "Worker %d: Started repairing.\n", workerId);
    logInfo(message);


    while (true) {

        if (worker->repairedHouses >= world->totalDamaged) {
            break;
        }

        if (!moveWorker(world, workerId)) {
            break;
        }

        House *currentHouse = &world->houses[position->x][position->y];

        int semIndex = position->x * world->n + position->y; // Calculate semaphore index for the next house

        if (beenInTheHouseBefore(workerId, currentHouse)) {
            continue;
        }

        lockHouse(semIds[semIndex]);     // Lock the house semaphore before repairing

        if (currentHouse->noteCount == 0) {

            // Try to repair
            if (currentHouse->isDamaged) {
                currentHouse->isDamaged = false;

                worker->repairedHouses++;
            }

        }

        char *newNote = malloc(MAX_NOTE_LENGTH * sizeof(char));
        sprintf(newNote, "%d repaired %d times.\n", workerId, worker->repairedHouses);
        addNoteToHouse(currentHouse, newNote);
        unlockHouse(semIds[semIndex]); // Unlock the house semaphore after repairing

        usleep(600000);
    }

    printf("Worker %d finished repairing %d houses.\n", workerId, worker->repairedHouses);
}

void startRepairProcess(World *world, int workerId, int *semIds) {
    if (fork() == 0) { // Child process
        workerLogic(world, workerId, semIds);
        exit(0); // Terminate child process
    }
}

bool moveWorker(World *world, int workerId) {
    Worker *worker = &world->workers[workerId];
    Point *point = &worker->position;
    Zone workerZone = getWorkerZone(world, workerId);

    if (!worker->zoneTraverseStarted) {
        if (isPointOnTheCorner(workerZone, *point)) {
            worker->zoneTraverseStarted = true;

            worker->prevXMove = getStartingZoneXDirection(workerZone, *point);
            worker->prevYMove = getStartingZoneYDirection(workerZone, *point);
        } else {
            enum Direction nextMove = decideNextMoveToGetInTheNearestCorner(workerZone, *point);
            if (nextMove != NONE) {
                moveWorkerInDirection(world, workerId, nextMove);
            } else {
                char *message = malloc(100 * sizeof(char));;
                sprintf(message,
                        "Worker %d: Already on the corner. But the zoneTraverseStarted is false. (x: %d, y: %d)\n",
                        workerId, point->x, point->y);
                workerFacedError(worker, message);
                return false;
            }
        }
    }

    if (worker->zoneTraverseStarted) {
        enum Direction nextMove = decideNextMoveToTraverseTheZone(
                workerZone, *point, worker->prevXMove, worker->prevYMove
        );

        if (nextMove == LEFT || nextMove == RIGHT) {
            worker->prevXMove = nextMove;
        } else {
            worker->prevYMove = nextMove;

            if (worker->prevXMove == LEFT) {
                worker->prevXMove = RIGHT;
            } else {
                worker->prevXMove = LEFT;
            }
        }

        if (nextMove != NONE) {
            moveWorkerInDirection(world, workerId, nextMove);
        } else {
            finishWorkerProcess(worker);
            return false;
        }
    }

    return true;
}

void moveWorkerInDirection(World *world, int workerId, enum Direction direction) {
    Worker *worker = &world->workers[workerId];
    Point *point = &worker->position;
    movePoint(point, direction);

    if (direction == RIGHT || direction == LEFT) {
        worker->prevXMove = direction;
    } else {
        worker->prevYMove = direction;
    }
}

Zone getWorkerZone(World *world, int workerId) {
    int n = world->n;
    int halfN = n / 2;
    int isNOdd = n % 2; // Check if N is odd
    int max = n - 1;
    int min = 0;

    int midPoint = isNOdd ? halfN : halfN - 1;

    if (workerId == 0) {
        return (Zone) {
                .topLeft = {min, min},
                .bottomRight = {midPoint, midPoint}
        };
    }

    if (workerId == 1) {
        return (Zone) {
                .topLeft = {midPoint + 1, 0},
                .bottomRight = {max, midPoint}
        };
    }

    if (workerId == 2) {
        return (Zone) {
                .topLeft = {0, midPoint + 1},
                .bottomRight = {midPoint, max}
        };
    }

    return (Zone) {
            .topLeft = {midPoint + 1, midPoint + 1},
            .bottomRight = {max, max}
    };
}

bool beenInTheHouseBefore(int workerId, House *house) {
    for (int i = 0; i < house->noteCount; i++) {
        // check if the workerId exists in the iterated note
        char *workerIdStr = malloc(100 * sizeof(char));;
        sprintf(workerIdStr, "%d", workerId);
        if (strstr(house->notes[i], workerIdStr) != NULL) {
            return true;
        }
    }
    return false;
}

void printFinalResults(World *world) {

    printf("\n\n\n*************************\n            Houses\n*************************\n\n");
    for (int i = 0; i < world->n; i++) {
        for (int j = 0; j < world->n; j++) {
            House *house = &world->houses[i][j];
            printHouse(house);
        }
    }

    printf("*************************\n            Workers\n*************************\n\n");
    for (int i = 0; i < 4; i++) {
        Worker *worker = &world->workers[i];
        printWorker(worker);
    }
}