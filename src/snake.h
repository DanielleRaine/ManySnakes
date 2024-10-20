#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Declare Direction and FoodType enums.
 */

// directions that snakes can take
typedef enum
{
   	SNAKE_RIGHT = 'r',
   	SNAKE_UP = 'u',
   	SNAKE_LEFT = 'l',
   	SNAKE_DOWN = 'd'
} SnakeDirection;

// different types of foods that can appear
typedef enum
{
    	FOOD_APPLE = 'a'
} FoodType;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Declare Snake, SnakeNode, Food structs.
 */	

// the parts of the snake that are displayed to screen and their properties
// in the form of a linked list node
typedef struct SnakeNode
{
    	struct SnakeNode *next;
	int xPos, yPos;
} SnakeNode;

// snake container struct that holds overall properties and the head SnakeNode
typedef struct Snake
{
    	SnakeNode *head, *tail;
    	Uint64 speed;	// in units of milliseconds per block
    	int nodeWidth, nodeHeight;	// width and height of each SnakeNode
	int length;	// number of SnakeNodes
	SnakeDirection currentDirection;	// current direction of the snake
    	SnakeDirection pendingDirection;	// direction for the next move of the snake
    	Uint64 lastMoveTime;	// time in milliseconds since last move
    	Uint64 nextMoveTime;	// time in milliseconds until next move
	SDL_Color color;	// color of SnakeNodes
} Snake;

// food for the snake to eat!!
typedef struct Food
{
    	FoodType type;
	int xPos, yPos;
	int textureWidth, textureHeight;
    	SDL_Texture *texture;	// food texture
} Food;

Snake *CreateSnake(int xPos, int yPos, int nodeWidth, int nodeHeight, Uint64 speed, int length, SnakeDirection direction, SDL_Color *color);
void StepSnake(Snake *snake, int xMax, int yMax);
void GrowSnake(Snake *snake, int xNew, int yNew);
bool CheckCollisionSnake(Snake *snake);
bool RenderSnake(SDL_Renderer *renderer, Snake *snake, int xOrigin, int yOrigin, int xMultiplier, int yMultiplier);
void DestroySnake(Snake *snake);

Food *CreateFood(SDL_Renderer *renderer, FoodType type, int xPos, int yPos, int textureWidth, int textureHeight, const char *filepath);
void RandPosFood(Food *food, Snake *snake, int xMax, int yMax);
bool RenderFood(SDL_Renderer *renderer, Food *food, int xOrigin, int yOrigin, int xMultiplier, int yMultiplier);
void DestroyFood(Food *food);


#endif
