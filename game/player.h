#pragma once

#include <SDL.h>
#include "shape.h"

int PLAYER_WIDTH;
int PLAYER_HEIGHT;

typedef struct PlayerTag {
	Polygon *playerShape;
	SDL_Texture *tex;
	double xPos;
	double yPos;
	double xVelocity;
	double yVelocity;
} Player;

/**
*Returns a pointer to a new player with initial position, texture, and shape already filled out.
*/
Player *createPlayer();

/**
*Accelerates argument player. Values left, up, right, and down should be zero if the human player is not holding down the key to go in
*the corresponding direction and one if the human player is holding down the key to go in the corresponding direction.
*/
void acceleratePlayer(Player *player, int left, int up, int right, int down);

/**
*Move the argument player based on their velocity.
*/
void movePlayer(Player *player);

/* Redraws the players texture.*/
void resetPlayerTexture(Player *player);

/*Rescales the players position and motion based on a change in resolution.*/
void rescalePlayerPositionAndMotion(Player *player, double *scale);

void rescalePlayerSpeed(double *scale);