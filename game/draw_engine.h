#pragma once

#include<SDL.h>
#include "shape.h"

/**
*Returns an SDL_Texture that is the image of the argument polygon filled with the color passed as the three values r, g, and b.
*These values are rgb values from 0 to 255.
*/
SDL_Texture *fillPolygon(Polygon *polygon, int r, int g, int b);
