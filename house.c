//
// Created by Arash Khajelou on 2/28/24.
//

#include <stdio.h>
#include <string.h>
#include "house.h"

void initHouse(House *house) {
    house->position.x = 0;
    house->position.y = 0;
    house->isDamaged = false;
    house->noteCount = 0; // Initialize note count
    for (int i = 0; i < MAX_WORKER_COUNT; i++) {
        house->notes[i][0] = '\0'; // Initialize each note as an empty string
    }
}

void addNoteToHouse(House *house, const char *note) {
    if (house->noteCount >= MAX_WORKER_COUNT) {
        fprintf(stderr, "Cannot add more notes to the house.\n");
        return;
    }

    // Ensure the note length does not exceed MAX_NOTE_LENGTH, including the null terminator
    strncpy(house->notes[house->noteCount], note, MAX_NOTE_LENGTH - 1);
    house->notes[house->noteCount][MAX_NOTE_LENGTH - 1] = '\0'; // Ensure null termination
    house->noteCount++; // Increment the note count
}