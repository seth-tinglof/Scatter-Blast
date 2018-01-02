#include<SDL.h>
#include"window.h"
#include"player.h"
#include"enemy.h"
#include"projectile.h"

SDL_Texture *endScreenTexture;
int viewEndScreen;
int quit = 0;

void createEndScreen() {
	SDL_Surface *end = SDL_LoadBMP("end_screen.bmp");
	if (end == NULL) {
		printf("Unable to load bitmap: %s\n", SDL_GetError());
	}
	endScreenTexture = renderSurfaceToTexture(end);
	drawTexture(endScreenTexture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	drawScreen();
}

void endEventHandling() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN && (event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_LALT || event.key.keysym.scancode == SDL_SCANCODE_RALT)) {
			const Uint8 *state = SDL_GetKeyboardState(0);
			if (state[SDL_SCANCODE_RETURN] && (state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT])) {
				double scale[3];
				toggleFullscreen(scale);
				rescalePlayerSpeed(scale);
				rescaleEnemyInitialSpeed(scale);
				rescaleProjectileSpeed(scale);
				createEndScreen();
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
			if (event.button.x >= SCREEN_WIDTH / 2.2967 && event.button.x <= SCREEN_WIDTH / 1.8605 &&
				event.button.y >= SCREEN_HEIGHT / 1.519 && event.button.y <= SCREEN_HEIGHT / 1.34) {
				quit = 1;
			}
			else if (event.button.x >= SCREEN_WIDTH / 2.2967 && event.button.x <= SCREEN_WIDTH / 1.8605 &&
				event.button.y >= SCREEN_HEIGHT / 1.9354 && event.button.y <= SCREEN_HEIGHT / 1.6564) {
				viewEndScreen = 0;
			}
		}
		if (event.type == SDL_QUIT || quit) {
			SDL_DestroyTexture(endScreenTexture);
			quitWindow();
			exit(0);
		}
	}
}

void runEndScreen() {
	viewEndScreen = 1;
	createEndScreen();
	while (viewEndScreen) {
		endEventHandling();
		SDL_Delay(30);
	}
}