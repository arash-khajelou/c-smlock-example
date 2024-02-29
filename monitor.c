//
// Created by Arash Khajelou on 2/27/24.
//

#include <ncurses.h>

#include "world.h"
#include "monitor.h"

// Initialization of ncurses mode
void initializeMonitor() {
    initscr();          // Start curses mode
    noecho();           // Don't echo any keypresses
    curs_set(FALSE);    // Don't display a cursor
}

// Modified printWorldStatus function using ncurses
void printWorldStatus(World *world) {
    clear(); // Clear the screen
    for (int i = 0; i < world->n; i++) {
        for (int j = 0; j < world->n; j++) {
            char symbol = world->houses[i][j].isDamaged ? 'D' : '.';
            for (int k = 0; k < 4; k++) {
                if (world->workers[k].position.x == i && world->workers[k].position.y == j) {
                    symbol = '1' + k;
                    break;
                }
            }
            mvaddch(i, j * 2, symbol); // Use mvaddch to move and add a character
        }
    }
    refresh(); // Print it on to the real screen
}

void cleanupMonitor() {
    endwin();
}
