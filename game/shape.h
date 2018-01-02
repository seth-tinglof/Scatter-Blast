#pragma once

typedef struct PointTag {
	double x;
	double y;
} Point;

typedef struct LineTag {
	Point initial;
	Point final;
} Line;

typedef struct PolygonTag {
	int numFaces;
	int width;
	int height;
	Line *sides;
} Polygon;

/**
Creates a polygon object from an array of x, y coordinate pairs.
*/
Polygon *createPolygonFromArrayOfXYPairs(int numFaces, int width, int height, double *xYPairs);

/**
Frees memory used to store passed polygon
*/
void deallocatePolygon(Polygon *removed);

/**
Finds the point of intersection between two line segments and stores that point in result.
One is returned if there is no intersection or the lines intersect at multiple points, and
zero is returned if an itersection is successfully found
*/
int findIntersection(Line *one, Line *two, Point *result);

/**
*Compares two doubles to see if they are almost equal. returns 1 if they are and 0 if they are not. The purpose of this
*function is to correct for floating point errors.
*/
int almostEqual(double one, double two);

/**
*Takes two Polygons followed by doubles with the x, y coordinates of both polygons as arguments
*and returns one if they intersect and two otherwise.
*/
int polygonsIntersect(Polygon *poly1, Polygon *poly2, double x1, double y1, double x2, double y2);
