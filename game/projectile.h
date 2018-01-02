#pragma once

#include<SDL.h>
#include "shape.h"
#include "enemy.h"
#include "player.h"

SDL_Texture *projUpTex;
SDL_Texture *projRightTex;
SDL_Texture *projDownTex;
SDL_Texture *projLeftTex;

Polygon *projUpPoly;
Polygon *projRightPoly;
Polygon *projDownPoly;
Polygon *projLeftPoly;

int PROJECTILE_WIDTH;
int PROJECTILE_HEIGHT;

typedef struct ProjectileTag {
	double xPos;
	double yPos;
	double xVelocity;
	double yVelocity;
	int creationTime;
	Color color;
} Projectile;

typedef enum { DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT } Direction;

void createProjectileTextures();

Projectile *createProjectile(Player *player, Direction direction, int curTime);

int moveProjectile(Projectile *projectile, int curTime);

void deallocateProjectileShapeAndTextures();

void deallocateProjectile(Projectile *target);

/*Rescale the position and motion of a projectile after the screen is resized.*/
void rescaleProjectilePositionAndMotion(Projectile *projectile, double *scale);

/*Rescale the speed of new projectiles after the screen is resized.*/
void rescaleProjectileSpeed(double *scale);