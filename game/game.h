#include <SDL.h>

/*Headers for window.c*/
int SCREEN_WIDTH;
int SCREEN_HEIGHT;


/**
Creates a New SDL window and renderer.
*/
int createWindow();

/**
Makes the window "fullscreen" if it is currently "windowed", and makes the window "windowed" if it is currently "fullscreen".
*/
int toggleFullscreen();

/**
Renders arguemnt surface to a texture and returns a pointer to that texture.
*/
SDL_Texture *renderSurfaceToTexture(SDL_Surface *surface);

/**
Clears the screen/renderer of all images.
*/
void clearScreen();

/**
Draws argument texture to the screen. Four parameters are the x coordinate of the texture on the screen,
the y coordinate, the width in pixels, and the height in pixels.
*/
int drawTexture(SDL_Texture *tex, int x, int y, int w, int h);

/**
Presents the window with everything that has been drawn to the renderer.
*/
void drawScreen();

/**
*Closes window and quits SDL.
*/
void quitWindow();

/**
*Returns a pointer to a new SDL texture with argument width and height. The texture starts completely transparent.
*/
SDL_Texture *createTexture(int w, int h);

/**
*Creates a new texture with argument width and height that will be drawn to by subsiquent calls to writeToBuildTexture.
*/
void setBuildTexture(int w, int h);

/**
*Draws argument texture to the texture created by setBuildTexture at the location of the arguement destination SDL_Rect.
*Returns 0 on success and 1 on error.
*/
int writeToBuildTexture(SDL_Texture *tex1, SDL_Rect *dest);

/**
*Returns the texture created by setBuildTexture.
*/
SDL_Texture *getBuildTexture(int w, int h);

/**
*Copies the texture passed as the second argument to the texture passed as the first argument. Returns 0 on success and 1 on error.
*/
int scaleTexture(SDL_Texture *target, SDL_Texture *texture);

/**
*Renders a horizontal line on the argument texture. First three integer arguments are the initial and final x position of the line and the y
*position of the line. The next three are the rgb values of the line.
*/
int renderHorizontalLineOnTexture(SDL_Texture *target, int xi, int xf, int y, int r, int g, int b);

/*End of headers for window.c*/

/*Headers for shape.c*/

typedef struct PointTag{
	double x;
	double y;
} Point;

typedef struct LineTag {
	Point initial;
	Point final;
} Line;

typedef struct PolygonTag {
	int numFaces;
	int width;
	int height;
	Line *sides;
} Polygon;

/**
Creates a polygon object from an array of x, y coordinate pairs.
*/
Polygon *createPolygonFromArrayOfXYPairs(int numFaces, int width, int height, double *xYPairs);

/**
Frees memory used to store passed polygon
*/
void deallocatePolygon(Polygon *removed);

/**
Finds the point of intersection between two line segments and stores that point in result.
One is returned if there is no intersection or the lines intersect at multiple points, and
zero is returned if an itersection is successfully found
*/
int findIntersection(Line *one, Line *two, Point *result);

/**
*Compares two doubles to see if they are almost equal. returns 1 if they are and 0 if they are not. The purpose of this
*function is to correct for floating point errors.
*/
int almostEqual(double one, double two);

/**
*Takes two Polygons followed by doubles with the x, y coordinates of both polygons as arguments 
*and returns one if they intersect and two otherwise.
*/
int polygonsIntersect(Polygon *poly1, Polygon *poly2, double x1, double y1, double x2, double y2);

/*End of headers for shape.c*/

/*Headers for draw_engine.c*/

/**
*Returns an SDL_Texture that is the image of the argument polygon filled with the color passed as the three values r, g, and b.
*These values are rgb values from 0 to 255.
*/
SDL_Texture *fillPolygon(Polygon *polygon, int r, int g, int b);

/*End of headers for draw_engine.c*/

/**
Runs the main loop to play the game.
*/
void runGameLoop();

/*Headers for enemy.c*/

 typedef enum {COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_YELLOW} Color;

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

/*End of headers for enemy.c*/

/*Headers for random_until.c*/

/**
Returns a random double between 0 and 1. Not all values are possible or equally likely due to rounding.
*/
double randDouble();

/**
Returns a random integer between the values of the two arguments inclusive of the first and exclusive of the second. First argument should
be the lower bound.
*/
int randRange(int i, int f);

/*End of headers for random_util.c*/

/*Start of headers for player.c*/

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

/**
*Redraws the players texture.
*/
void resetPlayerTexture(Player *player);

/*End of headers for player.c*/

/*Start of headers for projectile.c*/
int PROJECTILE_WIDTH;
int PROJECTILE_HEIGHT;

typedef struct ProjectileTag {
	Polygon *shape;
	SDL_Texture *tex;
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

/*End of headers for projectile.c*/