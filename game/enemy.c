#include <stdlib.h>
#include <math.h>
#include"draw_engine.h"
#include"random_util.h"
#include"enemy.h"
#include"player.h"
#include"window.h"

#define ENEMY_LIFE_TIME 10000
#define ENEMY_ACCELERATION  .03
#define ENEMY_DRAG .99

double ENEMY_SPEED = 3;

void assignRandomEnemyPosition(Enemy *result) {
	switch (result->color) {
	case 0:
		result->xPos = -100;
		result->yPos = randRange(0, SCREEN_HEIGHT);
		break;
	case 1:
		result->xPos = randRange(0, SCREEN_WIDTH);
		result->yPos = -100;
		break;
	case 2:
		result->xPos = SCREEN_WIDTH;
		result->yPos = randRange(0, SCREEN_HEIGHT);
		break;
	case 3:
		result->xPos = randRange(0, SCREEN_HEIGHT);
		result->yPos = SCREEN_HEIGHT;
		break;
	}
}

Polygon *createEnemyShape() {
	int MIN_RADIUS = SCREEN_WIDTH / 85;
	ENEMY_WIDTH = SCREEN_WIDTH / 17;
	ENEMY_HEIGHT = ENEMY_WIDTH;
	int sides = randRange(3, 7);
	double *points = malloc(2 * sides * sizeof(double));	//points is an array of x,y pairs for the endpoints of the sides.
	double angle = 0;
	double radius = 0;
	for (int i = 0; i < sides; i++) {
		radius = randDouble() * (ENEMY_WIDTH / 2 - MIN_RADIUS) + MIN_RADIUS; //random radius between MIN_RADIUS and half the enemy width.
		points[2*i] = ENEMY_WIDTH / 2 + radius * cos(angle);
		points[2*i+1] = ENEMY_WIDTH / 2 + radius * sin(angle);
		angle += 2 * M_PI / sides;									//increments angle for next point.
	}
	Polygon *result = createPolygonFromArrayOfXYPairs(sides, ENEMY_WIDTH, ENEMY_HEIGHT, points);
	free(points);
	return result;
}

void createEnemyTexture(Enemy *result) {
	switch (result->color) {
	case COLOR_RED:
		result->tex = fillPolygon(result->shape, 250, 0, 0);
		break;
	case COLOR_BLUE:
		result->tex = fillPolygon(result->shape, 0, 0, 250);
		break;
	case COLOR_GREEN:
		result->tex = fillPolygon(result->shape, 0, 250, 0);
		break;
	case COLOR_YELLOW:
		result->tex = fillPolygon(result->shape, 230, 230, 50);
		break;
	}
}

Enemy *createEnemy(int creationTime) {
	Enemy *result = malloc(sizeof(Enemy));
	int col = randRange(0, 4);
	switch (col)
	{
	case 0:
		result->color = COLOR_RED;
		break;
	case 1:
		result->color = COLOR_BLUE;
		break;
	case 2:
		result->color = COLOR_GREEN;
		break;
	case 3:
		result->color = COLOR_YELLOW;
		break;
	}
	assignRandomEnemyPosition(result);
	result->shape = createEnemyShape();
	result->xVelocity = 0.0;
	result->yVelocity = 0.0;
	result->creationTime = creationTime;
	createEnemyTexture(result);
	return result;
}

void resetEnemyTexture(Enemy *enemy) {
	SDL_DestroyTexture(enemy->tex);
	createEnemyTexture(enemy);
}

void deallocateEnemy(Enemy *enemy) {
	deallocatePolygon(enemy->shape);
	SDL_DestroyTexture(enemy->tex);
	free(enemy);
}

void setEnemyInitialVelocity(double playerX, double playerY, Enemy *enemy) {
	double angle = atan2(playerY + PLAYER_HEIGHT / 2 - enemy->yPos - ENEMY_HEIGHT / 2, playerX + PLAYER_WIDTH / 2 - enemy->xPos - ENEMY_WIDTH / 2);
	if(enemy->color == COLOR_RED || enemy->color == COLOR_GREEN)
		enemy->xVelocity = ENEMY_SPEED * cos(angle);
	else
		enemy->yVelocity = ENEMY_SPEED * sin(angle);
}

void accelerateEnemy(double playerX, double playerY, Enemy *enemy) {
	double angle = atan2(playerY + PLAYER_HEIGHT / 2 - enemy->yPos - ENEMY_HEIGHT / 2, playerX + PLAYER_WIDTH / 2 - enemy->xPos - ENEMY_WIDTH / 2);
	if(enemy->color == COLOR_BLUE || enemy->color == COLOR_YELLOW)
		enemy->xVelocity = (enemy->xVelocity + ENEMY_ACCELERATION * cos(angle)) * ENEMY_DRAG;
	else
		enemy->yVelocity = (enemy->yVelocity + ENEMY_ACCELERATION * sin(angle)) * ENEMY_DRAG;
}

void moveEnemy(Enemy *enemy) {
	enemy->xPos += enemy->xVelocity;
	enemy->yPos += enemy->yVelocity;
}

int enemyShouldBeDeleted(Enemy *enemy, int currentTime) {
	if (currentTime - enemy->creationTime > ENEMY_LIFE_TIME)
		return 1;
	return 0;
}

void rescaleEnemyPositionAndMotion(Enemy *enemy, double *scale) {
	enemy->xPos *= scale[0];
	enemy->yPos *= scale[1];
	enemy->xVelocity *= scale[0];
	enemy->yVelocity *= scale[1];
}

void rescaleEnemyInitialSpeed(double *scale) {
	ENEMY_SPEED *= scale[2];
}