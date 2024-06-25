#include "snake.h"


Snake *MNYSNKS_CreateSnake(Uint64 speed, SDL_Rect *body, int size, Direction direction)
{
	// a snake cannot have zero size. if less than 1, return NULL
	if (size < 1)
    	{
		return NULL;
    	}

	// create snake struct and set speed and direction
    	Snake *snake = malloc(sizeof(Snake));
    	
	snake->speed = speed;

	snake->w = body->w;
	snake->h = body->h;

	snake->currentDirection = snake->pendingDirection = direction;
    	
	// create snake head and set starting position and size of each node
    	snake->head = snake->tail = malloc(sizeof(SnakeNode));    
    	snake->head->x = body->x;
    	snake->head->y = body->y;


	// create the rest of the snake
    	for (int i = 1; i < size; ++i)
    	{
    	    	snake->tail->next = malloc(sizeof(SnakeNode));
    	    	snake->tail->next->x = snake->tail->x;
    	    	snake->tail->next->y = snake->tail->y + snake->h;
    	    	snake->tail = snake->tail->next;
    	}
    	snake->tail->next = NULL;

    	return snake;
}

void MNYSNKS_StepSnake(Snake *snake, SDL_Rect *bounds)
{
    	snake->currentDirection = snake->pendingDirection;
    	int xDelta, yDelta;
    	// displace the snake depending on the direction it was set to move
    	switch (snake->currentDirection)
    	{
		case RIGHT:
			xDelta = snake->w;
    	        	yDelta = 0;
    	        	break;
    	    	case UP:
    	    	    	xDelta = 0;
    	    	    	yDelta = -snake->h;
    	    	    	break;
    	    	case LEFT:
    	    	    	xDelta = -snake->w;
    	    	    	yDelta = 0;
    	    	    	break;
    	    	case DOWN:
    	    	    	xDelta = 0;
    	    	    	yDelta = snake->h;
    	    	    	break;
    	    	default:
    	    	    	return;
    	}
    	SnakeNode *cur = snake->head;
    	int xNext = cur->x + xDelta;
    	int yNext = cur->y + yDelta;

    	// if the snake is out of bounds, loop to other side
    	if (xNext < bounds->x) // snake went left out of bounds
    	{
    	    	xNext += bounds->w;
    	}

    	if (xNext >= bounds->x + bounds->w) // snake went right out of bounds
    	{
    	    	xNext = bounds->x;
    	}

    	if (yNext < bounds->y) // snake went up out of bounds
    	{
    	    	yNext += bounds->h;
    	}

    	if (yNext >= bounds->y + bounds->h) // snake went down out of bounds
    	{
    	    	yNext = bounds->y;
    	}

    	// update body of the snake to move it
    	while (cur)
    	{
    	    	int xLast = cur->x;
    	    	int yLast = cur->y;
    	    	cur->x = xNext;
    	    	cur->y = yNext;
    	    	xNext = xLast;
    	    	yNext = yLast;
    	    	cur = cur->next;
    	}
}

void MNYSNKS_DestroySnake(Snake *snake)
{
	// loop through every SnakeNode and free it
    	while (snake->head)
    	{
    	    	SnakeNode *next = snake->head->next;
    	    	free(snake->head);
    	    	snake->head = next;
    	}

	// free Snake struct
    	free(snake);
}

Food *MNYSNKS_CreateFood(SDL_Renderer *renderer, FoodType type, SDL_Rect *body, const char *filepath)
{
	Food *food = malloc(sizeof(Food));
	food->type = type;
	food->image = IMG_LoadTexture(renderer, filepath);
	if (!(food && filepath))
	{
		free(food);
		return NULL;
	}

	food->body = *body;

	return food;
}

void MNYSNKS_RandPosFood(Food *food, Snake *snake, SDL_Rect *bounds)
{
	srand(SDL_GetTicks());

	bool validPos = false;
	while (!validPos)
	{
		food->body.x = rand() % (bounds->w / food->body.w) * food->body.w + bounds->x;
		food->body.y = rand() % (bounds->h / food->body.h) * food->body.h + bounds->y;

		validPos = true;
		SnakeNode *cur = snake->head;
		while(cur)
		{
			if (food->body.x == cur->x || food->body.y == cur->y)
			{
				validPos = false;
				break;
			}

			cur = cur->next;
		}
	}
}

void MNYSNKS_DestroyFood(Food *food)
{
	SDL_DestroyTexture(food->image);
	free(food);
}

