//
// Created by Arash Khajelou on 2/28/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "geometry.h"

void initZone(Zone *zone, int x, int y, int n) {
    zone->topLeft.x = x;
    zone->topLeft.y = y;
    zone->bottomRight.x = x + n;
    zone->bottomRight.y = y + n;
}

bool isPointInTheZone(Zone zone, Point point) {
    return point.x >= zone.topLeft.x && point.x <= zone.bottomRight.x &&
           point.y >= zone.topLeft.y && point.y <= zone.bottomRight.y;
}

Point getNearestCorner(Zone zone, Point point) {
    // Calculate Manhattan distances to each corner
    int distTopLeft = abs(zone.topLeft.x - point.x) + abs(zone.topLeft.y - point.y);
    int distTopRight = abs(zone.bottomRight.x - point.x) + abs(zone.topLeft.y - point.y);
    int distBottomLeft = abs(zone.topLeft.x - point.x) + abs(zone.bottomRight.y - point.y);
    int distBottomRight = abs(zone.bottomRight.x - point.x) + abs(zone.bottomRight.y - point.y);

    // Initialize the nearest corner as the top left and its distance
    Point nearestCorner = zone.topLeft;
    int minDist = distTopLeft;

    // Check and update the nearest corner based on Manhattan distances
    if (distTopRight < minDist) {
        minDist = distTopRight;
        nearestCorner = (Point){zone.bottomRight.x, zone.topLeft.y};
    }
    if (distBottomLeft < minDist) {
        minDist = distBottomLeft;
        nearestCorner = (Point){zone.topLeft.x, zone.bottomRight.y};
    }
    if (distBottomRight < minDist) {
        nearestCorner = zone.bottomRight;
    }

    return nearestCorner;
}

enum Direction decideNextMoveToGetInTheNearestCorner(Zone zone, Point point) {
    Point nearestPoint = getNearestCorner(zone, point);

    int xToGo = nearestPoint.x - point.x;
    int yToGo = nearestPoint.y - point.y;

    if (xToGo != 0) {
        return xToGo > 0 ? RIGHT : LEFT;
    }

    if (yToGo != 0) {
        return yToGo > 0 ? BOTTOM : TOP;
    }

    return NONE;
}

enum Direction
decideNextMoveToTraverseTheZone(Zone zone, Point point, enum Direction prevXMove, enum Direction prevYMove) {
    // Ensure the point is within the zone
    if (!isPointInTheZone(zone, point)) {
        return NONE;
    }

    if (prevXMove == NONE && prevYMove == NONE) {
        if (isPointOnTheCorner(zone, point)) {
            if (point.x == zone.topLeft.x || point.y == zone.bottomRight.y) {
                return RIGHT;
            } else {
                return LEFT;
            }
        }
        return NONE;
    }


    if (canContinueMoving(zone, point, prevXMove)) {
        return prevXMove;
    }

    if (canContinueMoving(zone, point, prevYMove)) {
        return prevYMove;
    }

    return NONE;
}

bool isPointOnTheEdge(Zone zone, Point point) {
    return point.x == zone.topLeft.x || point.x == zone.bottomRight.x || point.y == zone.topLeft.y ||
           point.y == zone.bottomRight.y;
}

bool isPointOnTheCorner(Zone zone, Point point) {
    return (point.x == zone.topLeft.x || point.x == zone.bottomRight.x) &&
           (point.y == zone.topLeft.y || point.y == zone.bottomRight.y);
}

bool canContinueMoving(Zone zone, Point point, enum Direction direction) {
    switch (direction) {
        case TOP:
            return point.y > zone.topLeft.y;
        case RIGHT:
            return point.x < zone.bottomRight.x;
        case BOTTOM:
            return point.y < zone.bottomRight.y;
        case LEFT:
            return point.x > zone.topLeft.x;
        case NONE:
            return false;
    }
    return false;
}

void movePoint(Point *point, enum Direction direction) {
    switch (direction) {
        case TOP:
            point->y--;
            break;
        case RIGHT:
            point->x++;
            break;
        case BOTTOM:
            point->y++;
            break;
        case LEFT:
            point->x--;
            break;
        case NONE:
            break;
    }
}

enum Direction getStartingZoneXDirection(Zone zone, Point point) {
    if (!isPointOnTheCorner(zone, point)) {
        return NONE;
    }

    if (point.x == zone.topLeft.x) {
        return RIGHT;
    } else if (point.x == zone.bottomRight.x) {
        return LEFT;
    }

    return NONE;
}

enum Direction getStartingZoneYDirection(Zone zone, Point point) {
    if (!isPointOnTheCorner(zone, point)) {
        return NONE;
    }

    if (point.y == zone.topLeft.y) {
        return BOTTOM;
    } else if (point.y == zone.bottomRight.y) {
        return TOP;
    }

    return NONE;
}

void printZone(Zone *zone) {
    printf("(%d, %d) => (%d, %d)", zone->topLeft.x, zone->topLeft.y, zone->bottomRight.x, zone->bottomRight.y);
}

void printPoint(Point *point) {
    printf("(%d, %d)", point->x, point->y);
}

void printDirection(enum Direction direction) {
    char *stringDir;
    switch (direction) {
        case TOP:
            stringDir = "TOP";
            break;
        case RIGHT:
            stringDir = "RIGHT";
            break;
        case BOTTOM:
            stringDir = "BOTTOM";
            break;
        case LEFT:
            stringDir = "LEFT";
            break;
        default:
            stringDir = "INVALID";
    }

    printf("%s", stringDir);
}