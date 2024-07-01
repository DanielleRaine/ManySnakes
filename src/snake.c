#include "snake.h"


Snake *CreateSnake(Uint64 speed, SDL_Rect *box, int size, Direction direction)
{
	// a snake cannot have zero size. if less than 1, return NULL
	if (size < 1)
    	{
		return NULL;
    	}

	// create snake struct and set speed and direction
    	Snake *snake = malloc(sizeof(Snake));
	snake->speed = speed;
	snake->currentDirection = snake->pendingDirection = direction;

	// set shape of each SnakeNode
	snake->w = box->w;
	snake->h = box->h;

	// create snake head and set starting position and size of each node
    	snake->head = snake->tail = malloc(sizeof(SnakeNode));    
    	snake->head->x = box->x;
    	snake->head->y = box->y;
	snake->size = size;
	
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

void StepSnake(Snake *snake, SDL_Rect *bounds)
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

    	// update each node of snake to move it
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

void GrowSnake(Snake *snake, int x, int y)
{
	snake->tail->next = malloc(sizeof(SnakeNode));
	snake->tail = snake->tail->next;
	snake->tail->x = x;
	snake->tail->y = y;
	snake->tail->next = NULL;
	++snake->size;
}

bool CheckCollisionSnake(Snake *snake)
{
	// a snake of less than 5 nodes cannot collide with itself
	if (snake->size < 5)
	{
		return false;
	}

	// a snake of minimum 5 nodes has a 5th node to start from
	SnakeNode *cur = snake->head->next->next->next->next;
	while (cur)
	{
		// if the position of a node is the same as the head, then collision
		if (cur->x == snake->head->x && cur->y == snake->head->y)
		{
			return true;
		}

		cur = cur->next;
	}

	return false;
}

bool RenderSnake(SDL_Renderer *renderer, Snake *snake)
{
	SnakeNode *cur = snake->head;
	SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF);
	while (cur)
	{
		SDL_Rect rect = {cur->x, cur->y, snake->w, snake->h};
		if (SDL_RenderFillRect(renderer, &rect) != 0)
		{
			return false;
		}
		cur = cur->next;
	}

	return true;
}

void DestroySnake(Snake *snake)
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

Food *CreateFood(SDL_Renderer *renderer, FoodType type, SDL_Rect *box, const char *filepath)
{
	Food *food = malloc(sizeof(Food));
	food->type = type;
	food->image = IMG_LoadTexture(renderer, filepath);
	if (!(food && filepath))
	{
		free(food);
		return NULL;
	}

	food->box = *box;

	return food;
}

void RandPosFood(Food *food, Snake *snake, SDL_Rect *bounds)
{
	bool validPos = false;
	while (!validPos)
	{
		// get a random x and y position in a grid layout
		food->box.x = rand() % (bounds->w / food->box.w) * food->box.w + bounds->x;
		food->box.y = rand() % (bounds->h / food->box.h) * food->box.h + bounds->y;

		validPos = true;
		SnakeNode *cur = snake->head;
		while(cur)
		{
			// if the food exists in the same place as the snake, find new random position
			if (food->box.x == cur->x && food->box.y == cur->y)
			{
				validPos = false;
				break;
			}

			cur = cur->next;
		}
	}
}

bool RenderFood(SDL_Renderer *renderer, Food *food)
{
	if (SDL_RenderCopy(renderer, food->image, NULL, &food->box) != 0)
	{
		return false;
	}

	return true;	
}

void DestroyFood(Food *food)
{
	SDL_DestroyTexture(food->image);
	free(food);
}

