#include<SDL.h>
#include"window.h"
#include"player.h"
#include"enemy.h"
#include"projectile.h"

SDL_Texture *titleScreenTexture;
int viewTitleScreen;

/*Creates the texture for the title screen and stores a pointer to this texture in the variable titleScreenTexture.*/
void createTitleWindow() {
	SDL_Surface *title = SDL_LoadBMP("title_screen.bmp");
	if (title == NULL) {
		printf("Unable to load bitmap: %s\n", SDL_GetError());
	}
	titleScreenTexture = renderSurfaceToTexture(title);
	drawTexture(titleScreenTexture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	drawScreen();
}

/*Handles the events for player input in the title screen.*/
void titleEventHandling() {
	SDL_Event event;
	/*Toggle fullscreen if player hits alt+enter.*/
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN && (event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_LALT || event.key.keysym.scancode == SDL_SCANCODE_RALT)) {
			const Uint8 *state = SDL_GetKeyboardState(0);
			if (state[SDL_SCANCODE_RETURN] && (state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT])) {
				double scale[3];
				toggleFullscreen(scale);
				rescalePlayerSpeed(scale);
				rescaleEnemyInitialSpeed(scale);
				rescaleProjectileSpeed(scale);
				createTitleWindow();
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {				//Player hits play.
			if (event.button.x >= SCREEN_WIDTH / 2.1573 && event.button.x <= SCREEN_WIDTH / 1.8641 &&
				event.button.y >= SCREEN_HEIGHT / 1.2632 && event.button.y <= SCREEN_HEIGHT / 1.1551) {
				viewTitleScreen = 0;
			}
		}
		else if (event.type == SDL_QUIT) {		     //Player attempts to close the window.
			SDL_DestroyTexture(titleScreenTexture);
			quitWindow();
			exit(0);
		}
	}
}

void runTitleScreen() {
	createTitleWindow();
	viewTitleScreen = 1;
	while (viewTitleScreen) {
		titleEventHandling();
		SDL_Delay(30);
	}
}