#include <stdlib.h>
#include <math.h>
#include"shape.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

Polygon *createPolygonFromArrayOfXYPairs(int numFaces, int width, int height, double *xYPairs) {
	Line *lines = malloc(numFaces * sizeof(Line));
	for (int i = 0; i < numFaces * 2 - 3; i += 2) {
		lines[i / 2].initial.x = xYPairs[i];
		lines[i / 2].initial.y = xYPairs[i + 1];
		lines[i / 2].final.x = xYPairs [i + 2];
		lines[i / 2].final.y = xYPairs[i + 3];
	}
	lines[numFaces - 1].initial.x = xYPairs[numFaces * 2 - 2];
	lines[numFaces - 1].initial.y = xYPairs[numFaces * 2 - 1];
	lines[numFaces - 1].final.x = *xYPairs;
	lines[numFaces - 1].final.y = xYPairs[1];
	Polygon *result = malloc(sizeof(Polygon));
	result->numFaces = numFaces;
	result->sides = lines;
	result->width = width;
	result->height = height;
	return result;
}

void deallocatePolygon(Polygon *removed) {
	free(removed->sides);
	free(removed);
}

int almostEqual(double one, double two) {
	if (one == two)
		return 1;
	if (one > two && one - 0.000001 < two)
		return 1;
	if (one < two && one + 0.000001 > two)
		return 1;
	return 0;
}

int isXYPairOnSegment(Line *line, double x, double y) {
	if ((almostEqual(x, line->initial.x) || almostEqual(x, line->final.x)) && (almostEqual(y, line->initial.y) || almostEqual(y, line->final.y)))
		return 1;
	if (max(line->initial.x, line->final.x) < x || min(line->initial.x, line->final.x) > x)
		return 0;
	if (max(line->initial.y, line->final.y) < y || min(line->initial.y, line->final.y) > y)
		return 0;
	return 1;
}

int findIntersection(Line *one, Line *two, Point *result) {
	if (one->initial.x == one->final.x && two->initial.x == two->final.x) {
		return 1;
	}

	double a1 = NAN;
	double a2 = NAN;
	double b1 = NAN;
	double b2 = NAN;
	double xIntersect;
	double yIntersect;

	if (one->initial.x != one->final.x) {
		a1 = ((double)(one->final.y - one->initial.y)) / (one->final.x - one->initial.x);
		b1 = one->initial.y - a1 * one->initial.x;
	}
	if (two->initial.x != two->final.x) {
		a2 = ((double)(two->final.y - two->initial.y)) / (two->final.x - two->initial.x);
		b2 = two->initial.y - a2 * two->initial.x;
	}
	
	if (!isnan(a1) && !isnan(a2)) {
		if (a1 == a2)
			return 1;
		xIntersect = (b2 - b1) / (a1 - a2);
		yIntersect = a1 * xIntersect + b1;
	}

	else if (isnan(a1)) {
		xIntersect = one->initial.x;
		yIntersect = a2 * xIntersect + b2;
	}
	else {
		xIntersect = two->initial.x;
		yIntersect = a1 * xIntersect + b1;
	}

	if (!isXYPairOnSegment(one, xIntersect, yIntersect) || !isXYPairOnSegment(two, xIntersect, yIntersect))
		return 1;
	result->x = xIntersect;
	result->y = yIntersect;
	return 0;
}

int polygonsIntersect(Polygon *poly1, Polygon *poly2, double x1, double y1, double x2, double y2) {
	if (x1 + poly1->width < x2 || x1 - poly2->width > x2)
		return 0;
	if (y1 + poly1->height < y2 || y1 - poly2->height > y2)
		return 0;
	Point unused;
	Line side1;
	Line side2;
	for (int i = 0; i < poly1->numFaces; i++) {
		for (int j = 0; j < poly2->numFaces; j++) {
			side1.initial.x = x1 + (poly1->sides + i)->initial.x;
			side1.initial.y = y1 + (poly1->sides + i)->initial.y;
			side1.final.x = x1 + (poly1->sides + i)->final.x;
			side1.final.y = y1 + (poly1->sides + i)->final.y;

			side2.initial.x = x2 + (poly2->sides + j)->initial.x;
			side2.initial.y = y2 + (poly2->sides + j)->initial.y;
			side2.final.x = x2 + (poly2->sides + j)->final.x;
			side2.final.y = y2 + (poly2->sides + j)->final.y;
			if (findIntersection(&side1, &side2, &unused) == 0)
				return 1;
		}
	}
	return 0;
}