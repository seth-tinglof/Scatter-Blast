#pragma once

#include <SDL.h>

int SCREEN_WIDTH;
int SCREEN_HEIGHT;


/*
Creates a New SDL window and renderer.
*/
int createWindow();

/*Makes the window "fullscreen" if it is currently "windowed", and makes the window "windowed" if it is currently "fullscreen".
This function should be passed a size three double array; index 0 will be set to the ratio of the new screen width over the old, 
index one will be set to the ratio of the new screen height over the old, and index three will be the ratio of the length of the
new diagonal over the old.*/
int toggleFullscreen(double *scale);

/*
Renders arguemnt surface to a texture and returns a pointer to that texture.
*/
SDL_Texture *renderSurfaceToTexture(SDL_Surface *surface);

/*
Clears the screen/renderer of all images.
*/
void clearScreen();

/*
Draws argument texture to the screen. Four parameters are the x coordinate of the texture on the screen,
the y coordinate, the width in pixels, and the height in pixels.
*/
int drawTexture(SDL_Texture *tex, int x, int y, int w, int h);

/*
Presents the window with everything that has been drawn to the renderer.
*/
void drawScreen();

/*
Closes window and quits SDL.
*/
void quitWindow();

/*
Returns a pointer to a new SDL texture with argument width and height. The texture starts completely transparent.
*/
SDL_Texture *createTexture(int w, int h);

/*
Creates a new texture with argument width and height that will be drawn to by subsiquent calls to writeToBuildTexture.
*/
void setBuildTexture(int w, int h);

/*
Draws argument texture to the texture created by setBuildTexture at the location of the arguement destination SDL_Rect.
Returns 0 on success and 1 on error.
*/
int writeToBuildTexture(SDL_Texture *tex1, SDL_Rect *dest);

/*
Returns the texture created by setBuildTexture.
*/
SDL_Texture *getBuildTexture(int w, int h);

/*
Copies the texture passed as the second argument to the texture passed as the first argument. Returns 0 on success and 1 on error.
*/
int scaleTexture(SDL_Texture *target, SDL_Texture *texture);

/*
Renders a horizontal line on the argument texture. First three integer arguments are the initial and final x position of the line and the y
position of the line. The next three are the rgb values of the line.
*/
int renderHorizontalLineOnTexture(SDL_Texture *target, int xi, int xf, int y, int r, int g, int b);