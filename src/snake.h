#ifndef SNAKE_H
#define SNAKE_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Declare Direction and FoodType enums.
 */

// describes the directions that snakes can take
typedef enum
{
   	RIGHT = 'r',
   	UP = 'u',
   	LEFT = 'l',
   	DOWN = 'd'
} Direction;

// describes the different types of foods that can appear
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
    	SDL_Rect body;
} SnakeNode;

// snake container struct that holds overall properties and the head SnakeNode
typedef struct 
{
    	SnakeNode *head;
    	SnakeNode *tail;
    	Uint64 speed;	// in units of milliseconds per block
    	Direction currentDirection;	// current direction of the snake
    	Direction pendingDirection;	// direction for the next move of the snake
    	int size;
    	Uint64 lastMoveTime;	// time in milliseconds since last move
    	Uint64 nextMoveTime;	// time in milliseconds until next move
} Snake;

// food for the snake to eat!!
typedef struct
{
    	FoodType type;
    	SDL_Rect shape;
    	SDL_Texture *image;	// image of the food
} Food;

Snake *MNYSNKS_CreateSnake(Uint64 speed, Direction direction, int xPos, int yPos, int wStep, int hStep, int size);
void MNYSNKS_DestroySnake(Snake *snake);

void MNYSNKS_StepSnake(Snake *snake, int xLeftBound, int xRightBound, int yUpBound, int yDownBound);

Food *MNYSNKS_CreateFood(SDL_Renderer *renderer, FoodType type, int width, int height, char filepath[]);
void MNYSNKS_DestroyFood(Food *food);


#endif
