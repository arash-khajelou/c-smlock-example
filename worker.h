//
// Created by Arash Khajelou on 2/28/24.
//

#ifndef FIRST_CODE_WORKER_H
#define FIRST_CODE_WORKER_H

#include "geometry.h"

enum WorkerStatus {
    WORKING, FINISHED
};

typedef struct {
    int workerId;
    int repairedHouses;
    bool zoneTraverseStarted;
    enum WorkerStatus workerStatus;
    enum Direction prevYMove;
    enum Direction prevXMove;
    Point position;
} Worker;

void initWorker(Worker *worker, int x, int y, int workerId);

void workerFacedError(Worker *worker, char *error);

void finishWorkerProcess(Worker *worker);

#endif //FIRST_CODE_WORKER_H
