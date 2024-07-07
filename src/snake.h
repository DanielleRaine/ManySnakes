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
   	RIGHT = 'r',
   	UP = 'u',
   	LEFT = 'l',
   	DOWN = 'd'
} Direction;

// different types of foods that can appear
typedef enum
{
    	APPLE = 'a'
} FoodType;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Declare Snake, SnakeNode, Food structs.
 */	

// the parts of the snake that are displayed to screen and their properties
// in the form of a linked list node
typedef struct SnakeNode
{
    	struct SnakeNode *next;
	int x, y;
} SnakeNode;

// snake container struct that holds overall properties and the head SnakeNode
typedef struct Snake
{
    	SnakeNode *head, *tail;
    	Uint64 speed;	// in units of milliseconds per block
    	int w, h;	// width and height of each SnakeNode
	int size;	// number of SnakeNodes
	Direction currentDirection;	// current direction of the snake
    	Direction pendingDirection;	// direction for the next move of the snake
    	Uint64 lastMoveTime;	// time in milliseconds since last move
    	Uint64 nextMoveTime;	// time in milliseconds until next move
} Snake;

// food for the snake to eat!!
typedef struct Food
{
    	FoodType type;
    	SDL_Rect box;
    	SDL_Texture *image;	// image of the food
} Food;

Snake *CreateSnake(Uint64 speed, SDL_Rect *box, int size, Direction direction);
void StepSnake(Snake *snake, SDL_Rect *bounds);
void GrowSnake(Snake *snake, int x, int y);
bool CheckCollisionSnake(Snake *snake);
bool RenderSnake(SDL_Renderer *renderer, Snake *snake);
void DestroySnake(Snake *snake);

Food *CreateFood(SDL_Renderer *renderer, FoodType type, SDL_Rect *box, const char *filepath);
void RandPosFood(Food *food, Snake *snake, SDL_Rect *bounds);
bool RenderFood(SDL_Renderer *renderer, Food *food);
void DestroyFood(Food *food);


#endif
