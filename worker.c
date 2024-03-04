//
// Created by Arash Khajelou on 2/28/24.
//

#include <printf.h>
#include <stdio.h>
#include "worker.h"

void initWorker(Worker *worker, int x, int y, int workerId) {
    worker->workerId = workerId;
    worker->position.x = x;
    worker->position.y = y;
    worker->workerStatus = WORKING;
    worker->repairedHouses = 0;
    worker->prevXMove = NONE;
    worker->prevYMove = NONE;
    worker->zoneTraverseStarted = false;
}

void workerFacedError(Worker *worker, char *error) {
    printf("Worker faced an error: %s\n", error);
    finishWorkerProcess(worker);
}

void finishWorkerProcess(Worker *worker) {
    worker->workerStatus = FINISHED;
    printf("Worker %d finished the zone traverse\n", worker->workerId);
}