#include <SDL.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"

#define DOWNSCALING 2			//A new texture is created larger by a factor of DOWNSCALING before being copied to a texture of the target size and returned.

/*Compares doubles for a qsort.*/
int doubleCompare(const void *one, const void *two) {
	const double val = *(const double *)one - *(const double *)two;
	if (val < 0)
		return -1;
	if (val > 0)
		return 1;
	return 0;
}

SDL_Texture *fillPolygon(Polygon *polygon, int r, int g, int b) {
	SDL_Texture *t = createTexture(polygon->width * DOWNSCALING, polygon->height * DOWNSCALING);
	double *xVals = malloc(polygon->numFaces * sizeof(double));
	int countIntersects = 0;
	Line currLine;
	Line poly_line;
	Point intersect;
	SDL_Rect fill;
	for (int i = 0; i < polygon->height * DOWNSCALING; i++) {
		for (int j = 0; j < polygon->numFaces; j++) {
			currLine.initial.y = i;
			currLine.final.y = i;
			currLine.initial.x = 0;
			currLine.final.x = polygon->width * DOWNSCALING;
			poly_line.initial.x = DOWNSCALING * (polygon->sides + j)->initial.x;
			poly_line.initial.y = DOWNSCALING * (polygon->sides + j)->initial.y;
			poly_line.final.x = DOWNSCALING * (polygon->sides + j)->final.x;
			poly_line.final.y = DOWNSCALING * (polygon->sides + j)->final.y;
			if (findIntersection(&currLine, &poly_line, &intersect) == 0) {
				Point topOfLine = poly_line.initial.y < poly_line.final.y ? poly_line.initial : poly_line.final;
				if(!almostEqual(intersect.x, topOfLine.x) || !almostEqual(intersect.y, topOfLine.y)){	//Does not store intersections with the top of a line to solve an edge case,i.e, a V or ^ shape.
					xVals[countIntersects] = intersect.x;												//Stores x-coordinate of intersection (y-coordintate is i).
					countIntersects++;
				}
			}
		}
		qsort(xVals, countIntersects, sizeof(double), doubleCompare);
		for (int j = 0; j < countIntersects - 1; j += 2) {
			renderHorizontalLineOnTexture(t, xVals[j], xVals[j+1], i, r, g, b);		//draw lines between every other pair of sorted intersects.
		}
		countIntersects = 0;
	}
	free(xVals);
	SDL_Texture *result = createTexture(polygon->width, polygon->height);
	scaleTexture(result, t);
	SDL_DestroyTexture(t);
	return result;
}