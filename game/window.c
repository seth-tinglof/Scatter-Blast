#include <stdio.h>
#include<SDL.h>
#include<math.h>
#include"window.h"

#define DEFAULT_R 220
#define DEFAULT_G 220
#define DEFAULT_B 220

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

SDL_Window *win;
SDL_Renderer *ren;

SDL_Texture *buildTexture;
int buildTexSet = 0;

int windowCreated = 0;
int isFullScreen = 0;

SCREEN_WIDTH = DEFAULT_WIDTH;
SCREEN_HEIGHT = DEFAULT_HEIGHT;

int createWindow() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initialization");
		return 1;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	win = SDL_CreateWindow("Hello World!",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	if(win == NULL) {
		printf("Error window");
		SDL_Quit();
		return 1;
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (ren == NULL) {
		SDL_DestroyWindow(win);
		printf("Error renderer");
		SDL_Quit();
		return 1;
	}

	SDL_SetRenderDrawColor(ren, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	windowCreated = 1;
	return 0;
}

int toggleFullscreen(double *scale) {
	if (!windowCreated)
		return 0;
	if (!isFullScreen) {
		SDL_DisplayMode mode;
		if (SDL_GetDesktopDisplayMode(0, &mode) != 0){
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
			return 1;
		}
		scale[0] = ((double)mode.w) / SCREEN_WIDTH;
		scale[1] = ((double)mode.h) / SCREEN_HEIGHT;
		scale[2] = sqrt(mode.w * mode.w + mode.h * mode.h) / sqrt(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT);
		SCREEN_WIDTH = mode.w;
		SCREEN_HEIGHT = mode.h;
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
		isFullScreen = 1;
	}
	else {
		scale[0] = ((double)DEFAULT_WIDTH) / SCREEN_WIDTH;
		scale[1] = ((double)DEFAULT_HEIGHT) / SCREEN_HEIGHT;
		scale[2] = sqrt(DEFAULT_WIDTH * DEFAULT_WIDTH + DEFAULT_HEIGHT * DEFAULT_HEIGHT) / sqrt(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT);
		SCREEN_WIDTH = DEFAULT_WIDTH;
		SCREEN_HEIGHT = DEFAULT_HEIGHT;
		SDL_SetWindowFullscreen(win, 0);
		isFullScreen = 0;
	}
	return 0;
}

SDL_Texture *renderSurfaceToTexture(SDL_Surface *surface) {
	if (!windowCreated) {
		printf("Window not created.");
		return NULL;
	}
	SDL_Texture *result = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (result == NULL) {
		printf("Error creating texture/");
		return NULL;
	}
	
	return result;
}

SDL_Texture *createTexture(int w, int h) {
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	SDL_Texture *result = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(ren, result);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderClear(ren);
	SDL_SetRenderTarget(ren, NULL);
	SDL_SetRenderDrawColor(ren, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	return result;
}

void setBuildTexture(int w, int h) {
	if (buildTexSet)
		SDL_DestroyTexture(buildTexture);
	buildTexture = createTexture(w, h);
	buildTexSet = 1;
}

int writeToBuildTexture(SDL_Texture *tex1, SDL_Rect *dest) {
	SDL_SetRenderTarget(ren, buildTexture);
	if (SDL_RenderCopy(ren, tex1, NULL, dest) != 0) {
		SDL_SetRenderTarget(ren, NULL);
		return 1;
	}
	SDL_SetRenderTarget(ren, NULL);
	return 0;
}

SDL_Texture *getBuildTexture(int w, int h) {
	SDL_Texture *result = createTexture(w, h);
	SDL_SetRenderTarget(ren, result);
	if (SDL_RenderCopy(ren, buildTexture, NULL, NULL) != 0) {
		printf("Unable to get build texture.");
	}
	SDL_SetRenderTarget(ren, NULL);
	return result;
}

int scaleTexture(SDL_Texture *target, SDL_Texture *texture) {
	SDL_SetRenderTarget(ren, target);
	if (SDL_RenderCopy(ren, texture, NULL, NULL) != 0) {
		SDL_SetRenderTarget(ren, NULL);
		return 1;
	}
	SDL_SetRenderTarget(ren, NULL);
	return 0;
}

int renderHorizontalLineOnTexture(SDL_Texture *target, int xi, int xf, int y, int r, int g, int b) {
	if (!windowCreated) {
		printf("Window not created.");
		return 1;
	}
	SDL_SetRenderTarget(ren, target);
	SDL_SetRenderDrawColor(ren, r , g, b, 255);
	SDL_RenderDrawLine(ren, xi, y, xf, y);
	SDL_SetRenderTarget(ren, NULL);
	SDL_SetRenderDrawColor(ren, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	return 0;
}

void clearScreen() {
	SDL_SetRenderDrawColor(ren, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(ren);
}

int drawTexture(SDL_Texture *tex, int x, int y, int w, int h) {
	if (tex == NULL) {
		printf("Null texture.");
		return 1;
	}
	SDL_Rect dstrect;
	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = w;
	dstrect.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dstrect);
	return 0;
}

void drawScreen() {
	SDL_RenderPresent(ren);
}

void quitWindow() {
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	if(buildTexSet)
		SDL_DestroyTexture(buildTexture);
	SDL_Quit();
}