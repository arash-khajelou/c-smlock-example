//
// Created by Arash Khajelou on 2/27/24.
//

#ifndef FIRST_CODE_WORLD_H
#define FIRST_CODE_WORLD_H


#include "limits.h"
#include "house.h"
#include "worker.h"

typedef struct {
    House houses[MAX_SIZE][MAX_SIZE];
    Worker workers[MAX_WORKER_COUNT];
    int n, totalDamaged;
} World;

void initWorld(World *world, int n, int damagedHouses);

void startRepairProcess(World *world, int workerId, int *semIds);

void workerLogic(World *world, int workerId, int *semIds);

bool moveWorker(World *world, int workerId);

Zone getWorkerZone(World *world, int workerId);

void moveWorkerInDirection(World *world, int workerId, enum Direction direction);

bool beenInTheHouseBefore(int id, House *house);

void printFinalResults(World *world);

#endif //FIRST_CODE_WORLD_H
