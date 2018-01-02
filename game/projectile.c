#include <SDL.h>
#include"window.h"
#include"shape.h"
#include"projectile.h"

#define PROJECTILE_LIFE_TIME 5000

double PROJECTILE_VELOCITY = 5.0;

void createProjectileTextures() {
	PROJECTILE_WIDTH = SCREEN_WIDTH / 35;
	PROJECTILE_HEIGHT = PROJECTILE_WIDTH;
	double upPoints[] = { PROJECTILE_WIDTH / 2.0, 0.0, (double)PROJECTILE_WIDTH, (double)PROJECTILE_HEIGHT, 0.0, (double)PROJECTILE_HEIGHT };
	projUpPoly = createPolygonFromArrayOfXYPairs(3, PROJECTILE_WIDTH, PROJECTILE_HEIGHT, upPoints);
	projUpTex = fillPolygon(projUpPoly, 0, 0, 250);

	double downPoints[] = { PROJECTILE_WIDTH / 2.0, (double) PROJECTILE_HEIGHT, (double)PROJECTILE_WIDTH, 0.0, 0.0, 0.0};
	projDownPoly = createPolygonFromArrayOfXYPairs(3, PROJECTILE_WIDTH, PROJECTILE_HEIGHT, downPoints);
	projDownTex = fillPolygon(projDownPoly, 230, 230, 50);

	double rightPoints[] = {(double) PROJECTILE_WIDTH, PROJECTILE_HEIGHT / 2.0, PROJECTILE_WIDTH * .9, PROJECTILE_HEIGHT * .2, 
							PROJECTILE_WIDTH * .6, 0.0, 0.0, 0.0, 0.0, (double) PROJECTILE_HEIGHT, 
							PROJECTILE_WIDTH * .6, (double) PROJECTILE_HEIGHT, PROJECTILE_WIDTH *.9, PROJECTILE_HEIGHT * .8};
	projRightPoly = createPolygonFromArrayOfXYPairs(7, PROJECTILE_WIDTH, PROJECTILE_HEIGHT, rightPoints);
	projRightTex = fillPolygon(projRightPoly, 0, 250, 0);

	double leftPoints[] = { 0, PROJECTILE_HEIGHT / 2.0, PROJECTILE_WIDTH * .1, PROJECTILE_HEIGHT * .2,
							PROJECTILE_WIDTH * .4, 0.0, (double) PROJECTILE_WIDTH, 0.0, (double)PROJECTILE_WIDTH, (double)PROJECTILE_HEIGHT,
							PROJECTILE_WIDTH * .4, (double)PROJECTILE_HEIGHT, PROJECTILE_WIDTH *.1, PROJECTILE_HEIGHT * .8 };
	projLeftPoly = createPolygonFromArrayOfXYPairs(7, PROJECTILE_WIDTH, PROJECTILE_HEIGHT, leftPoints);
	projLeftTex = fillPolygon(projLeftPoly, 250, 0, 0);
}

Projectile *createProjectile(Player *player, Direction direction, int creationTime) {
	Projectile *result = malloc(sizeof(Projectile));
	switch (direction)
	{
	case DIRECTION_LEFT:
		result->color = COLOR_RED;
		result->xPos = player->xPos - PROJECTILE_WIDTH;
		result->yPos = player->yPos + PLAYER_HEIGHT / 2 - PROJECTILE_HEIGHT / 2;
		result->xVelocity = player->xVelocity - PROJECTILE_VELOCITY;
		result->yVelocity = player->yVelocity;
		break;
	case DIRECTION_UP:
		result->color = COLOR_BLUE;
		result->xPos = player->xPos + PLAYER_WIDTH / 2 - PROJECTILE_WIDTH / 2;
		result->yPos = player->yPos - PROJECTILE_HEIGHT;
		result->xVelocity = player->xVelocity;
		result->yVelocity = player->yVelocity - PROJECTILE_VELOCITY;
		break;
	case DIRECTION_RIGHT:
		result->color = COLOR_GREEN;
		result->xPos = player->xPos + PLAYER_WIDTH;
		result->yPos = player->yPos + PLAYER_HEIGHT / 2 - PROJECTILE_HEIGHT / 2;
		result->xVelocity = player->xVelocity + PROJECTILE_VELOCITY;
		result->yVelocity = player->yVelocity;
		break;
	case DIRECTION_DOWN:
		result->color = COLOR_YELLOW;
		result->xPos = player->xPos + PLAYER_WIDTH / 2 - PROJECTILE_WIDTH / 2;
		result->yPos = player->yPos + PLAYER_HEIGHT;
		result->xVelocity = player->xVelocity;
		result->yVelocity = player->yVelocity + PROJECTILE_VELOCITY;
		break;
	}
	result->creationTime = creationTime;
	return result;
}

void deallocateProjectile(Projectile *target) {
	free(target);
}

void deallocateProjectileShapeAndTextures() {
	deallocatePolygon(projUpPoly);
	deallocatePolygon(projRightPoly);
	deallocatePolygon(projDownPoly);
	deallocatePolygon(projLeftPoly);

	SDL_DestroyTexture(projUpTex);
	SDL_DestroyTexture(projRightTex);
	SDL_DestroyTexture(projDownTex);
	SDL_DestroyTexture(projLeftTex);
}

int moveProjectile(Projectile *projectile, int curTime) {
	if (curTime - projectile->creationTime > PROJECTILE_LIFE_TIME) {
		return 1;
	}
	else {
		projectile->xPos += projectile->xVelocity;
		projectile->yPos += projectile->yVelocity;
		return 0;
	}
}

void rescaleProjectilePositionAndMotion(Projectile *projectile, double *scale) {
	projectile->xPos *= scale[0];
	projectile->yPos *= scale[1];
	projectile->xVelocity *= scale[0];
	projectile->yVelocity *= scale[1];
}

void rescaleProjectileSpeed(double *scale) {
	PROJECTILE_VELOCITY *= scale[2];
}