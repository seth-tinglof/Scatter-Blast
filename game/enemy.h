#pragma once

#include <SDL.h>
#include "shape.h"

/*The different colors that enemies can be: COLOR_RED, COLOR_BLUE, COLOR_GREEN, and COLOR_YELLOW.*/
typedef enum { COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_YELLOW } Color;

int ENEMY_WIDTH;
int ENEMY_HEIGHT;

typedef struct EnemyTag
{
	SDL_Texture *tex;
	Polygon *shape;
	int creationTime;
	double xPos;
	double yPos;
	double xVelocity;
	double yVelocity;
	Color color;
} Enemy;

/**
*Creates and returns a pointer to a new enemy. When no longer needed, this pointer should be passed to deallocate enemy.
*The fields for this enemies position, shape, color, and texture are given pseudo-random values that are consistent with the games rules.
*The parameter creation time is the time, in miliseconds, that this enemy was created at.
*/
Enemy *createEnemy(int creationTime);

/**
*Sets the texture field of the argument enemy.
*/
void createEnemyTexture(Enemy *result);

/*Resets the texture of the passed enemy*/
void resetEnemyTexture(Enemy *enemy);

/**
*Pass a pointer to an enemy created by create enemy to have this enemy be removed from memory.
*/
void deallocateEnemy(Enemy *enemy);

void setEnemyInitialVelocity(double playerX, double playerY, Enemy *enemy);

/**
*Changes the velocity fields for the enemy to accelerate it in the direction of the player.
*/
void accelerateEnemy(double playerX, double playerY, Enemy *enemy);

/**
*Move argument enemy based on its velociy fields.
*/
void moveEnemy(Enemy *enemy);

int enemyShouldBeDeleted(Enemy *enemy, int currentTime);

/*Rescales the position and motion of an enemy after a screen resize.*/
void rescaleEnemyPositionAndMotion(Enemy *enemy, double *scale);

/*Rescales the intial speed of an enemy after a screen resize.*/
void rescaleEnemyInitialSpeed(double *scale);