//
// Created by Arash Khajelou on 2/27/24.
//

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#include "monitor.h"

void initializeMonitor() {
    initscr(); // Start curses mode
    cbreak(); // Line buffering disabled
    noecho(); // Don't echo while we do getch
    curs_set(0); // Hide the cursor
    start_color(); // Start color functionality
}

void printWorldStatus(World *world) {
    clear(); // Clear the screen before redrawing

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);

    // Draw the grid based on the current world state
    for (int i = 0; i < world->n; i++) {
        for (int j = 0; j < world->n; j++) {
            if (world->houses[i][j].isDamaged) {
                attron(COLOR_PAIR(1)); // Red for damaged
            } else {
                attron(COLOR_PAIR(2)); // Green for undamaged
            }
            mvprintw(j, i * 2, "H");
            attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
        }
    }

    // Draw workers
    for (int i = 0; i < MAX_WORKER_COUNT; i++) {
        attron(COLOR_PAIR(3)); // Yellow for workers
        mvprintw(world->workers[i].position.y, world->workers[i].position.x * 2, "W");
        attroff(COLOR_PAIR(3));
    }

    refresh();
}

void cleanupMonitor() {
    endwin(); // End curses mode
}

void monitorMainLoop(World *world) {
    if (fork() == 0) { // Child process
        while (true) {
            printWorldStatus(world);

            int finishedCount = 0;
            for (int i = 0; i < 4; i++) {
                Worker *worker = &world->workers[i];
                if (worker->workerStatus == FINISHED) {
                    finishedCount++;
                }
            }

            if (finishedCount == 4) {
                break;
            }

            usleep(300000); // Sleep for 300 ms
        }
        exit(0); // Terminate child process
    }
}