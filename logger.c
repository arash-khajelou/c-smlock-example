//
// Created by Arash Khajelou on 2/29/24.
//

#include<stdio.h>

void logInfo(char *message) {
    printf("%s\n", message);
}

void logError(char *message) {
    perror(message);
}