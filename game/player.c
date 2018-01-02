#include <stdio.h>
#include"draw_engine.h"
#include"player.h"
#include"window.h"

double PLAYER_SPEED = 2.5;

void setPlayerVelocity(Player *player, int xVelocity, int yVelocity) {
	player->xVelocity = xVelocity;
	player->xVelocity = yVelocity;
}

SDL_Texture *createPlayerTexture() {
	PLAYER_WIDTH = SCREEN_WIDTH / 17;
	PLAYER_HEIGHT = PLAYER_WIDTH;
	double top[] = {0., 0., PLAYER_WIDTH / 2., PLAYER_HEIGHT / 2.,(double)PLAYER_WIDTH, 0.};
	double right[] = { (double)PLAYER_WIDTH, 0., PLAYER_WIDTH / 2., PLAYER_HEIGHT / 2.,(double)PLAYER_WIDTH,(double)PLAYER_HEIGHT };
	double bottom[] = { (double)PLAYER_WIDTH, (double)PLAYER_HEIGHT, PLAYER_WIDTH / 2., PLAYER_HEIGHT / 2., 0.,(double)PLAYER_HEIGHT };
	double left[] = { 0.,(double)PLAYER_HEIGHT, PLAYER_WIDTH / 2., PLAYER_HEIGHT / 2., 0., 0. };

	SDL_Rect dest1 = { .x = 0,.y = 0,.w = PLAYER_WIDTH,.h = PLAYER_HEIGHT / 2 };
	SDL_Rect dest2 = { .x = PLAYER_WIDTH / 2,.y = 0,.w = PLAYER_WIDTH / 2,.h = PLAYER_HEIGHT};
	SDL_Rect dest3 = { .x = 0,.y = PLAYER_HEIGHT / 2,.w = PLAYER_WIDTH,.h = PLAYER_HEIGHT / 2 };
	SDL_Rect dest4 = { .x = 0,.y = 0,.w = PLAYER_WIDTH / 2,.h = PLAYER_HEIGHT};

	Polygon *pTop = createPolygonFromArrayOfXYPairs(3, PLAYER_WIDTH, PLAYER_HEIGHT, top);
	Polygon *pRight = createPolygonFromArrayOfXYPairs(3, PLAYER_WIDTH, PLAYER_HEIGHT, right);
	Polygon *pBottom = createPolygonFromArrayOfXYPairs(3, PLAYER_WIDTH, PLAYER_HEIGHT, bottom);
	Polygon *pLeft = createPolygonFromArrayOfXYPairs(3, PLAYER_WIDTH, PLAYER_HEIGHT, left);

	setBuildTexture(PLAYER_WIDTH, PLAYER_HEIGHT);

	SDL_Texture *topTex = fillPolygon(pTop, 0, 0, 250);
	writeToBuildTexture(topTex, NULL);
	SDL_DestroyTexture(topTex);

	SDL_Texture *rightTex = fillPolygon(pRight, 0, 250, 0);
	writeToBuildTexture(rightTex, NULL);
	SDL_DestroyTexture(rightTex);

	SDL_Texture *botTex = fillPolygon(pBottom, 230, 230, 50);
	writeToBuildTexture(botTex, NULL);
	SDL_DestroyTexture(botTex);

	SDL_Texture *leftTex = fillPolygon(pLeft, 250, 0, 0);
	writeToBuildTexture(leftTex, NULL);
	SDL_DestroyTexture(leftTex);
	
	deallocatePolygon(pTop);
	deallocatePolygon(pRight);
	deallocatePolygon(pBottom);
	deallocatePolygon(pLeft);

	return getBuildTexture(PLAYER_WIDTH, PLAYER_HEIGHT);
}

void resetPlayerTexture(Player *player) {
	SDL_DestroyTexture(player->tex);
	player->tex = createPlayerTexture();
}

Player *createPlayer() {
	Player *result = malloc(sizeof(Player));
	result->xPos = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
	result->yPos = SCREEN_HEIGHT / 2 - PLAYER_HEIGHT / 2;
	result->yVelocity = 0;
	result->xVelocity = 0;
	result->tex = createPlayerTexture();
	double corners[] = { 0., 0. , PLAYER_WIDTH, 0., PLAYER_WIDTH, PLAYER_HEIGHT, 0., PLAYER_HEIGHT};
	result->playerShape = createPolygonFromArrayOfXYPairs(4, PLAYER_WIDTH, PLAYER_HEIGHT, corners);
	return result;
}

void acceleratePlayer(Player *player, int left, int up, int right, int down) {
	if (left && right || !left && !right) {
		player->xVelocity = 0;
	}
	else if (left) {
		player->xVelocity = -PLAYER_SPEED;
	}
	else if (right) {
		player->xVelocity = PLAYER_SPEED;
	}

	if (up && down || !up && !down) {
		player->yVelocity = 0;
	}
	else if (up) {
		player->yVelocity = -PLAYER_SPEED;
	}
	else if (down) {
		player->yVelocity = PLAYER_SPEED;
	}
}

void movePlayer(Player *player) {
	player->xPos += player->xVelocity;
	player->yPos += player->yVelocity;
	if (player->xPos < 0)
		player->xPos = 0;
	else if (player->xPos + PLAYER_WIDTH > SCREEN_WIDTH)
		player->xPos = SCREEN_WIDTH - PLAYER_WIDTH;
	if (player->yPos < 0)
		player->yPos = 0;
	else if (player->yPos + PLAYER_HEIGHT > SCREEN_HEIGHT)
		player->yPos = SCREEN_HEIGHT - PLAYER_HEIGHT;
}

void rescalePlayerSpeed(double *scale) {
	PLAYER_SPEED *= scale[2];
}

void rescalePlayerPositionAndMotion(Player *player, double *scale) {
	player->xPos *= scale[0];
	player->yPos *= scale[1];
	rescalePlayerSpeed(scale);
}