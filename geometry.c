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
    Point nearestPoint = point;
    if (point.x < zone.topLeft.x) {
        nearestPoint.x = zone.topLeft.x;
    } else if (point.x > zone.bottomRight.x) {
        nearestPoint.x = zone.bottomRight.x;
    }
    if (point.y < zone.topLeft.y) {
        nearestPoint.y = zone.topLeft.y;
    } else if (point.y > zone.bottomRight.y) {
        nearestPoint.y = zone.bottomRight.y;
    }
    return nearestPoint;
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
            return point.y >= zone.topLeft.y;
        case RIGHT:
            return point.x <= zone.bottomRight.x;
        case BOTTOM:
            return point.y <= zone.bottomRight.y;
        case LEFT:
            return point.x > zone.topLeft.x;
        case NONE:
            return false;
    }
    return false;
}

void movePoint(Point *point, enum Direction direction) {
    char *message = malloc(100 * sizeof(char));
    sprintf(message, "Moving point(x: %d, y: %d) in direction %d", direction, point->x, point->y);

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