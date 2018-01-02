#include <SDL.h>
#include <stdio.h>
#include "game_loop.h"
#include "title_screen.h"
#include "end_screen.h"
#include "window.h"
#include "projectile.h"
#include "player.h"
#include "enemy.h"

#define MIN_FRAME_TIME 8
#define MIN_SHOT_DELAY 300
#define MAX_PROJECTILES_OF_EACH_TYPE 50
#define MAX_ENEMIES_OF_EACH_TYPE 200
#define ENEMY_SPAWN_DELAY 500
#define ENEMY_LISTS 4
#define PROJ_LISTS 4

int titleSeen = 0;

int playing;
int quit;
int left;
int right;
int up;
int down;

int shootUp;
int shootRight;
int shootDown;
int shootLeft;

int lastShotTimeUp = -100;
int lastShotTimeRight = -100;
int lastShotTimeDown = -100;
int lastShotTimeLeft = -100;

SDL_Texture *tex;

Player *player;

Projectile *leftProjectiles[MAX_PROJECTILES_OF_EACH_TYPE];
Projectile *upProjectiles[MAX_PROJECTILES_OF_EACH_TYPE];
Projectile *rightProjectiles[MAX_PROJECTILES_OF_EACH_TYPE];
Projectile *downProjectiles[MAX_PROJECTILES_OF_EACH_TYPE];

int leftProjCount;
int upProjCount;
int rightProjCount;
int downProjCount;

Projectile *projLists[] = { upProjectiles, rightProjectiles, downProjectiles, leftProjectiles };
int *projCounts[] = { &upProjCount, &rightProjCount, &downProjCount, &leftProjCount };

Enemy *upEnemies[MAX_ENEMIES_OF_EACH_TYPE];
Enemy *rightEnemies[MAX_ENEMIES_OF_EACH_TYPE];
Enemy *downEnemies[MAX_ENEMIES_OF_EACH_TYPE];
Enemy *leftEnemies[MAX_ENEMIES_OF_EACH_TYPE];

int upEnemyCount;
int rightEnemyCount;
int downEnemyCount;
int leftEnemyCount;
int lastEnemySpawn;

Enemy *enemyLists[] = { upEnemies, rightEnemies, downEnemies, leftEnemies };
int *enemyCounts[] = { &upEnemyCount, &rightEnemyCount, &downEnemyCount, &leftEnemyCount };

/*Destroy and recreate every texture used in the game. Intended to be called when the window changes size.*/
void resetAllTextures() {
	extern void resetAllEnemyTextureLists();
	resetPlayerTexture(player);
	tex = player->tex;
	deallocateProjectileShapeAndTextures();
	createProjectileTextures();
	resetAllEnemyTextureLists();
}

/*Rescale the Positions of every enemy, projectile, and the player when the window changes size. Scale should be a double array of size three passed to toggleFullScreen.*/
void rescaleAllPositions(double *scale) {
	extern void rescaleAllEnemyLists(double *scale);
	rescalePlayerPositionAndMotion(player, scale);
	rescaleAllEnemyLists(scale);
	rescaleEnemyInitialSpeed(scale);
	extern void rescaleAllProjectileLists(double *scale);
	rescaleAllProjectileLists(scale);
	rescaleProjectileSpeed(scale);
}

/*Toggles the game between Fullscreen and windowed, resetting every texture.*/
void toggleFullscreenAndResetTextures() {
	double scale[3];
	toggleFullscreen(scale);
	resetAllTextures();
	rescaleAllPositions(scale);
}

/*Return the number of directions that the player is shooting in. Useful for limiting the number of directions in which they can fire.*/
int getNumDirectionsShooting() {
	return shootUp + shootRight + shootDown + shootLeft;
}

/*Handles the events for player input during the game.*/
void eventHandling() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {					//Player presses a button.
			if (event.key.keysym.scancode == SDL_SCANCODE_W)
				up = 1;
			else if (event.key.keysym.scancode == SDL_SCANCODE_S)
				down = 1;
			else if (event.key.keysym.scancode == SDL_SCANCODE_A)
				left = 1;
			else if (event.key.keysym.scancode == SDL_SCANCODE_D)
				right = 1;
			else if (event.key.keysym.scancode == SDL_SCANCODE_UP && getNumDirectionsShooting() < 2)
				shootUp = 1;
			else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT && getNumDirectionsShooting() < 2)
				shootRight = 1;
			else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN && getNumDirectionsShooting() < 2)
				shootDown = 1;
			else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT && getNumDirectionsShooting() < 2)
				shootLeft = 1;
			else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_LALT || event.key.keysym.scancode == SDL_SCANCODE_RALT) {	//player hits alt+enter
				const Uint8 *state = SDL_GetKeyboardState(0);
				if (state[SDL_SCANCODE_RETURN] && (state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT]))
					toggleFullscreenAndResetTextures();
			}
		}
		else if (event.type == SDL_KEYUP) {						//player releases a button.
			if (event.key.keysym.scancode == SDL_SCANCODE_W)
				up = 0;
			else if (event.key.keysym.scancode == SDL_SCANCODE_S)
				down = 0;
			else if (event.key.keysym.scancode == SDL_SCANCODE_A)
				left = 0;
			else if (event.key.keysym.scancode == SDL_SCANCODE_D)
				right = 0;
			else if (event.key.keysym.scancode == SDL_SCANCODE_UP)
				shootUp = 0;
			else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
				shootRight = 0;
			else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
				shootDown = 0;
			else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
				shootLeft = 0;
		}
		else if (event.type == SDL_QUIT) {					//player attempts to close the window.
			playing = 0;
			quit = 1;
		}
	}
}

/*Move the player. Passes the time of this frame and the last in miliseconds.*/
void playerMove(int lastTime, int curTime) {
	acceleratePlayer(player, left, up, right, down);
	movePlayer(player);
	if (curTime - lastShotTimeUp > MIN_SHOT_DELAY && shootUp && upProjCount < MAX_PROJECTILES_OF_EACH_TYPE) {
		upProjectiles[upProjCount] = createProjectile(player, DIRECTION_UP, curTime);
		upProjCount++;
		lastShotTimeUp = curTime;
	}
	if (curTime - lastShotTimeRight > MIN_SHOT_DELAY && shootRight && rightProjCount < MAX_PROJECTILES_OF_EACH_TYPE) {
		rightProjectiles[rightProjCount] = createProjectile(player, DIRECTION_RIGHT, curTime);
		rightProjCount++;
		lastShotTimeRight = curTime;
	}
	if (curTime - lastShotTimeDown > MIN_SHOT_DELAY && shootDown && downProjCount < MAX_PROJECTILES_OF_EACH_TYPE) {
		downProjectiles[downProjCount] = createProjectile(player, DIRECTION_DOWN, curTime);
		downProjCount++;
		lastShotTimeDown = curTime;
	}
	if (curTime - lastShotTimeLeft > MIN_SHOT_DELAY && shootLeft && leftProjCount < MAX_PROJECTILES_OF_EACH_TYPE) {
		leftProjectiles[leftProjCount] = createProjectile(player, DIRECTION_LEFT, curTime);
		leftProjCount++;
		lastShotTimeLeft = curTime;
	}
}

/*Remove a projectile at a specific index from a list of projectiles.  Pass a pointer to the size of the lists; the size will be updated if the list changes size*/
void removeProjectileFromList(int index, int *count, Projectile **list) {
	deallocateProjectile(list[index]);
	for (int i = index; i < *count -  1; i++) {
		list[i] = list[i+1];
	}
	*count -= 1;
}

/*Check if any Projectile in the arguement list hit any enemy in the arguement list. Pass pointers to the size of the lists; the size will be updated if the list changes size.*/
projectileListHitEnemyListDetection(int direction, int *countProj, Projectile **list1, int *countEnemy, Enemy **list2) {
	Polygon *projShape;
	if (direction == DIRECTION_LEFT)
		projShape = projLeftPoly;
	else if (direction == DIRECTION_DOWN)
		projShape = projDownPoly;
	else if (direction == DIRECTION_RIGHT)
		projShape = projRightPoly;
	else if (direction == DIRECTION_UP)
		projShape = projUpPoly;

	for (int i = 0; i < *countProj; i++) {
		for (int j = 0; j < *countEnemy; j++) { 
			if (polygonsIntersect(projShape, (*(list2 + j))->shape, (*(list1 + i))->xPos, (*(list1 + i))->yPos, (*(list2 + j))->xPos, (*(list2 + j))->yPos)) {
				removeProjectileFromList(i, countProj, list1);
				extern void removeEnemyFromList(int index, int *count, Enemy **list);
				removeEnemyFromList(j, countEnemy, list2);
			}
		}
	}
}

/*Check if any enemy in the list hit the player. If true the game ends (playing is set to zero).*/
void enemiesListHitPlayerDetection(int count, Enemy **list) {
	for (int i = 0; i < count; i++) {
		if (polygonsIntersect(player->playerShape, list[i]->shape, player->xPos, player->yPos, list[i]->xPos, list[i]->yPos)) {
			playing = 0;
		}
	}
}

/*Call enemiesListHitPlayerDetection on all enemy lists.*/
void allEnemyListsHitPlayerDetection() {
	for (int i = 0; i < ENEMY_LISTS; i++) {
		enemiesListHitPlayerDetection(*enemyCounts[i], enemyLists[i]);
	}
}

/*Move all projectiles in a list. The projectiles are removed from the list if they have been active too long. A pointer to the projectile count is passed and
updated the count is updated by this function if necessary.*/
void moveProjectileList(int *count, Projectile **list, int curTime) {
	for (int i = 0; i < *count; i++) {
		if (moveProjectile(*(list + i), curTime)) {
			removeProjectileFromList(0, count, list);
		}
	}
}

/*Call moveProjectileList on all projectile lists.*/
void moveAllProjectileLists(int curTime) {
	for (int i = 0; i < PROJ_LISTS; i++) {
		moveProjectileList(projCounts[i], projLists[i], curTime);
	}
}

/*Remove an enemy at a specific index from a list of projectiles.  Pass a pointer to the size of the list; the size will be updated if the list changes size.*/
void removeEnemyFromList(int index, int *count, Enemy **list) {
	deallocateEnemy(list[index]);
	for (int i = index; i < *count - 1; i++) {
		list[i] = list[i+1];
	}
	*count -= 1;
}

void projectilesMove(int curTime, int lastTime) {
	moveAllProjectileLists(curTime);

	/*Checks hit detection for enemies and projectiles of the same color.*/
	projectileListHitEnemyListDetection(DIRECTION_LEFT, &leftProjCount, leftProjectiles, &leftEnemyCount, leftEnemies);
	projectileListHitEnemyListDetection(DIRECTION_UP, &upProjCount, upProjectiles, &upEnemyCount, upEnemies);
	projectileListHitEnemyListDetection(DIRECTION_RIGHT, &rightProjCount, rightProjectiles, &rightEnemyCount, rightEnemies);
	projectileListHitEnemyListDetection(DIRECTION_DOWN, &downProjCount, downProjectiles, &downEnemyCount, downEnemies);
}

/*Move and accelerate all enemies in a list of enemies.*/
void moveAndAccelerateEnemyList(int *count, Enemy **list, int curTime) {
	for (int i = 0; i < *count; i++) {
		if (enemyShouldBeDeleted(*(list + i), curTime)) {
			removeEnemyFromList(i, count, list);
		}
		else {
			accelerateEnemy(player->xPos, player->yPos, *(list + i));
			moveEnemy(*(list + i));
		}
	}
}

/*Call moveAndAccelerateEnemyList on all enemy lists.*/
void moveAndAccelerateAllEnemyLists(int curTime) {
	for (int i = 0; i < ENEMY_LISTS; i++) 
		moveAndAccelerateEnemyList(enemyCounts[i], enemyLists[i], curTime);
}

/*Reset the texture of every enemy in a list of enemies.*/
void resetEnemyTextureList(int count, Enemy **list) {
	for (int i = 0; i < count; i++) 
		resetEnemyTexture(list[i]);
}

/*Call resetEnemyTextureList on every list of enemies.*/
void resetAllEnemyTextureLists() {
	for (int i = 0; i < ENEMY_LISTS; i++)
		resetEnemyTextureList(*enemyCounts[i], enemyLists[i]);
}

/*Move all enemies.*/
void enemyMove(int curTime, int lastTime) {
	if (curTime - lastEnemySpawn > ENEMY_SPAWN_DELAY) {
		Enemy *newEnemy = createEnemy(curTime);
		setEnemyInitialVelocity(player->xPos, player->yPos, newEnemy);
		switch (newEnemy->color) {
		case COLOR_RED:
			if (leftEnemyCount < MAX_ENEMIES_OF_EACH_TYPE) {
				leftEnemies[leftEnemyCount] = newEnemy;
				leftEnemyCount++;
			}
			break;
		case COLOR_BLUE:
			if (upEnemyCount < MAX_ENEMIES_OF_EACH_TYPE) {
				upEnemies[upEnemyCount] = newEnemy;
				upEnemyCount++;
			}
			break;
		case COLOR_GREEN:
			if (rightEnemyCount < MAX_ENEMIES_OF_EACH_TYPE) {
				rightEnemies[rightEnemyCount] = newEnemy;
				rightEnemyCount++;
			}
			break;
		case COLOR_YELLOW:
			if (downEnemyCount < MAX_ENEMIES_OF_EACH_TYPE) {
				downEnemies[downEnemyCount] = newEnemy;
				downEnemyCount++;
			}
		}
		lastEnemySpawn = curTime;
	}
	moveAndAccelerateAllEnemyLists(curTime);
}

/*Move all entities in the game.*/
void moveEntities(int lastTime, int curTime) {
	playerMove(lastTime, curTime);
	projectilesMove(curTime, lastTime);
	enemyMove(curTime, lastTime);
}

/*Render a list of projectiles. All projectiles in the list are assumed to be going in the same direction. This direction is passed as an argument.*/
void renderProjectileList(int direction, int count, Projectile **list) {
	SDL_Texture *projTex;
	if (direction == DIRECTION_LEFT)
		projTex = projLeftTex;
	else if (direction == DIRECTION_DOWN)
		projTex = projDownTex;
	else if (direction == DIRECTION_RIGHT)
		projTex = projRightTex;
	else if (direction == DIRECTION_UP)
		projTex = projUpTex;

	for (int i = 0; i < count; i++) {
		drawTexture(projTex, (int)(*(list + i))->xPos, (int)(*(list + i))->yPos, PROJECTILE_WIDTH, PROJECTILE_HEIGHT);
	}
}

/*Render all of the projectiles.*/
void renderProjectiles() {
	renderProjectileList(DIRECTION_UP, upProjCount, upProjectiles);
	renderProjectileList(DIRECTION_RIGHT, rightProjCount, rightProjectiles);
	renderProjectileList(DIRECTION_DOWN, downProjCount, downProjectiles);
	renderProjectileList(DIRECTION_LEFT, leftProjCount, leftProjectiles);
}

/*Render all enemies in a list of enemies.*/
void renderEnemyList(int count, Enemy **list) {
	for (int i = 0; i < count; i++) {
		drawTexture((*(list + i))->tex, (int)(*(list + i))->xPos, (int)(*(list + i))->yPos, ENEMY_WIDTH, ENEMY_HEIGHT);
	}
}

/*call renderEnemyList on all lists of enemies.*/
void renderAllEnemyLists() {
	for (int i = 0; i < ENEMY_LISTS; i++)
		renderEnemyList(*enemyCounts[i], enemyLists[i]);
}

/*Render every enemy. Calls renderAllEnemyLists.*/
void renderEnemies() {
	renderAllEnemyLists();
}

/*Renders on frame of the game.*/
void renderFrame() {
	clearScreen();
	drawTexture(tex, (int)player->xPos, (int)player->yPos, PLAYER_WIDTH, PLAYER_HEIGHT);
	renderProjectiles();
	renderEnemies();
	drawScreen();
}

/*Sets initial values for all variables in the game.*/
void initializeVariables() {
	playing = 1;
	quit = 0;

	left = 0;
	right = 0;
	up = 0;
	down = 0;

	shootUp = 0;
	shootRight = 0;
	shootDown = 0;
	shootLeft = 0;

	leftProjCount = 0;
	upProjCount = 0;
	rightProjCount = 0;
	downProjCount = 0;

	upEnemyCount = 0;
	rightEnemyCount = 0;
	downEnemyCount = 0;
	leftEnemyCount = 0;
	lastEnemySpawn = 0;
}

/*Runs the main game loop.*/
void runGameLoop() {
	initializeVariables();
	if (!titleSeen) {
		runTitleScreen();
		titleSeen = 1;
	}
	int lastTime = -100;
	int curTime = 0;

	player = createPlayer();
	tex = player->tex;
	createProjectileTextures();

	while (playing) {
		lastTime = SDL_GetTicks();
		eventHandling();
		moveEntities(curTime, lastTime);
		allEnemyListsHitPlayerDetection();
		renderFrame();
		curTime = SDL_GetTicks();
		if (MIN_FRAME_TIME + lastTime - curTime > 0) 
			SDL_Delay(MIN_FRAME_TIME + lastTime - curTime);
	}
	deallocateProjectileShapeAndTextures();
	SDL_DestroyTexture(tex);
	if (quit) {
		quitWindow();
	}
	else {
		runEndScreen();
		runGameLoop();
	}
}

/*Rescales a list of enemies based on a change in window resolution.*/
void rescaleEnemyList(int count, double *scale, Enemy **list) {
	for (int i = 0; i < count; i++) 
		rescaleEnemyPositionAndMotion(list[i], scale);
}

/*calls rescaleEnemyList on all lists of enemies.*/
void rescaleAllEnemyLists(double *scale) {
	for (int i = 0; i < ENEMY_LISTS; i++)
		rescaleEnemyList(*enemyCounts[i], scale, enemyLists[i]);
}

/*Rescales a list of projectiles based on a change in window resolution.*/
void rescaleProjectileList(int count, double *scale, Projectile **list) {
	for (int i = 0; i < count; i++) 
		rescaleProjectilePositionAndMotion(list[i], scale);
}

/*Calls rescaleProjectileList on all projectile lists.*/
void rescaleAllProjectileLists(double *scale) {
	for (int i = 0; i < PROJ_LISTS; i++)
		rescaleProjectileList(*projCounts[i], scale, projLists[i]);
}