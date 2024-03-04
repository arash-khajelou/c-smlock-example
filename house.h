//
// Created by Arash Khajelou on 2/28/24.
//

#ifndef FIRST_CODE_HOUSE_H
#define FIRST_CODE_HOUSE_H

#include <stdbool.h>
#include "geometry.h"
#include "limits.h"

typedef struct {
    Point position;
    bool isDamaged;
    char notes[MAX_WORKER_COUNT][MAX_NOTE_LENGTH];
    int noteCount;
} House;


void addNoteToHouse(House *house, const char *note);

void initHouse(House *house, int x, int y);

void printHouse(House *house);

#endif //FIRST_CODE_HOUSE_H
