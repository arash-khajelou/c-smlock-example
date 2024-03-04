//
// Created by Arash Khajelou on 2/27/24.
//

#include <ncurses.h>

#include "monitor.h"

void initializeMonitor() {
    initscr(); // Start curses mode
    cbreak(); // Line buffering disabled
    noecho(); // Don't echo while we do getch
    curs_set(0); // Hide the cursor
    start_color(); // Start color functionality
}

void printWorldStatus(World *world) {
    clear(); // Clear the screen of its current contents

    // Initialize some color pairs
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);

    // Iterate through the world grid
    for (int i = 0; i < world->n; i++) {
        for (int j = 0; j < world->n; j++) {
            // Choose color based on house state
            if (world->houses[i][j].isDamaged) {
                attron(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
            }
            mvprintw(i, j * 2, "H"); // Print "H" for House
            attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
        }
    }

    // Print workers
    for (int i = 0; i < MAX_WORKER_COUNT; i++) {
        attron(COLOR_PAIR(3));
        mvprintw(world->workers[i].position.y, world->workers[i].position.x * 2, "W");
        attroff(COLOR_PAIR(3));
    }

    refresh(); // Print it on to the real screen
}

void cleanupMonitor() {
    endwin(); // End curses mode
}