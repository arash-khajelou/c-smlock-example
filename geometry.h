//
// Created by Arash Khajelou on 2/28/24.
//

#ifndef FIRST_CODE_GEOMETRY_H
#define FIRST_CODE_GEOMETRY_H

#include <stdbool.h>

// Define directions for clarity
enum Direction {
    TOP, RIGHT, BOTTOM, LEFT, NONE
};

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point topLeft, bottomRight;
} Zone;

void initZone(Zone *zone, int x, int y, int n);

bool isPointInTheZone(Zone zone, Point point);

Point getNearestCorner(Zone zone, Point point);

enum Direction decideNextMoveToGetInTheNearestCorner(Zone zone, Point point);

enum Direction
decideNextMoveToTraverseTheZone(Zone zone, Point point, enum Direction prevXMove, enum Direction prevYMove);

bool canContinueMoving(Zone zone, Point point, enum Direction direction);

void movePoint(Point *point, enum Direction direction);

bool isPointOnTheEdge(Zone zone, Point point);

bool isPointOnTheCorner(Zone zone, Point point);

enum Direction getStartingZoneXDirection(Zone zone, Point point);

enum Direction getStartingZoneYDirection(Zone zone, Point point);

void printZone(Zone *zone);

void printPoint(Point *point);

void printDirection(enum Direction direction);

#endif //FIRST_CODE_GEOMETRY_H
